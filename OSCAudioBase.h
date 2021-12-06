#if !defined(_OSCAUDIOBASE_H_)
#define _OSCAUDIOBASE_H_

#include <OSCMessage.h>
#include <Audio.h>

class OSCAudioBase
{
  public:
    OSCAudioBase(const char* _name)
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
          //name[nameLen+1] = '/';
          //name[nameLen+2] = 0;
        }
      }
      linkIn(); 
    }
    ~OSCAudioBase() {if (NULL != name) free(name); linkOut(); }
    virtual void route(OSCMessage& msg, int addressOffset)=0;
    char* name;
    size_t nameLen;
    bool isMine(OSCMessage& msg, int addressOffset) {return msg.match(name,addressOffset) == (int) nameLen+1;}
    bool validParams(OSCMessage& msg,const char* types)
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

    bool isTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
    {
      bool result = msg.fullMatch(pattern,addressOffset+nameLen+1) && validParams(msg,types);

      if (result) Serial.println(name+1);
      
      return result;
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

    static void routeAll(OSCMessage& msg, int addressOffset)
    {
      OSCAudioBase** ppLink = &first_route; 
      while (NULL != *ppLink)
      {
        (*ppLink)->route(msg,addressOffset);
        ppLink = &((*ppLink)->next_route);
      }
    }
    
  private:
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
};

#include <OSCAudioAutogen.h>
#endif // !defined(_OSCAUDIOBASE_H_)
