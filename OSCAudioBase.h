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

class OSCAudioBase
{
  public:
    OSCAudioBase(const char* _name,AudioStream* _sibling = NULL) : 
				name(NULL), sibling(_sibling), next_group(NULL)
    {
      setName(_name); 
      linkIn(); 
			pFirst = &first_route;
			Serial.printf("%s base created:\n",name);
			listObjects();
    }
	
		// Construct a group member
    OSCAudioBase(const char* _name,							//!< name of this object
								 OSCAudioBase& first,						//!< group to become member of
								 AudioStream* _sibling = NULL) : 
				name(NULL), sibling(_sibling), next_group(NULL)
    {
			OSCAudioBase** pParent = &(first.next_group);
      setName(_name); 
			pFirst = pParent;
			linkInGroup(&first);
			Serial.printf("%s base created; member of %s:\n",name,first.name);
			listObjects();
    }
	
    
    virtual ~OSCAudioBase() {Serial.println("dtor!"); Serial.flush(); if (NULL != name) free(name); linkOut(); }
    virtual void route(OSCMessage& msg, int addressOffset, OSCBundle&)=0;
    char* name;
    size_t nameLen;
		AudioStream* sibling;
		enum error {OK,NOT_FOUND,BLANK_NAME,DUPLICATE_NAME,NO_DYNAMIC,NO_MEMORY};
	
	
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
				//Serial.printf("Created %s at 0x%08X\n",name,(uint32_t) name);
					}
			
			if (NULL != toFree)
			{
				//Serial.printf("now free 0x%08X...\n",(uint32_t) toFree);
				//Serial.flush();
				free(toFree);
			}		
				}
		}
	
	
		/**
		 * Check to see if message is directed at audio instances whose name matches ours
		 */
			bool isMine(OSCMessage& msg, int addressOffset) {return msg.match(name,addressOffset) == (int) nameLen+1;}
		
		
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
		 * Check to see if message matches expected target pattern and parameter types,
		 * AFTER the instance's name.
		 */
			bool isTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
			{
				bool result = msg.fullMatch(pattern,addressOffset+nameLen+1) && validParams(msg,types);
				
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
      OSCAudioBase** ppLink = &first_route; 
      while (NULL != *ppLink)
      {
        (*ppLink)->route(msg,addressOffset,reply);
        ppLink = &((*ppLink)->next_route);
      }
    }


		/**
		 * Find an OSC audio object by name
		 */
		static OSCAudioBase* find(const char* _name)	//!< object to find
		{
			return find(_name,&first_route);
		}
		
		/**
		 * Find an OSC audio object by name, 
		 * starting from a point in the connection tree
		 */
		static OSCAudioBase* find(const char* _name,			//!< object to find
															OSCAudioBase** ppLink)	//!< where to start
		{
			OSCAudioBase* result = NULL;
			
			while (NULL != *ppLink)
			{
				// first check to see if this object matches
				if (0 == strcmp(_name,(*ppLink)->name+1))
				{
					result = *ppLink;
					break;
				}
				
				// now look in our group, if any
				result = find(_name,&((*ppLink)->next_group));
				if (NULL != result)
					break;
				
				// not found, go on down the list
				ppLink = &((*ppLink)->next_route);
			}
			
			return result;			
		}

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
		
		
		static char* sanitise(const char* src, char* dst);
		static char* trimUnderscores(const char* src, char* dst);
    static void routeDynamic(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
		// Reply mechanisms:
		void addReplyExecuted(OSCMessage& msg, int addressOffset, OSCBundle& reply);
		
		static OSCMessage& staticPrepareReplyResult(OSCMessage& msg, OSCBundle& reply);
		OSCMessage& prepareReplyResult(OSCMessage& msg, OSCBundle& reply);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, bool v);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, float v);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, int32_t v);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint32_t v);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint8_t v);
		void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint16_t v);

	//protected:
		friend class OSCAudioGroup;
		// existing objects: message passing and linking in/out
		static OSCAudioBase* first_route; //!< linked list to route OSC messages to all derived instances
		OSCAudioBase** pFirst; 		//!< pointer back to list head
		OSCAudioBase* next_route;	//!< list of related objects
		OSCAudioBase* next_group; //!< list of unrelated objects
			
  //private:
		static void renameObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
		size_t nameAlloc;	//!< space allocated for name: may be shorter than current name
		
		// Link in and out of the routing lists
		void linkIn(OSCAudioBase** pFirst = &first_route) {next_route = *pFirst; *pFirst = this;}
		void linkOut() 
		{
			OSCAudioBase** ppLink = pFirst; 
			while (NULL != *ppLink && this != *ppLink)
			{
				ppLink = &((*ppLink)->next_route);
				Serial.printf("%08X ... ",(uint32_t) *ppLink); Serial.flush();
			}
			if (NULL != ppLink)
			{
				Serial.printf("Unlink!\n"); Serial.flush();
				*ppLink = next_route;
				next_route = NULL;
			}
		}
		
		OSCAudioBase** pParent; //!< pointer back to next_group of ultimate parent
		
		// Link in and out of the grouping lists
		void linkInGroup(OSCAudioBase* parent) 
		{
			if (NULL != parent)
			{
				next_route = parent->next_group; 
				parent->next_group = this;
				pParent = &(parent->next_group);
			}
		}
		
		void linkOutGroup() 
		{
			OSCAudioBase** ppLink = pParent; 
			while (NULL != *ppLink && this != *ppLink)
				ppLink = &((*ppLink)->next_group);
			if (NULL != ppLink)
			{
				*ppLink = next_group;
				next_group = NULL;
			}
		}
		
	
