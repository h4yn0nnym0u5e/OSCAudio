/* Open Sound Control for Audio Library for Teensy 3.x, 4.x
 * Copyright (c) 2021, Jonathan Oakley, teensy-osc@0akley.co.uk
 *
 * Development of this library was enabled by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards, implementing libraries, and maintaining
 * the forum at https://forum.pjrc.com/ 
 *
 * Please support PJRC's efforts to develop open source software by 
 * purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#if !defined(_OSCAUDIOBASE_H_)
#define _OSCAUDIOBASE_H_

#include <OSCBundle.h>
#include <Audio.h>

#if defined(SAFE_RELEASE)  // only defined in Dynamic Audio Objects library
#define DYNAMIC_AUDIO_AVAILABLE
#endif // defined(SAFE_RELEASE)

#if !defined(COUNT_OF)
#define COUNT_OF(x) ((sizeof x) / (sizeof x[0]))
#endif // !defined(COUNT_OF)
 

#define noOSC_DEBUG_PRINT
#if defined(OSC_DEBUG_PRINT)
#define DEBUG_SER Serial
#define OSC_SPRT(...) DEBUG_SER.print(__VA_ARGS__)
#define OSC_SPLN(...) DEBUG_SER.println(__VA_ARGS__)
#define OSC_SPTF(...) DEBUG_SER.printf(__VA_ARGS__)
#define OSC_SFSH(...) DEBUG_SER.flush(__VA_ARGS__)
#define OSC_DBGP(...) dbgPrt(__VA_ARGS__)
#else
#define OSC_SPRT(...)
#define OSC_SPLN(...)
#define OSC_SPTF(...)
#define OSC_SFSH(...)
#define OSC_DBGP(...)
#endif // defined(OSC_DEBUG_PRINT)

extern void listObjects(void);


class OSCAudioBase;
class OSCAudioGroup;
class OSCAudioConnection;

typedef struct OSCAudioTypes_s {
  const char* name;	//!< the name of the [OSC]AudioStream type
#if defined(DYNAMIC_AUDIO_AVAILABLE)
  OSCAudioBase* (*mkRoot)(const char*); //!< make object at root
  OSCAudioBase* (*mkGroup)(const char*,OSCAudioGroup&); //!< make object within group
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)
} OSCAudioTypes_t;


class OSCAudioBase
{
  public:
	friend class OSCAudioConnection;
	friend class OSCAudioGroup;
	
    OSCAudioBase(const char* _name,AudioStream* _sibling = NULL) : 
				name(NULL), sibling(_sibling), next_group(NULL), pParent(NULL)
    {
		setName(_name); 
		linkIn(); 
		OSC_SPTF("%s base created:\n",name);
		listObjects();
    }
	
	// Construct a group member
    OSCAudioBase(const char* _name,		//!< name of this object
				 OSCAudioBase& first,	//!< group to become member of
				 AudioStream* _sibling = NULL) : 
				name(NULL), sibling(_sibling), next_group(NULL), pParent(NULL)
    {
		setName(_name); 
		linkInGroup(&first);
		OSC_SPTF("%s base created; member of %s:\n",name,first.name);
		listObjects();
    }
	
    
    virtual ~OSCAudioBase() {OSC_SPTF("dtor for %s at %08X!\n",name,(uint32_t) this); OSC_SFSH(); if (NULL != name) free(name); linkOut(); }
    virtual void route(OSCMessage& msg, int addressOffset, OSCBundle&)=0;
    char* name;
    size_t nameLen;
	AudioStream* sibling;
	enum error {OK,NOT_FOUND,BLANK_NAME,DUPLICATE_NAME,NO_DYNAMIC,NO_MEMORY, // 0-5
				AMBIGUOUS_PATH,NOT_ROUTED,INVALID_METHOD,NOT_CONNECTED}; // 6-9
	static const OSCAudioTypes_t audioTypes[];
	static size_t countOfAudioTypes(void);


	/**
	 * (Re)set the name of the OSCAudio object so the system can find it.
	 * We don't allow a NULL name, as everything should have a name.
	 * If there's already enough space for a new name then we don't 
	 * re-allocate less, which may help heap fragmentation in some small way.
	 */
	void setName(const char* _name)
	{		
#define NAME_PAD 3	
		void* toFree = name;
		if (NULL != _name)
		{
			nameLen = strlen(_name);
								
			if (nameAlloc < nameLen+NAME_PAD || NULL == name)
			{
				nameAlloc = nameLen+NAME_PAD;
				name = (char*) malloc(nameAlloc); // include space for // and null terminator
			}
			else
				toFree = NULL;
			
					if (NULL != name)
					{
						name[0] = '/'; // for routing
						strcpy(name+1,_name);
					}
			
			if (NULL != toFree)
			{
				free(toFree);
			}		
		}
	}
		
		
	/**
	 * Return index of AudioStream type in the audioTypes[] array, given its name.
	 * -1 means name wasn't found.
	 */
	static int getTypeIndex(const char* name)
	{
		int objIdx;
		
		for (objIdx = OSCAudioBase::countOfAudioTypes()-1;objIdx>=0;objIdx--)
			if (0 == strcmp(OSCAudioBase::audioTypes[objIdx].name,name))
				break;
			
		return objIdx;
	}

	/**
	 * Return offset > 0 if message is directed at audio instances whose name matches ours.
	 * Subsequent calls to match() should add the returned offset to the existing
	 * addressOffset, as appropriate.
	 */
	int isMine(OSCMessage& msg, int addressOffset) {return msg.match(name,addressOffset);}
	
			
	/**
	 * Check to see if message's parameter types match those expected for the 
	 * candidate function to be called.
	 */
	static bool validParams(OSCMessage& msg,	//!< OSC message to check
						const char* types)	//!< expected parameter types: NULL imples none expected
	{
		size_t sl = 0;
		bool result;
	
		if (NULL != types)
		sl = strlen(types);
		result= (size_t) msg.size() == sl;
	
		for (size_t i=0;i<sl && result;i++)
		{
			char type = msg.getType(i);
			
			result = types[i] == type;
			if (!result && ';' == types[i]) // boolean: encoded directly in type
				result = type == 'T' || type == 'F';
		}
		
		return result;
	}


	/**
	 * Check to see if message matches expected target pattern and parameter types
	 */
	bool isTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
	{
		bool result = msg.fullMatch(pattern,addressOffset) && validParams(msg,types);
		
		return result;
	}
	
	/**
	 * Check to see if message matches expected target pattern and parameter types
	 */
	static bool isStaticTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
	{
		return msg.fullMatch(pattern,addressOffset) && validParams(msg,types);
	}
	
	
    void debugPrint(OSCMessage& msg, int addressOffset)
    {
      char prt[50];
      msg.getAddress(prt,addressOffset);

      if (NULL != name)
        Serial.println(name);
      Serial.println(addressOffset);
      Serial.println(prt);
      Serial.println(isMine(msg,addressOffset));
      Serial.println(msg.size());
      Serial.println();      
    }

	/**
	 * Route a message for the audio system to every known object.
	 */
    static void routeAll(OSCMessage& msg, 	//!< received message
						 int addressOffset,	//!< offset past the already used part of the address
						 OSCBundle& reply)	//!< bundle to hold reply
    {
      routeFrom(&first_route,msg,addressOffset,reply); 
    }


	/**
	 * Route a message for the audio system to every object linked after given starting point.
	 */
    static void routeFrom(OSCAudioBase** ppLink, //!< pointer to starting link
						 OSCMessage& msg, 	//!< received message
						 int addressOffset,	//!< offset past the already used part of the address
						 OSCBundle& reply)	//!< bundle to hold reply
    {
		while (NULL != *ppLink)
		{
			(*ppLink)->route(msg,addressOffset,reply);
			ppLink = &((*ppLink)->next_route);
		}
	}
		
		
	/**
	 * Implement a proper route and callback strategy,
	 * with a context pointer to pass to the callback function
	 * when we find a match
	 */
	static void callBack(OSCMessage& msg,
						 int offset,
						 int hitAtOffset,
						 OSCAudioBase* ooi,
						 void (*cbk)(OSCAudioBase*,OSCMessage&,int,void*),
						 void* context,
						 bool enterGroups)
	{
#if	defined(OSC_DEBUG_PRINT)
		char addr[50]; 
		msg.getAddress(addr,offset);
#endif
		while (NULL != ooi)
		{
			int o2 = msg.match(ooi->name,offset);
			OSC_SPTF("%08X (%s vs %s) ...",(uint32_t) ooi,addr,ooi->name);
			if (o2 > 0) // matched at least some
			{
				if (offset+o2 >= hitAtOffset) // should be == at target 
					cbk(ooi,msg,offset,context); // found target, do callback for it
				else // only partial match so far...
				{
					if (enterGroups) // ...if allowed...
						callBack(msg,offset+o2,hitAtOffset,ooi->getNextGroup(),cbk,context,enterGroups); // ...recurse down the groups
				}
			}
			ooi = ooi->getNext(); // chain to next object at this level
		}
		OSC_SPLN();
	}


	static void callBack(const char* addr,	//!< OSC message Address Pattern to match: NULL is safe, does nothing
						 void (*cbk)(OSCAudioBase*,OSCMessage&,int,void*), //!< function to call on match
						 void* context = NULL, //!< pointer to "context" to pass to callback function
						 OSCAudioBase* ooi = NULL, //!< Object Of Interest to start from (e.g. group)
						 bool enterGroups = true)
	{
		if (NULL != addr)
		{
			OSCMessage msg(addr);
			int hitAtOffset = strlen(addr);
			if (NULL == ooi)
				ooi = first_route;
			
			callBack(msg,0,hitAtOffset,ooi,cbk,context,enterGroups);
		}
	}
	
	// count matches to pattern
	static int hitCount(const char* addr,
						OSCAudioBase* ooi = NULL,
						bool enterGroups = true);
				
	// count matches and return last matching object
	static int findMatch(const char* addr,				//!< address to match
						OSCAudioBase** found = NULL,	//!< last-found matching object
						OSCAudioBase* ooi = NULL,		//!< where in structure to start from (default is root)
						bool enterGroups = true);		//!< whether to allow matches in sub-groups
	 
	/**
	 * Return pointer to first OSC audio object
	 */
	static OSCAudioBase* getFirst(void)	
	{
		return first_route;
	}
	
	/**
	 * Return pointer to next OSC audio object after given one
	 */
	OSCAudioBase* getNext(void)
	{
		return next_route;
	}
	
	/**
	 * Return pointer to next OSC audio group after given one
	 */
	virtual	OSCAudioBase* getNextGroup(void)
	{
		return NULL;
	}
	virtual OSCAudioConnection* getFirstSrc(void) {return NULL;}
	virtual OSCAudioConnection* getFirstDst(void) {return NULL;}
		
		
	static char* sanitise(const char* src, char* dst, int offset = 0);
	static char* trimUnderscores(const char* src, char* dst);
    static void routeDynamic(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
	// Reply mechanisms:
	void addReplyExecuted(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
	static OSCMessage& staticPrepareReplyResult(OSCMessage& msg, OSCBundle& reply);
	OSCMessage& prepareReplyResult(OSCMessage& msg, OSCBundle& reply);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, bool v, error ret = OK);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, float v);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, int32_t v);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint32_t v);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint8_t v);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint16_t v);

