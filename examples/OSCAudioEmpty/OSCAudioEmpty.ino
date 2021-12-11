/* 
 *  Empty audio design for use with Audio Design Tool++
 *  
 *   "C:\Program Files (x86)\Arduino\hardware\tools\arm\bin\arm-none-eabi-addr2line" -e 
 */

#include <OSCBundle.h>
#include <SLIPEncodedSerial.h>


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "OSCAudioBase.h"

// set this to the hardware serial port you wish to use
#define HWSERIALPORT Serial7

SLIPEncodedSerial HWSERIAL(HWSERIALPORT);

void setup() {
	Serial.begin(115200);
  while(!Serial)
    ;

  if (CrashReport)
  {
    Serial.println(CrashReport);
    CrashReport.clear();
  }
	HWSERIAL.begin(115200);
  HWSERIAL.setTimeout(100);

  //-------------------------------
  AudioMemory(50); // no idea what we'll need, so allow plenty
  //-------------------------------
  //testSanitise();
  listObjects();
}


void routeAudio(OSCMessage& msg, int addressOffset)
{
  Serial.println("audio message!");
  OSCAudioBase::routeAll(msg,addressOffset);
}


void routeDynamic(OSCMessage& msg, int addressOffset)
{
#if defined(SAFE_RELEASE)  
  Serial.println("dynamic objects message!");
  OSCAudioBase::routeDynamic(msg,addressOffset);
#else
  Serial.println("dynamic objects not available!");
#endif // defined(SAFE_RELEASE)  
}


void listObjects(void)
{
  OSCAudioBase* obj=OSCAudioBase::getFirst();

  while (NULL != obj)
  {
    Serial.printf("%s is at %08X\n",obj->name,(uint32_t) obj);
    Serial.flush();
    obj = obj->getNext();
  }
}


void testSanitise()
{
  char buf[20];
  OSCAudioBase::sanitise("Hello world!",buf); Serial.println(buf);  
  OSCAudioBase::sanitise("[Hello world!]",buf); Serial.println(buf);  
  OSCAudioBase::trimUnderscores(buf,buf); Serial.println(buf);  
  OSCAudioBase::sanitise("*[H]e{}llo#, world!?/",buf); Serial.println(buf);  
  OSCAudioBase::trimUnderscores(buf,buf); Serial.println(buf);  
  OSCAudioBase::sanitise("#*,/? []{}",buf); Serial.println(buf);  
  OSCAudioBase::trimUnderscores(buf,buf); Serial.printf("<%s>\n",buf);  
}


void processMessage(OSCMessage* msg)
{
  char prt[200];
  
  if (!msg->hasError())
  {
    msg->getAddress(prt);  
    Serial.println(prt);
    Serial.flush();
  
    msg->route("/teensy*/audio",routeAudio); // see if this object can use the message
    msg->route("/teensy*/dynamic",routeDynamic); // see if this object can use the message
  }
  else
    Serial.println("error in msg");
}


// work with SLIP-protocol serial port:
void loop()
{
  OSCBundle bndl;
  OSCMessage msg;
  char firstCh = 0;
  int msgLen;
  
  while (!HWSERIAL.endofPacket())
  {
    
    msgLen = HWSERIAL.available();
    while (msgLen--)
    {
      char c = HWSERIAL.read();
      // figure out if it's a message or a bundle
      if (0 == firstCh)
        firstCh = c;
      if ('#' == firstCh)
        bndl.fill((uint8_t) c); // simple messages should result in a 1-message "bundle", but don't
      else
        msg.fill((uint8_t) c); // so process them specifically
    }
  }

  if ('#' == firstCh)
  {
    if (!bndl.hasError())  
    {
      int bndlSize = bndl.size();
      
      for (int i=0;i<bndlSize;i++)
      {
        OSCMessage* msg = bndl.getOSCMessage(i); 
        Serial.printf("Message %d\n",i);
        processMessage(msg);   
      }  
    }
    else
    {
      Serial.printf("error %d in bundle\n",(int) bndl.getError());
      int bndlSize = bndl.size();
      
      for (int i=0;i<bndlSize;i++)
      {
        OSCMessage* msg = bndl.getOSCMessage(i); 
        Serial.printf("error %d in message %d\n",(int) msg->getError(),i);
      }
    }
    listObjects();
  }
  else 
  {
    if ('/' == firstCh) 
    {
      processMessage(&msg);   
      listObjects();
    }
  }
  Serial.println();
}