#if defined(DYNAMIC_AUDIO_AVAILABLE)
//============================== Dynamic Audio Objects ==================================================
    
  private:
		// dynamic audio objects:
		static void createConnection(OSCMessage& msg, int addressOffset, OSCBundle& reply);
		static void createObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
		static void destroyObject(OSCMessage& msg, int addressOffset, OSCBundle& reply);
		static void clearAllObjects(OSCMessage& msg, int addressOffset, OSCBundle& reply);
	
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)	
};


#if defined(DYNAMIC_AUDIO_AVAILABLE) 
//============================== Dynamic Audio Objects ==================================================
// ============== AudioConnection ====================
class OSCAudioConnection : public AudioConnection, OSCAudioBase
{
  public:
		OSCAudioConnection(const char* _name) :  OSCAudioBase(_name) {}

		void route(OSCMessage& msg, int addressOffset, OSCBundle& reply)
		{
			if (isMine(msg,addressOffset))
			{ 
				if (isTarget(msg,addressOffset,"/c*","ss")) {OSCconnect(msg,addressOffset,reply,true);}
				else if (isTarget(msg,addressOffset,"/c*","sisi")) {OSCconnect(msg,addressOffset,reply);} 
				else if (isTarget(msg,addressOffset,"/d*",NULL)) {disconnect();} 
			}
		}
		
	private:
		void OSCconnect(OSCMessage& msg,int addressOffset,OSCBundle& reply, bool zeroToZero = false);
};
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)	

//============================== Audio Object Groups ==================================================
// ============== AudioGroup ====================
class OSCAudioGroup : public OSCAudioBase
{
	public:
		OSCAudioGroup(const char* _name, OSCAudioGroup* parent = NULL) :  OSCAudioBase(_name) 
		{
			if (NULL != parent)
			{
				linkOut();
				linkInGroup(parent);
				pFirst = &(parent->next_group);
				Serial.printf("%s group re-linked:\n",name);
				listObjects();
			}
		}

		/**
		 * Return pointer to next OSC audio group after given one
		 */
		virtual	OSCAudioBase* getNextGroup(void)
		{
			return next_group;
		}
		
		void route(OSCMessage& msg, int addressOffset, OSCBundle& reply)
		{
			if (isMine(msg,addressOffset))
			{ 
			}
		}		
};

#if defined(DYNAMIC_AUDIO_AVAILABLE)
#include <OSCAudioAutogen-dynamic.h>
#else
#include <OSCAudioAutogen-static.h>
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)

#endif // !defined(_OSCAUDIOBASE_H_)
