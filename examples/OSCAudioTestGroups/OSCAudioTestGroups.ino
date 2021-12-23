#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

//#include "dynamic-util.h"
#include "OSCAudioBase.h"


//=============================================================
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

void listObjectsx(void){}

//=============================================================
void goFind(OSCMessage& msg,int o,int md,OSCAudioBase* ooi)
{
  int o2;

  while (NULL != ooi)
  {
    for (int i=1;i<o;i++) Serial.print(' ');
    Serial.printf("%s at %08X",ooi->name,ooi);
    o2 = msg.match(ooi->name,o);
    if (o2 > 0)
    {
      if (o2+o == md)
        Serial.println("...hit!");
      else
      {
        Serial.println();
        goFind(msg,o+o2,md,ooi->getNextGroup()); 
        Serial.println("urn"); Serial.flush();
      }
    }
    else
      Serial.println();
    ooi = ooi->getNext();
    //Serial.println((uint32_t) ooi,HEX); Serial.flush();
  }
  Serial.print("ret"); Serial.flush();
}

void goFind(char* addr)
{
  OSCMessage msg(addr);
  int md = strlen(addr);
  Serial.printf("done after %d\n",md);
  //msg.add;
  OSCAudioBase* ooi = OSCAudioBase::getFirst();

  goFind(msg,0,md,ooi);
  Serial.println("urn");
  
}

//=============================================================
void testCallBack(OSCAudioBase* ooi,OSCMessage& msg,int offset,void* context)
{
  Serial.printf("Hit! %s at %08X\n",ooi->name,ooi);  
  if (NULL != context)
    *((OSCAudioGroup**) context) = ooi;
}


//=============================================================
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
  OSCAudioEffectGranular* gran = new OSCAudioEffectGranular("gran");
    OSCAudioConnection* p2 = new OSCAudioConnection("p2",gran,mixer);
  
  OSCAudioGroup* voice1 = new OSCAudioGroup("voice1");
    OSCAudioGroup* v1i1 = new OSCAudioGroup("i1",voice1);
    OSCAudioAnalyzePrint* prtr = new OSCAudioAnalyzePrint("print",*voice1);
    OSCAudioGroup* v1i0 = new OSCAudioGroup("i0",voice1);
    
      OSCAudioMixer4* mixer2 = new OSCAudioMixer4("mixer",*v1i0);
        OSCAudioConnection* p1 = new OSCAudioConnection("p1",mixer,0,prtr,0);
      OSCAudioSynthWaveform* wav1 = new OSCAudioSynthWaveform("wav1",*v1i0);
      OSCAudioSynthWaveformModulated* wav2 = new OSCAudioSynthWaveformModulated("wav2",*v1i0);
      
      OSCAudioMixer4* mixer2b = new OSCAudioMixer4("mixer",*v1i1);
      OSCAudioSynthWaveform* wav1b = new OSCAudioSynthWaveform("wav1",*v1i1);
      OSCAudioSynthWaveformModulated* wav2b = new OSCAudioSynthWaveformModulated("wav2",*v1i1);
      
  //OSCAudioSynthWaveform* wav1c = new OSCAudioSynthWaveform("wav1");
  OSCAudioOutputI2S* i2s = new OSCAudioOutputI2S("i2s");
  Serial.println("==================\nCreation complete:");
  listObjects();
  /*
  Serial.println("now delete");
  delete mixer; // this looks OK
  //mixer->linkOut();
  listObjects();
  delete wav1c; // this seems to crash
  //wav1c->linkOut();
  listObjects();
  delete wav1b;
  delete wav2;
  delete prtr;
  delete gran;
  //wav1b->linkOut();
  listObjects();
  
  delete v1i0;
  listObjects();
  
  delete voice1;
  listObjects();
  */
  
  char* addr = "/voice1/i*/w*";
  char* grp = "/voice1/i0";
  char* wpat = "/w*";
  /*
  OSCMessage msg(addr);
  int o = 0;
  int md = strlen(addr);
  Serial.printf("done after %d\n",md);
  //msg.add;
  OSCAudioBase* ooi = OSCAudioBase::getFirst();
  Serial.println(ooi->name);
  Serial.println(o+=msg.match("/voice1",o));
  Serial.println(o+ msg.match("/i1",o));
  Serial.println(o+=msg.match("/i0",o));
  Serial.println(o+ msg.match("/wav1",o));
  Serial.println(o+ msg.match("/wav2",o));
  Serial.println(o+ msg.match("/mixer",o));
  */
  goFind(addr); // protoype of lbrary code
  
  OSCAudioBase::callBack(addr,testCallBack); // use library
  OSCAudioBase::callBack("/w*",testCallBack,NULL,NULL,false); // top-level only search

  OSCAudioGroup* i0base; // has to be group or we can't add members
  OSCAudioBase::callBack(grp,testCallBack,&i0base); // find specific group
  Serial.printf("%s found at %08X\n",grp,(uint32_t) i0base);
  OSCAudioSynthWaveformModulated* wav3 = new OSCAudioSynthWaveformModulated("wav3",*i0base); // create a new group member
  OSCAudioBase::callBack(wpat,testCallBack,NULL,i0base->getNextGroup()); // search group for "w*" objects

  Serial.printf("%d instances of %s\n",OSCAudioBase::hitCount(addr),addr);
  Serial.printf("%d instances of %s in %s\n",OSCAudioBase::hitCount(wpat,i0base->getNextGroup(),false),wpat,grp);
   
  Serial.println("done");
}


//=============================================================
void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
}
