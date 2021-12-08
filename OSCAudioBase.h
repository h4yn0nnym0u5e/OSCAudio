#if !defined(_OSCAUDIOBASE_H_)
#define _OSCAUDIOBASE_H_

#include <OSCMessage.h>
#include <Audio.h>


class OSCAudioBase
{
  public:
    OSCAudioBase(const char* _name,AudioStream* _sibling = NULL) : sibling(_sibling)
    {
      if (NULL != _name)
      {
        nameLen = strlen(_name);
        
        Serial.printf("Created %s\n\n",_name);
        
        name = (char*) malloc(nameLen+3); // include space for // and null terminator
        if (NULL != name)
        {
          name[0] = '/'; // for routing
          strcpy(name+1,_name);
        }
      }
      linkIn(); 
    }
	
    
    virtual ~OSCAudioBase() {if (NULL != name) free(name); linkOut(); }
    virtual void route(OSCMessage& msg, int addressOffset)=0;
    char* name;
    size_t nameLen;
	AudioStream* sibling;
	
	/**
	 * Check to see if message is directed at audio instances whose name matches ours
	 */
    bool isMine(OSCMessage& msg, int addressOffset) {return msg.match(name,addressOffset) == (int) nameLen+1;}
	
	
	/**
	 * Check to see if message's parameter types match those expected for the 
	 * candidate function to be called.
	 */
    static bool validParams(OSCMessage& msg,const char* types)
    {
      size_t sl = strlen(types);
      bool result = (size_t) msg.size() == sl;

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
	
  private:
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
	public:
	/**
	 * Route a message for the audio creation / destruction / connection sub-system.
	 */
    static void routeDynamic(OSCMessage& msg, int addressOffset);
    
  private:
	// dynamic audio objects:
	static void createObject(OSCMessage& msg, int addressOffset);
	static void destroyObject(OSCMessage& msg, int addressOffset);
	static void createConnection(OSCMessage& msg, int addressOffset);
			
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