//protected:
	// existing objects: message passing and linking in/out
	static OSCAudioBase* first_route; //!< linked list to route OSC messages to all derived instances
	//OSCAudioBase** pFirst; 		//!< pointer back to list head
	OSCAudioBase* next_route;	//!< list of related objects
	OSCAudioBase* next_group; //!< list of unrelated objects
			
//private:
	static void renameObjectCB(OSCAudioBase* ooi,OSCMessage& msg,int offset,void* ctxt);
	static void renameObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	size_t nameAlloc;	//!< space allocated for name: may be shorter than current name
	
	// Link in and out of the routing lists
	// Link in: this occurs even if we're going to be a group member, before
	// the OSCAudioGroup is constructed
	void linkIn(OSCAudioBase** pFirst = &first_route) {next_route = *pFirst; *pFirst = this;}
	void linkInGroup(OSCAudioBase* grp);
	
	// Link out: if we're a group member then we're not on the main routing
	// list, and the ~OSCAudioGroup destructor will already have unlinked us
	void linkOut() 
	{
		if (NULL == pParent) // then we're not a group member: unlink
		{
			OSCAudioBase** ppLink = &first_route;
			
			while (NULL != *ppLink && this != *ppLink)
			{
				ppLink = &((*ppLink)->next_route);
				OSC_SPTF("%08X ... ",(uint32_t) *ppLink); OSC_SFSH();
			}
			if (NULL != ppLink)
			{
				OSC_SPTF("Unlink!\n"); OSC_SFSH();
				*ppLink = next_route;
				next_route = NULL;
			}
		}
		else
		{
			linkOutGroup(&pParent);
		}
			
	}
	void linkOutGroup(OSCAudioGroup** ppPrnt);
	
	OSCAudioGroup* pParent; //!< pointer back to ultimate parent

  protected:		
	static char* getMessageString(OSCMessage& msg, int position, void* buf, bool slashPad = false); //!< read message string into memory assigned by alloca() (probably)
	static char* getMessageAddress(OSCMessage& msg, void* buf, int len, int offset = 0); //!< read message address into memory assigned by alloca() (probably)

		
	
