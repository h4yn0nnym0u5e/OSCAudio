#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//#include "dynamic-util.h"
#include "OSCAudioBase.h"



void listObjects(OSCAudioBase* obj,int indent=0)
{
  static int depth = 0;
  depth++;
  while (depth < 5 && NULL != obj)
  {
    OSCAudioBase* grp = obj->getNextGroup();
    for (int i=0;i<indent;i++)
      Serial.print(' ');
    Serial.printf("%s is at %08X; route: %08X",obj->name,(uint32_t) obj,(uint32_t) obj->getNext()); Serial.flush();
    if (NULL != grp)
    {
      Serial.printf("; group: %08X\n",(uint32_t) grp); Serial.flush();
      listObjects(grp,indent+2);
    }
    else
      Serial.println(); Serial.flush();   
    //Serial.printf("group %08X\n",(uint32_t) (obj->getNextGroup()));
    Serial.flush();

    obj = obj->getNext();
  }
  depth--;
}


void listObjects(void)
{
  listObjects(OSCAudioBase::getFirst());
  Serial.println("--------------\n");
}


void setup() {
  Serial.begin(115200);
  while(!Serial)
    ;
    
  if (CrashReport)
  {
    Serial.println(CrashReport);
    CrashReport.clear();
  }

  //-------------------------------
  AudioMemory(50); // no idea what we'll need, so allow plenty
  //-------------------------------
  OSCAudioMixer4* mixer = new OSCAudioMixer4("mixer");
  OSCAudioSynthWaveform* wav1c = new OSCAudioSynthWaveform("wav1");
  delete mixer;  
  mixer = new OSCAudioMixer4("mixer");
  OSCAudioGroup* voice1 = new OSCAudioGroup("voice1");
    OSCAudioGroup* v1i1 = new OSCAudioGroup("i1",voice1);
    OSCAudioGroup* v1i0 = new OSCAudioGroup("i0",voice1);
    
      OSCAudioMixer4* mixer2 = new OSCAudioMixer4("mixer",*v1i0);
      OSCAudioSynthWaveform* wav1 = new OSCAudioSynthWaveform("wav1",*v1i0);
      OSCAudioSynthWaveform* wav2 = new OSCAudioSynthWaveform("wav2",*v1i0);
      
      OSCAudioMixer4* mixer2b = new OSCAudioMixer4("mixer",*v1i1);
      OSCAudioSynthWaveform* wav1b = new OSCAudioSynthWaveform("wav1",*v1i1);
      OSCAudioSynthWaveform* wav2b = new OSCAudioSynthWaveform("wav2",*v1i1);
      
  //OSCAudioSynthWaveform* wav1c = new OSCAudioSynthWaveform("wav1");
  OSCAudioOutputI2S* i2s = new OSCAudioOutputI2S("i2s");
  Serial.println("==================\nCreation complete:");
  listObjects();
  Serial.println("now delete");
  //delete mixer; 
  mixer->linkOut();
  listObjects();
  wav1c->linkOut();
  listObjects();
  wav1b->linkOut();
  listObjects();
  /*
  //mixer = new OSCAudioMixer4("mixer"); // causes a listObjects with debug code
  delete wav1c;
  listObjects();
  delete wav1b;
  listObjects();
  */
  Serial.println("done");
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
