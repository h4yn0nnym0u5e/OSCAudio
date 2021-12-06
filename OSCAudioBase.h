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


class OSCAudioSynthWaveform : public AudioSynthWaveform, OSCAudioBase
{
  public:
    OSCAudioSynthWaveform(const char* _name) : OSCAudioBase(_name) {}

    void route(OSCMessage& msg, int addressOffset)
    {
      if (isMine(msg,addressOffset))
      {
        //debugPrint(msg,addressOffset+nameLen+1);
        // Can't use msg.route() here because the callback has to be static, and we'd then
        // lose knowledge of the instance.
        //
        // To permit shorter message addresses, we allow shortening of the member function
        // to any point that is still unique
        if (isTarget(msg,addressOffset,"/am*","f")) {amplitude(msg.getFloat(0));} 
        if (isTarget(msg,addressOffset,"/ar*","bf")) {OSCarbitraryWaveform(msg,addressOffset+nameLen+1);} 
        if (isTarget(msg,addressOffset,"/b*","ffi")) {begin(msg.getFloat(0),msg.getFloat(1),msg.getInt(2));}         
        if (isTarget(msg,addressOffset,"/b*","i")) {begin(msg.getInt(0));}         
        if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} 
        if (isTarget(msg,addressOffset,"/o*","f")) {offset(msg.getFloat(0));} 
        if (isTarget(msg,addressOffset,"/ph*","f")) {phase(msg.getFloat(0));} 
        if (isTarget(msg,addressOffset,"/pu*","f")) {pulseWidth(msg.getFloat(0));} 
      }
    }
  private:
    void OSCarbitraryWaveform(OSCMessage& msg, int addressOffset) {debugPrint(msg,addressOffset);}
};


class OSCAudioMixer4 : public AudioMixer4, OSCAudioBase
{
  public:
    OSCAudioMixer4(const char* _name) : OSCAudioBase(_name) {}

    void route(OSCMessage& msg, int addressOffset)
    {
      if (isMine(msg,addressOffset))
      {
        if (isTarget(msg,addressOffset,"/g*","if")) {gain(msg.getInt(0),msg.getFloat(1));} 
      }
    }
};
#endif // !defined(_OSCAUDIOBASE_H_)
