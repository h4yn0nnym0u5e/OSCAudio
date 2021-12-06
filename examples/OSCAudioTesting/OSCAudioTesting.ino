/* 
 *  Play with OSC routing. Test input comes from OSCSLIPsend.py
 *  
 *   "C:\Program Files (x86)\Arduino\hardware\tools\arm\bin\arm-none-eabi-addr2line" -e 
 */

#include <OSCMessage.h>
#include <SLIPEncodedSerial.h>


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include "OSCAudioBase.h"

// GUItool: begin automatically generated code
//AudioSynthWaveform       waveform1;      //xy=654,472
OSCAudioSynthWaveform   waveform1("waveform1");
OSCAudioMixer4          mixer1("mixer1");
AudioOutputI2S           i2s1;           //xy=977,476
AudioConnection          patchCord1(waveform1, 0, i2s1, 0);
AudioConnection          patchCord2(waveform1, 0, mixer1, 1);
AudioConnection          patchCord3(mixer1, 0, i2s1, 1);
OSCAudioControlSGTL5000     sgtl5000_1("sgtl5000");     //xy=977,519
// GUItool: end automatically generated code



#define USE_SLIP

// set this to the hardware serial port you wish to use
#define HWSERIALPORT Serial7

#if defined(USE_SLIP)
SLIPEncodedSerial HWSERIAL(HWSERIALPORT);
#else
#define HWSERIAL HWSERIALPORT
#endif // defined(USE_SLIP)

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
  AudioMemory(10);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.1);
  //-------------------------------
}


void routeAudio(OSCMessage& msg, int addressOffset)
{
  Serial.println("audio message!");
  OSCAudioBase::routeAll(msg,addressOffset);
}


// work with SLIP-protocol serial port:
void loop()
{
  OSCMessage msg;
  int msgLen;
  char prt[200];
  
  while (!HWSERIAL.endofPacket())
  {
    
    msgLen = HWSERIAL.available();
    while (msgLen--)
    {
      char c = HWSERIAL.read();
      msg.fill((uint8_t) c);
    }
  }
  Serial.println();

  if (!msg.hasError())
  {
    msg.getAddress(prt);
    
    Serial.println(prt);
    Serial.flush();
    msg.route("/teensy*/audio",routeAudio); // see if this object can use the message
    Serial.println("=====================");
  }
}
