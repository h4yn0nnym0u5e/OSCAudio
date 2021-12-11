#if !defined(_OSCAUDIOBASE_H_)
#define _OSCAUDIOBASE_H_

#include <OSCMessage.h>
#include <Audio.h>


class OSCAudioBase
{
  public:
    OSCAudioBase(const char* _name,AudioStream* _sibling = NULL) : name(NULL), sibling(_sibling)
    {
      setName(_name); 
      linkIn(); 
    }
	
    
    virtual ~OSCAudioBase() {if (NULL != name) free(name); linkOut(); }
    virtual void route(OSCMessage& msg, int addressOffset)=0;
    char* name;
    size_t nameLen;
	AudioStream* sibling;
	
	
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
						 int addressOffset)	//!< offset past the already used part of the address
    {
      OSCAudioBase** ppLink = &first_route; 
      while (NULL != *ppLink)
      {
        (*ppLink)->route(msg,addressOffset);
        ppLink = &((*ppLink)->next_route);
      }
    }


	/**
	 * Find an OSC audio object by name
	 */
    static OSCAudioBase* find(const char* _name)	//!< object to find
    {
		OSCAudioBase* result = NULL;
		OSCAudioBase** ppLink = &first_route; 
		while (NULL != *ppLink)
		{
			if (0 == strcmp(_name,(*ppLink)->name+1))
			{
				result = *ppLink;
				break;
			}
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
	
	static char* sanitise(const char* src, char* dst);
	static char* trimUnderscores(const char* src, char* dst);
    static void routeDynamic(OSCMessage& msg, int addressOffset);
	
  private:
	static void renameObject(OSCMessage& msg, int addressOffset);
	size_t nameAlloc;	//!< space allocated for name: may be shorter than current name
	// existing objects: message passing and linking in/out
    static OSCAudioBase* first_route; //!< linked list to route OSC messages to all derived instances
    OSCAudioBase* next_route;
    void linkIn() {next_route = first_route; first_route = this;}
    void linkOut() 
    {
      OSCAudioBase** ppLink = &first_route; 
      while (NULL != *ppLink && this != *ppLink)
        ppLink = &((*ppLink)->next_route);
      if (NULL != ppLink)
      {
        *ppLink = next_route;
        next_route = NULL;
      }
    }
	
#if defined(SAFE_RELEASE) // only defined in Dynamic Audio Objects library
//============================== Dynamic Audio Objects ==================================================
    
  private:
	// dynamic audio objects:
	static void createConnection(OSCMessage& msg, int addressOffset);
	static void createObject(OSCMessage& msg, int addressOffset);
	static void destroyObject(OSCMessage& msg, int addressOffset);
	static void emptyAllObjects(OSCMessage& msg, int addressOffset);
	
#endif // defined(SAFE_RELEASE)	
};


#if defined(SAFE_RELEASE) // only defined in Dynamic Audio Objects library
//============================== Dynamic Audio Objects ==================================================
// ============== AudioConnection ====================
class OSCAudioConnection : public AudioConnection, OSCAudioBase
{
    public:
        OSCAudioConnection(const char* _name) :  OSCAudioBase(_name) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/c*","ss")) {OSCconnect(msg,addressOffset,true);}
            else if (isTarget(msg,addressOffset,"/c*","sisi")) {OSCconnect(msg,addressOffset);} 
            else if (isTarget(msg,addressOffset,"/d*",NULL)) {disconnect();} 
          }
        }
		
	private:
		void OSCconnect(OSCMessage& msg,int addressOffset,bool zeroToZero = false);
};
#endif // defined(SAFE_RELEASE)	

#if defined(SAFE_RELEASE)
#include <OSCAudioAutogen-dynamic.h>
#else
#include <OSCAudioAutogen-static.h>
#endif // defined(SAFE_RELEASE)

#endif // !defined(_OSCAUDIOBASE_H_)
