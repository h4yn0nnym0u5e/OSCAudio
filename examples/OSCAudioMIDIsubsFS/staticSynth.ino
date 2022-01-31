#include "OSCAudioBase.h"

// Include statically-generated synth, if we can't make it dynamically
#if !defined(DYNAMIC_AUDIO_AVAILABLE)

// emulate OSC, for simplicity
#define crOb(typ,nam)    OSC##typ* nam = new OSC##typ(#nam)
#define crObG(typ,nam,grp)    OSC##typ* nam = new OSC##typ(#nam,grp)
#define crGrpRt(nam)     OSCAudioGroup* nam = new OSCAudioGroup(#nam)
#define crGrp(nam,paren) OSCAudioGroup* paren##_##nam = new OSCAudioGroup(#nam,paren)
#define crCo(nam,src,o,dst,i)    OSCAudioConnection* nam = new OSCAudioConnection(#nam,*src,o,*dst,i); (void) nam
#define crCoG(grp,nam,src,o,dst,i)    OSCAudioConnection* nam = new OSCAudioConnection(#nam,*grp,*src,o,*dst,i); (void) nam

#define VOICES 6
#define USE_DAC
#define USE_I2S
void initSynth(void)
{
  crOb(AudioMixer4,mixer1);
  crOb(AudioMixer4,mixer2);
  crOb(AudioMixer4,mixer3);
  crCo(mix1_3,mixer1,0,mixer3,0);
  crCo(mix2_3,mixer2,0,mixer3,1);
  
#if defined(USE_I2C)  
  crOb(AudioControlSGTL5000,sgtl5000);
  crOb(AudioOutputI2S,i2s);
  crCo(mix3_i2sL,mixer3,0,i2s,0);
  crCo(mix3_i2sR,mixer3,0,i2s,1);
  sgtl5000->enable();
  sgtl5000->volume(0.5f);
  sgtl5000->lineOutLevel(12);
#endif
#if defined(USE_DAC)  
  crOb(AudioOutputAnalogStereo,dac);
  crCo(mix3_dacL,mixer3,0,dac,0);
  crCo(mix3_dacR,mixer3,0,dac,1);
#endif
  
  // create grouped voices:
  crGrpRt(voice1);

  for (int i=0; i<VOICES;i++)
  {
    AudioMixer4* outMix = i>3?mixer2:mixer1;
    int inp = i % 4;
    
    // create sub-group:
    char buf[20];
    sprintf(buf,"i%d",i);
    OSCAudioGroup* pGrp = new OSCAudioGroup(buf,voice1);

    // create group members
    crObG(AudioMixer4,mixer,*pGrp);
    crObG(AudioEffectEnvelope,env,*pGrp);
    crObG(AudioSynthWaveform,wav2,*pGrp);
    crObG(AudioSynthWaveform,wav,*pGrp);

    // create connections
    crCoG(pGrp,wav_mix,wav,0,mixer,0);
    crCoG(pGrp,wav2_mix,wav2,0,mixer,1);
    crCoG(pGrp,mix_env,mixer,0,env,0);
    crCoG(pGrp,env_out,env,0,(i>3?mixer2:mixer1),inp);

    mixer->gain(0,0.5f);
    mixer->gain(1,0.5f);
    outMix->gain(i%4,0.16f);
  }
  Serial.println("Created 6-voice synth");
}
#else
void initSynth(void) {}
#endif