#if defined(DYNAMIC_AUDIO_AVAILABLE)
//============================== Dynamic Audio Objects ==================================================
    
  private:
	// dynamic audio objects:
	static void createConnection(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	static void createObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	static void createGroup(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	static void destroyObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	static void clearAllObjects(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)	
};



//============================== Dynamic Audio Objects ==================================================
// ============== AudioConnection ====================
class OSCAudioConnection : public OSCAudioBase, public AudioConnection
{
  public:
#if defined(DYNAMIC_AUDIO_AVAILABLE) // AudioConnection::AudioConnection(void) not in static library - omit these
	OSCAudioConnection(const char* _name) 
		:  OSCAudioBase(_name),
		pSrcParent(NULL),pDstParent(NULL),next_src(NULL),next_dst(NULL)  
		{}
	
	OSCAudioConnection(const char* _name,OSCAudioBase& first) 
		:  OSCAudioBase(_name, first),
		pSrcParent(NULL),pDstParent(NULL),next_src(NULL),next_dst(NULL)  
		{}
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)
		
	OSCAudioConnection(const char* _name, OSCAudioBase& src, uint8_t srcO, OSCAudioBase& dst, uint8_t dstI) 
		:  OSCAudioBase(_name),AudioConnection(*src.sibling,srcO,*dst.sibling,dstI),
		pSrcParent(NULL),pDstParent(NULL) ,next_src(NULL),next_dst(NULL) 
		{mkLinks(src,dst);}
				
	OSCAudioConnection(const char* _name, OSCAudioBase* src, uint8_t srcO, OSCAudioBase* dst, uint8_t dstI) 
		:  OSCAudioConnection(_name,*src,srcO,*dst,dstI) {}
		
	OSCAudioConnection(const char* _name, OSCAudioBase& src, OSCAudioBase& dst) 
		:  OSCAudioConnection(_name,src,0,dst,0) {}
		
	OSCAudioConnection(const char* _name, OSCAudioBase* src, OSCAudioBase* dst) 
		:  OSCAudioConnection(_name,*src,*dst) {}
		
		
	~OSCAudioConnection();

	void route(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
	// Link in and out of the source connection lists
	void linkInSrc(OSCAudioGroup* parent);		
	void linkOutSrc();
	
	// Link in and out of the destination connection lists
	void linkInDst(OSCAudioGroup* parent);		
	void linkOutDst();
	
	OSCAudioConnection* getNextSrc(void) {return next_src;}
	OSCAudioConnection* getNextDst(void) {return next_dst;}
				
  private:
	void mkLinks(OSCAudioBase& src, OSCAudioBase& dst);
	OSCAudioGroup* pSrcParent; //!< parent group of source object
	OSCAudioGroup* pDstParent; //!< parent group of destination object
	void OSCconnect(OSCMessage& msg,int addressOffset,OSCBundle& reply, bool zeroToZero = false);
	OSCAudioConnection* next_src;	//!< next in list of connections whose source is in a given group
	OSCAudioConnection* next_dst;	//!< next in list of connections whose destination is in a given group
};


//============================== Audio Object Groups ==================================================
// ============== AudioGroup ====================
class OSCAudioGroup : public OSCAudioBase
{
  public:
	OSCAudioGroup(const char* _name, OSCAudioGroup* parent = NULL);
	~OSCAudioGroup();
	virtual	OSCAudioBase* getNextGroup(void);;
	void route(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	void linkInGroup(OSCAudioGroup* parent);	
	void linkOutGroup();
	virtual OSCAudioConnection* getFirstSrc(void) {return first_src;}
	virtual OSCAudioConnection* getFirstDst(void) {return first_dst;}
		
  protected:
	friend class OSCAudioConnection;
	OSCAudioConnection* first_src;	//!< list of connections whose source is in this group
	OSCAudioConnection* first_dst;	//!< list of connections whose destination is in this group
};

	


#if defined(DYNAMIC_AUDIO_AVAILABLE)
#include <OSCAudioAutogen-dynamic.h>
#else
#include <OSCAudioAutogen-static.h>
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)

#endif // !defined(_OSCAUDIOBASE_H_)
