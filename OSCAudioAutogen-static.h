// ============== AsyncAudioInputSPDIF3 ====================
class AsyncOSCAudioInputSPDIF3 : public AsyncAudioInputSPDIF3, OSCAudioBase
{
    public:
        AsyncOSCAudioInputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/b*",NULL)) {begin();} // void begin();
            else if (isTarget(msg,addressOffset,"/getA*",NULL)) {getAttenuation();} // double getAttenuation() const;
            else if (isTarget(msg,addressOffset,"/getB*",NULL)) {getBufferedTime();} // double getBufferedTime() const;
            else if (isTarget(msg,addressOffset,"/getH*",NULL)) {getHalfFilterLength();} // int32_t getHalfFilterLength() const;
            else if (isTarget(msg,addressOffset,"/getI*",NULL)) {getInputFrequency();} // double getInputFrequency() const;
            else if (isTarget(msg,addressOffset,"/getT*",NULL)) {getTargetLantency();} // double getTargetLantency() const;
            else if (isTarget(msg,addressOffset,"/i*",NULL)) {isLocked();} // static bool isLocked();
          }
        }
};

// ============== AudioAmplifier ====================
class OSCAudioAmplifier : public AudioAmplifier, OSCAudioBase
{
    public:
        OSCAudioAmplifier(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/g*","f")) {gain(msg.getFloat(0));} // void gain(float n)
            //else if (isTarget(msg,addressOffset,"/s*",";")) {slew(msg.getBoolean(0));} // void slew(bool doSlew)
          }
        }
};
/*
// ============== AudioAnalyzeEvent ====================
class OSCAudioAnalyzeEvent : public AudioAnalyzeEvent, OSCAudioBase
{
    public:
        OSCAudioAnalyzeEvent(const char* _name) :  OSCAudioBase(_name) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/getC*",NULL)) {getCount();} // uint32_t getCount(void) {return count;}
            else if (isTarget(msg,addressOffset,"/getM*",NULL)) {getMicros();} // uint32_t getMicros(void) {return tstamp;}
            // else if (isTarget(msg,addressOffset,"/s*","b")) {setEventFn(msg.getBlob(1));} // void setEventFn(EventResponderFunction evFn,void* context);
          }
        }
};
*/
// ============== AudioAnalyzeFFT1024 ====================
class OSCAudioAnalyzeFFT1024 : public AudioAnalyzeFFT1024, OSCAudioBase
{
    public:
        OSCAudioAnalyzeFFT1024(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/ava*",NULL)) {available();} // bool available() {
            else if (isTarget(msg,addressOffset,"/ave*","i")) {averageTogether(msg.getInt(0));} // void averageTogether(uint8_t n) {
            else if (isTarget(msg,addressOffset,"/r*","ii")) {read(msg.getInt(0),msg.getInt(1));} // float read(unsigned int binFirst, unsigned int binLast) {
            else if (isTarget(msg,addressOffset,"/r*","i")) {read(msg.getInt(0));} // float read(unsigned int binNumber) {
            // else if (isTarget(msg,addressOffset,"/w*","b")) {windowFunction(msg.getBlob(0));} // void windowFunction(const int16_t *w) {
          }
        }
};

// ============== AudioAnalyzeFFT256 ====================
class OSCAudioAnalyzeFFT256 : public AudioAnalyzeFFT256, OSCAudioBase
{
    public:
        OSCAudioAnalyzeFFT256(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/ava*",NULL)) {available();} // bool available() {
            else if (isTarget(msg,addressOffset,"/ave*","i")) {averageTogether(msg.getInt(0));} // void averageTogether(uint8_t n) {
            else if (isTarget(msg,addressOffset,"/r*","ii")) {read(msg.getInt(0),msg.getInt(1));} // float read(unsigned int binFirst, unsigned int binLast) {
            else if (isTarget(msg,addressOffset,"/r*","i")) {read(msg.getInt(0));} // float read(unsigned int binNumber) {
            // else if (isTarget(msg,addressOffset,"/w*","b")) {windowFunction(msg.getBlob(0));} // void windowFunction(const int16_t *w) {
          }
        }
};

// ============== AudioAnalyzeNoteFrequency ====================
class OSCAudioAnalyzeNoteFrequency : public AudioAnalyzeNoteFrequency, OSCAudioBase
{
    public:
        OSCAudioAnalyzeNoteFrequency(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // bool available( void );
            else if (isTarget(msg,addressOffset,"/b*","f")) {begin(msg.getFloat(0));} // void begin( float threshold );
            else if (isTarget(msg,addressOffset,"/p*",NULL)) {probability();} // float probability( void );
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {read();} // float read( void );
            else if (isTarget(msg,addressOffset,"/t*","f")) {threshold(msg.getFloat(0));} // void threshold( float p );
          }
        }
};

// ============== AudioAnalyzePeak ====================
class OSCAudioAnalyzePeak : public AudioAnalyzePeak, OSCAudioBase
{
    public:
        OSCAudioAnalyzePeak(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // bool available(void) {
            else if (isTarget(msg,addressOffset,"/readP*",NULL)) {readPeakToPeak();} // float readPeakToPeak(void) {
            else if (isTarget(msg,addressOffset,"/read",NULL)) {read();} // float read(void) {
          }
        }
};

// ============== AudioAnalyzePrint ====================
class OSCAudioAnalyzePrint : public AudioAnalyzePrint, OSCAudioBase
{
    public:
        OSCAudioAnalyzePrint(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*","i")) {delay(msg.getInt(0));} // void delay(uint32_t num) { delay_length = num; }
            else if (isTarget(msg,addressOffset,"/l*","i")) {length(msg.getInt(0));} // void length(uint32_t num) { print_length = num; }
            // else if (isTarget(msg,addressOffset,"/n*","s")) {name(msg.getString(0));} // void name(const char *str) { myname = str; }
            // NOT DEFINED: else if (isTarget(msg,addressOffset,"/t*","fi")) {trigger(msg.getFloat(0),msg.getInt(1));} // void trigger(float level, int edge);
            else if (isTarget(msg,addressOffset,"/t*",NULL)) {trigger();} // void trigger(void);
          }
        }
};

// ============== AudioAnalyzeRMS ====================
class OSCAudioAnalyzeRMS : public AudioAnalyzeRMS, OSCAudioBase
{
    public:
        OSCAudioAnalyzeRMS(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // bool available(void) {
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {read();} // float read(void);
          }
        }
};

// ============== AudioAnalyzeToneDetect ====================
class OSCAudioAnalyzeToneDetect : public AudioAnalyzeToneDetect, OSCAudioBase
{
    public:
        OSCAudioAnalyzeToneDetect(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // bool available(void) {
            else if (isTarget(msg,addressOffset,"/f*","fi")) {frequency(msg.getFloat(0),msg.getInt(1));} // void frequency(float freq, uint16_t cycles=10) {
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {read();} // float read(void);
            else if (isTarget(msg,addressOffset,"/s*","iii")) {set_params(msg.getInt(0),msg.getInt(1),msg.getInt(2));} // void set_params(int32_t coef, uint16_t cycles, uint16_t len);
            else if (isTarget(msg,addressOffset,"/t*","f")) {threshold(msg.getFloat(0));} // void threshold(float level) {
          }
        }
};

// ============== AudioControlAK4558 ====================
class OSCAudioControlAK4558 : public AudioControlAK4558, OSCAudioBase
{
    public:
        OSCAudioControlAK4558(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/disableI*",NULL)) {disableIn();} // bool disableIn(void);	//powers down ADC
            else if (isTarget(msg,addressOffset,"/disableO*",NULL)) {disableOut();} // bool disableOut(void);	//powers down DAC
            else if (isTarget(msg,addressOffset,"/disable",NULL)) {disable();} // bool disable(void) { return (disableIn()&&disableOut()); }	//powers down ADC/DAC
            else if (isTarget(msg,addressOffset,"/enableI*",NULL)) {enableIn();} // bool enableIn(void);	//powers up ADC
            else if (isTarget(msg,addressOffset,"/enableO*",NULL)) {enableOut();} // bool enableOut(void);	//powers up DAC
            else if (isTarget(msg,addressOffset,"/enable",NULL)) {enable();} // bool enable(void);		//enables the CODEC, does not power up ADC nor DAC (use enableIn() and enableOut() for selective power up)
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float n) { return false; }	//not supported by AK4558
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n) { return false; }	//sets inputs to mono left, mono right, stereo (default stereo), not yet implemented
            else if (isTarget(msg,addressOffset,"/volumeL*","f")) {volumeLeft(msg.getFloat(0));} // bool volumeLeft(float n);	//sets LOUT volume to n (range 0.0 - 1.0)
            else if (isTarget(msg,addressOffset,"/volumeR*","f")) {volumeRight(msg.getFloat(0));} // bool volumeRight(float n);	//sets ROUT volume to n (range 0.0 - 1.0)
            else if (isTarget(msg,addressOffset,"/volume","f")) {volume(msg.getFloat(0));} // bool volume(float n);	//sets LOUT/ROUT volume to n (range 0.0 - 1.0)
          }
        }
};

// ============== AudioControlCS42448 ====================
class OSCAudioControlCS42448 : public AudioControlCS42448, OSCAudioBase
{
    public:
        OSCAudioControlCS42448(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*",NULL)) {disable();} // bool disable(void) {
            else if (isTarget(msg,addressOffset,"/e*",NULL)) {enable();} // bool enable(void);
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float level) {
            else if (isTarget(msg,addressOffset,"/inputL*","if")) {inputLevel(msg.getInt(0),msg.getFloat(1));} // bool inputLevel(int channel, float level) {
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n) {
            else if (isTarget(msg,addressOffset,"/s*","i")) {setAddress(msg.getInt(0));} // void setAddress(uint8_t addr) {
            else if (isTarget(msg,addressOffset,"/v*","f")) {volume(msg.getFloat(0));} // bool volume(float level) {
            else if (isTarget(msg,addressOffset,"/v*","if")) {volume(msg.getInt(0),msg.getFloat(1));} // bool volume(int channel, float level) {
          }
        }
};

// ============== AudioControlCS4272 ====================
class OSCAudioControlCS4272 : public AudioControlCS4272, OSCAudioBase
{
    public:
        OSCAudioControlCS4272(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/da*","ff")) {dacVolume(msg.getFloat(0),msg.getFloat(1));} // bool dacVolume(float left, float right);
            else if (isTarget(msg,addressOffset,"/da*","f")) {dacVolume(msg.getFloat(0));} // bool dacVolume(float n) { return volumeInteger(n * 127 + 0.499f); }
            else if (isTarget(msg,addressOffset,"/disableD*",NULL)) {disableDither();} // bool disableDither(void);
            else if (isTarget(msg,addressOffset,"/disable",NULL)) {disable();} // bool disable(void) { return false; }
            else if (isTarget(msg,addressOffset,"/enableD*",NULL)) {enableDither();} // bool enableDither(void);
            else if (isTarget(msg,addressOffset,"/enable",NULL)) {enable();} // bool enable(void);
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float n) { return false; }
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n) { return false; }
            else if (isTarget(msg,addressOffset,"/muteI*",NULL)) {muteInput();} // bool muteInput(void);
            else if (isTarget(msg,addressOffset,"/muteO*",NULL)) {muteOutput();} // bool muteOutput(void);
            else if (isTarget(msg,addressOffset,"/unmuteI*",NULL)) {unmuteInput();} // bool unmuteInput(void);
            else if (isTarget(msg,addressOffset,"/unmuteO*",NULL)) {unmuteOutput();} // bool unmuteOutput(void);
            else if (isTarget(msg,addressOffset,"/v*","ff")) {volume(msg.getFloat(0),msg.getFloat(1));} // bool volume(float left, float right);
            else if (isTarget(msg,addressOffset,"/v*","f")) {volume(msg.getFloat(0));} // bool volume(float n) { return volumeInteger(n * 127 + 0.499f); }
          }
        }
};

// ============== AudioControlSGTL5000 ====================
class OSCAudioControlSGTL5000 : public AudioControlSGTL5000, OSCAudioBase
{
    public:
        OSCAudioControlSGTL5000(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/adcHighPassFilterD*",NULL)) {adcHighPassFilterDisable();} // unsigned short adcHighPassFilterDisable(void);
            else if (isTarget(msg,addressOffset,"/adcHighPassFilterE*",NULL)) {adcHighPassFilterEnable();} // unsigned short adcHighPassFilterEnable(void);
            else if (isTarget(msg,addressOffset,"/adcHighPassFilterF*",NULL)) {adcHighPassFilterFreeze();} // unsigned short adcHighPassFilterFreeze(void);
            else if (isTarget(msg,addressOffset,"/audioPo*",NULL)) {audioPostProcessorEnable();} // unsigned short audioPostProcessorEnable(void);
            else if (isTarget(msg,addressOffset,"/audioPre*",NULL)) {audioPreProcessorEnable();} // unsigned short audioPreProcessorEnable(void);
            else if (isTarget(msg,addressOffset,"/audioPro*",NULL)) {audioProcessorDisable();} // unsigned short audioProcessorDisable(void);
            else if (isTarget(msg,addressOffset,"/autoVolumeC*","iiifff")) {autoVolumeControl(msg.getInt(0),msg.getInt(1),msg.getInt(2),msg.getFloat(3),msg.getFloat(4),msg.getFloat(5));} // unsigned short autoVolumeControl(uint8_t maxGain, uint8_t lbiResponse, uint8_t hardLimit, float threshold, float attack, float decay);
            else if (isTarget(msg,addressOffset,"/autoVolumeD*",NULL)) {autoVolumeDisable();} // unsigned short autoVolumeDisable(void);
            else if (isTarget(msg,addressOffset,"/autoVolumeE*",NULL)) {autoVolumeEnable();} // unsigned short autoVolumeEnable(void);
            else if (isTarget(msg,addressOffset,"/dacVolumeRampD*",NULL)) {dacVolumeRampDisable();} // bool dacVolumeRampDisable();
            else if (isTarget(msg,addressOffset,"/dacVolumeRampL*",NULL)) {dacVolumeRampLinear();} // bool dacVolumeRampLinear();
            else if (isTarget(msg,addressOffset,"/dacVolumeRamp",NULL)) {dacVolumeRamp();} // bool dacVolumeRamp();
            else if (isTarget(msg,addressOffset,"/dacVolume","ff")) {dacVolume(msg.getFloat(0),msg.getFloat(1));} // unsigned short dacVolume(float left, float right);
            else if (isTarget(msg,addressOffset,"/dacVolume","f")) {dacVolume(msg.getFloat(0));} // unsigned short dacVolume(float n);
            else if (isTarget(msg,addressOffset,"/di*",NULL)) {disable();} // bool disable(void) { return false; }
            // else if (isTarget(msg,addressOffset,"/ena*","b")) {enable(msg.getBlob(1));} // bool enable(const unsigned extMCLK, const uint32_t pllFreq = (4096.0l * AUDIO_SAMPLE_RATE_EXACT) ); //With extMCLK > 0, the SGTL acts as Master
            else if (isTarget(msg,addressOffset,"/ena*",NULL)) {enable();} // bool enable(void);//For Teensy LC the SGTL acts as master, for all other Teensys as slave.
            else if (isTarget(msg,addressOffset,"/enhanceBassD*",NULL)) {enhanceBassDisable();} // unsigned short enhanceBassDisable(void);
            else if (isTarget(msg,addressOffset,"/enhanceBassE*",NULL)) {enhanceBassEnable();} // unsigned short enhanceBassEnable(void);
            else if (isTarget(msg,addressOffset,"/enhanceBass","ff")) {enhanceBass(msg.getFloat(0),msg.getFloat(1));} // unsigned short enhanceBass(float lr_lev, float bass_lev);
            else if (isTarget(msg,addressOffset,"/enhanceBass","ffii")) {enhanceBass(msg.getFloat(0),msg.getFloat(1),msg.getInt(2),msg.getInt(3));} // unsigned short enhanceBass(float lr_lev, float bass_lev, uint8_t hpf_bypass, uint8_t cutoff);
            else if (isTarget(msg,addressOffset,"/eqBands","fffff")) {eqBands(msg.getFloat(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3),msg.getFloat(4));} // void eqBands(float bass, float mid_bass, float midrange, float mid_treble, float treble);
            else if (isTarget(msg,addressOffset,"/eqBands","ff")) {eqBands(msg.getFloat(0),msg.getFloat(1));} // void eqBands(float bass, float treble);
            else if (isTarget(msg,addressOffset,"/eqBand","if")) {eqBand(msg.getInt(0),msg.getFloat(1));} // unsigned short eqBand(uint8_t bandNum, float n);
            else if (isTarget(msg,addressOffset,"/eqFilterC*","i")) {eqFilterCount(msg.getInt(0));} // unsigned short eqFilterCount(uint8_t n);
            // else if (isTarget(msg,addressOffset,"/eqFilter","ib")) {eqFilter(msg.getInt(0),msg.getBlob(1));} // void eqFilter(uint8_t filterNum, int *filterParameters);
            else if (isTarget(msg,addressOffset,"/eqS*","i")) {eqSelect(msg.getInt(0));} // unsigned short eqSelect(uint8_t n);
            else if (isTarget(msg,addressOffset,"/h*","i")) {headphoneSelect(msg.getInt(0));} // bool headphoneSelect(int n) {
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float n) {return false;}
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n) {
            else if (isTarget(msg,addressOffset,"/k*",NULL)) {killAutomation();} // void killAutomation(void) { semi_automated=false; }
            else if (isTarget(msg,addressOffset,"/lineI*","ii")) {lineInLevel(msg.getInt(0),msg.getInt(1));} // bool lineInLevel(uint8_t left, uint8_t right);
            else if (isTarget(msg,addressOffset,"/lineI*","i")) {lineInLevel(msg.getInt(0));} // bool lineInLevel(uint8_t n) { return lineInLevel(n, n); }
            else if (isTarget(msg,addressOffset,"/lineO*","ii")) {lineOutLevel(msg.getInt(0),msg.getInt(1));} // unsigned short lineOutLevel(uint8_t left, uint8_t right);
            else if (isTarget(msg,addressOffset,"/lineO*","i")) {lineOutLevel(msg.getInt(0));} // unsigned short lineOutLevel(uint8_t n);
            else if (isTarget(msg,addressOffset,"/mi*","i")) {micGain(msg.getInt(0));} // bool micGain(unsigned int dB);
            else if (isTarget(msg,addressOffset,"/muteH*",NULL)) {muteHeadphone();} // bool muteHeadphone(void) { return write(0x0024, ana_ctrl | (1<<4)); }
            else if (isTarget(msg,addressOffset,"/muteL*",NULL)) {muteLineout();} // bool muteLineout(void) { return write(0x0024, ana_ctrl | (1<<8)); }
            else if (isTarget(msg,addressOffset,"/se*","i")) {setAddress(msg.getInt(0));} // void setAddress(uint8_t level);
            else if (isTarget(msg,addressOffset,"/surroundSoundD*",NULL)) {surroundSoundDisable();} // unsigned short surroundSoundDisable(void);
            else if (isTarget(msg,addressOffset,"/surroundSoundE*",NULL)) {surroundSoundEnable();} // unsigned short surroundSoundEnable(void);
            else if (isTarget(msg,addressOffset,"/surroundSound","i")) {surroundSound(msg.getInt(0));} // unsigned short surroundSound(uint8_t width);
            else if (isTarget(msg,addressOffset,"/surroundSound","ii")) {surroundSound(msg.getInt(0),msg.getInt(1));} // unsigned short surroundSound(uint8_t width, uint8_t select);
            else if (isTarget(msg,addressOffset,"/unmuteH*",NULL)) {unmuteHeadphone();} // bool unmuteHeadphone(void) { return write(0x0024, ana_ctrl & ~(1<<4)); }
            else if (isTarget(msg,addressOffset,"/unmuteL*",NULL)) {unmuteLineout();} // bool unmuteLineout(void) { return write(0x0024, ana_ctrl & ~(1<<8)); }
            else if (isTarget(msg,addressOffset,"/v*","ff")) {volume(msg.getFloat(0),msg.getFloat(1));} // bool volume(float left, float right);
            else if (isTarget(msg,addressOffset,"/v*","f")) {volume(msg.getFloat(0));} // bool volume(float n) { return volumeInteger(n * 129 + 0.499f); }
          }
        }
};

// ============== AudioControlTLV320AIC3206 ====================
class OSCAudioControlTLV320AIC3206 : public AudioControlTLV320AIC3206, OSCAudioBase
{
    public:
        OSCAudioControlTLV320AIC3206(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/aic_r*","ii")) {aic_readPage(msg.getInt(0),msg.getInt(1));} // unsigned int aic_readPage(uint8_t page, uint8_t reg);
            else if (isTarget(msg,addressOffset,"/aic_w*","iii")) {aic_writePage(msg.getInt(0),msg.getInt(1),msg.getInt(2));} // bool aic_writePage(uint8_t page, uint8_t reg, uint8_t val);
            else if (isTarget(msg,addressOffset,"/d*",NULL)) {disable();} // bool disable(void);
            else if (isTarget(msg,addressOffset,"/enableA*",";i")) {enableAutoMuteDAC(msg.getBoolean(0),msg.getInt(1));} // bool enableAutoMuteDAC(bool, uint8_t);
            else if (isTarget(msg,addressOffset,"/enableM*",";")) {enableMicDetect(msg.getBoolean(0));} // bool enableMicDetect(bool);
            else if (isTarget(msg,addressOffset,"/enable",NULL)) {enable();} // bool enable(void);
            else if (isTarget(msg,addressOffset,"/getH*",NULL)) {getHPCutoff_Hz();} // float getHPCutoff_Hz(void) { return HP_cutoff_Hz; }
            else if (isTarget(msg,addressOffset,"/getS*",NULL)) {getSampleRate_Hz();} // float getSampleRate_Hz(void) { return sample_rate_Hz; }
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float n);  //dummy to be compatible with Teensy Audio Library
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n);   //use AIC3206_INPUT_IN1 or one of other choices defined earlier
            else if (isTarget(msg,addressOffset,"/o*","i")) {outputSelect(msg.getInt(0));} // bool outputSelect(int n);  //use AIC3206_OUTPUT_HEADPHONE_JACK_OUT or one of other choices defined earlier
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {readMicDetect();} // int  readMicDetect(void);
            else if (isTarget(msg,addressOffset,"/setH*",";ff")) {setHPFonADC(msg.getBoolean(0),msg.getFloat(1),msg.getFloat(2));} // void setHPFonADC(bool enable, float cutoff_Hz, float fs_Hz);
            // else if (isTarget(msg,addressOffset,"/setII*","ib")) {setIIRCoeffOnADC(msg.getInt(0),msg.getBlob(1));} // void setIIRCoeffOnADC(int chan, uint32_t *coeff);  //for chan, use AIC3206_BOTH_CHAN or AIC3206_LEFT_CHAN or AIC3206_RIGHT_CHAN
            else if (isTarget(msg,addressOffset,"/setIn*","f")) {setInputGain_dB(msg.getFloat(0));} // bool setInputGain_dB(float n);
            else if (isTarget(msg,addressOffset,"/setM*","i")) {setMicBias(msg.getInt(0));} // bool setMicBias(int n);  //use AIC3206_MIC_BIAS_OFF or AIC3206_MIC_BIAS_2_5 or one of other choices defined earlier
            else if (isTarget(msg,addressOffset,"/u*","i")) {updateInputBasedOnMicDetect(msg.getInt(0));} // bool updateInputBasedOnMicDetect(int setting = AIC3206_INPUT_IN1); //which input to monitor
            else if (isTarget(msg,addressOffset,"/volume_*","f")) {volume_dB(msg.getFloat(0));} // bool volume_dB(float n);
            else if (isTarget(msg,addressOffset,"/volume","f")) {volume(msg.getFloat(0));} // bool volume(float n);
          }
        }
};

// ============== AudioControlWM8731 ====================
class OSCAudioControlWM8731 : public AudioControlWM8731, OSCAudioBase
{
    public:
        OSCAudioControlWM8731(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*",NULL)) {disable();} // bool disable(void) { return false; }
            else if (isTarget(msg,addressOffset,"/e*",NULL)) {enable();} // bool enable(void);
            else if (isTarget(msg,addressOffset,"/inputL*","f")) {inputLevel(msg.getFloat(0));} // bool inputLevel(float n); // range: 0.0f to 1.0f
            else if (isTarget(msg,addressOffset,"/inputS*","i")) {inputSelect(msg.getInt(0));} // bool inputSelect(int n);
            else if (isTarget(msg,addressOffset,"/v*","f")) {volume(msg.getFloat(0));} // bool volume(float n) { return volumeInteger(n * 80.0f + 47.499f); }
          }
        }
};

// ============== AudioControlWM8731master ====================
class OSCAudioControlWM8731master : public AudioControlWM8731master, OSCAudioBase
{
    public:
        OSCAudioControlWM8731master(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/e*",NULL)) {enable();} // bool enable(void);
          }
        }
};

// ============== AudioEffectBitcrusher ====================
class OSCAudioEffectBitcrusher : public AudioEffectBitcrusher, OSCAudioBase
{
    public:
        OSCAudioEffectBitcrusher(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/b*","i")) {bits(msg.getInt(0));} // void bits(uint8_t b) {
            else if (isTarget(msg,addressOffset,"/s*","f")) {sampleRate(msg.getFloat(0));} // void sampleRate(float hz) {
          }
        }
};

// ============== AudioEffectDelay ====================
class OSCAudioEffectDelay : public AudioEffectDelay, OSCAudioBase
{
    public:
        OSCAudioEffectDelay(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/de*","if")) {delay(msg.getInt(0),msg.getFloat(1));} // void delay(uint8_t channel, float milliseconds) {
            else if (isTarget(msg,addressOffset,"/di*","i")) {disable(msg.getInt(0));} // void disable(uint8_t channel) {
          }
        }
};

// ============== AudioEffectDelayExternal ====================
class OSCAudioEffectDelayExternal : public AudioEffectDelayExternal, OSCAudioBase
{
    public:
        OSCAudioEffectDelayExternal(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/de*","if")) {delay(msg.getInt(0),msg.getFloat(1));} // void delay(uint8_t channel, float milliseconds) {
            else if (isTarget(msg,addressOffset,"/di*","i")) {disable(msg.getInt(0));} // void disable(uint8_t channel) {
          }
        }
};

// ============== AudioEffectDigitalCombine ====================
class OSCAudioEffectDigitalCombine : public AudioEffectDigitalCombine, OSCAudioBase
{
    public:
        OSCAudioEffectDigitalCombine(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/s*","i")) {setCombineMode(msg.getInt(0));} // void setCombineMode(int mode_in) {
          }
        }
};

// ============== AudioEffectEnvelope ====================
class OSCAudioEffectEnvelope : public AudioEffectEnvelope, OSCAudioBase
{
    public:
        OSCAudioEffectEnvelope(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {attack(msg.getFloat(0));} // void attack(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/dec*","f")) {decay(msg.getFloat(0));} // void decay(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/del*","f")) {delay(msg.getFloat(0));} // void delay(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/h*","f")) {hold(msg.getFloat(0));} // void hold(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/isA*",NULL)) {isActive();} // bool isActive();
            else if (isTarget(msg,addressOffset,"/isS*",NULL)) {isSustain();} // bool isSustain();
            else if (isTarget(msg,addressOffset,"/noteOf*",NULL)) {noteOff();} // void noteOff();
            else if (isTarget(msg,addressOffset,"/noteOn",NULL)) {noteOn();} // void noteOn();
            else if (isTarget(msg,addressOffset,"/releaseN*","f")) {releaseNoteOn(msg.getFloat(0));} // void releaseNoteOn(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/release","f")) {release(msg.getFloat(0));} // void release(float milliseconds) {
            else if (isTarget(msg,addressOffset,"/s*","f")) {sustain(msg.getFloat(0));} // void sustain(float level) {
          }
        }
};
/*
// ============== AudioEffectExpEnvelope ====================
class OSCAudioEffectExpEnvelope : public AudioEffectExpEnvelope, OSCAudioBase
{
    public:
        OSCAudioEffectExpEnvelope(const char* _name) :  OSCAudioBase(_name) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","ff")) {attack(msg.getFloat(0),msg.getFloat(1));} // void attack(float milliseconds, float target_factor = TF)
            else if (isTarget(msg,addressOffset,"/c*",NULL)) {close();} // void close(){
            else if (isTarget(msg,addressOffset,"/dec*","ff")) {decay(msg.getFloat(0),msg.getFloat(1));} // void decay(float milliseconds, float target_factor = TF)
            else if (isTarget(msg,addressOffset,"/del*","f")) {delay(msg.getFloat(0));} // void delay(float milliseconds)
            else if (isTarget(msg,addressOffset,"/getG*",NULL)) {getGain();} // float getGain() {return HIRES_TO_FLOAT(mult_hires);}
            else if (isTarget(msg,addressOffset,"/getS*",NULL)) {getState();} // uint8_t getState();
            else if (isTarget(msg,addressOffset,"/h*","f")) {hold(msg.getFloat(0));} // void hold(float milliseconds)
            else if (isTarget(msg,addressOffset,"/isA*",NULL)) {isActive();} // bool isActive();
            else if (isTarget(msg,addressOffset,"/isS*",NULL)) {isSustain();} // bool isSustain();
            else if (isTarget(msg,addressOffset,"/noteOf*",NULL)) {noteOff();} // void noteOff();
            else if (isTarget(msg,addressOffset,"/noteOn",NULL)) {noteOn();} // void noteOn();
            else if (isTarget(msg,addressOffset,"/r*","ff")) {release(msg.getFloat(0),msg.getFloat(1));} // void release(float milliseconds, float target_factor = TF)
            else if (isTarget(msg,addressOffset,"/s*","f")) {sustain(msg.getFloat(0));} // void sustain(float level)
          }
        }
};
*/
// ============== AudioEffectFade ====================
class OSCAudioEffectFade : public AudioEffectFade, OSCAudioBase
{
    public:
        OSCAudioEffectFade(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/fadeI*","i")) {fadeIn(msg.getInt(0));} // void fadeIn(uint32_t milliseconds) {
            else if (isTarget(msg,addressOffset,"/fadeO*","i")) {fadeOut(msg.getInt(0));} // void fadeOut(uint32_t milliseconds) {
          }
        }
};

// ============== AudioEffectFreeverb ====================
class OSCAudioEffectFreeverb : public AudioEffectFreeverb, OSCAudioBase
{
    public:
        OSCAudioEffectFreeverb(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*","f")) {damping(msg.getFloat(0));} // void damping(float n) {
            else if (isTarget(msg,addressOffset,"/r*","f")) {roomsize(msg.getFloat(0));} // void roomsize(float n) {
          }
        }
};

// ============== AudioEffectFreeverbStereo ====================
class OSCAudioEffectFreeverbStereo : public AudioEffectFreeverbStereo, OSCAudioBase
{
    public:
        OSCAudioEffectFreeverbStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*","f")) {damping(msg.getFloat(0));} // void damping(float n) {
            else if (isTarget(msg,addressOffset,"/r*","f")) {roomsize(msg.getFloat(0));} // void roomsize(float n) {
          }
        }
};

// ============== AudioEffectGranular ====================
class OSCAudioEffectGranular : public AudioEffectGranular, OSCAudioBase
{
    public:
        OSCAudioEffectGranular(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/beginF*","f")) {beginFreeze(msg.getFloat(0));} // void beginFreeze(float grain_length) {
            else if (isTarget(msg,addressOffset,"/beginP*","f")) {beginPitchShift(msg.getFloat(0));} // void beginPitchShift(float grain_length) {
            // else if (isTarget(msg,addressOffset,"/begin","bi")) {begin(msg.getBlob(0),msg.getInt(1));} // void begin(int16_t *sample_bank_def, int16_t max_len_def);
            else if (isTarget(msg,addressOffset,"/se*","f")) {setSpeed(msg.getFloat(0));} // void setSpeed(float ratio) {
            else if (isTarget(msg,addressOffset,"/st*",NULL)) {stop();} // void stop();
          }
        }
};

// ============== AudioEffectMidSide ====================
class OSCAudioEffectMidSide : public AudioEffectMidSide, OSCAudioBase
{
    public:
        OSCAudioEffectMidSide(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/d*",NULL)) {decode();} // void decode() { encoding = false; }
            else if (isTarget(msg,addressOffset,"/e*",NULL)) {encode();} // void encode() { encoding = true; }
          }
        }
};

// ============== AudioEffectMultiply ====================
class OSCAudioEffectMultiply : public AudioEffectMultiply, OSCAudioBase
{
    public:
        OSCAudioEffectMultiply(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioEffectRectifier ====================
class OSCAudioEffectRectifier : public AudioEffectRectifier, OSCAudioBase
{
    public:
        OSCAudioEffectRectifier(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioEffectReverb ====================
class OSCAudioEffectReverb : public AudioEffectReverb, OSCAudioBase
{
    public:
        OSCAudioEffectReverb(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/r*","f")) {reverbTime(msg.getFloat(0));} // void reverbTime(float);
          }
        }
};

// ============== AudioEffectWaveFolder ====================
class OSCAudioEffectWaveFolder : public AudioEffectWaveFolder, OSCAudioBase
{
    public:
        OSCAudioEffectWaveFolder(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioEffectWaveshaper ====================
class OSCAudioEffectWaveshaper : public AudioEffectWaveshaper, OSCAudioBase
{
    public:
        OSCAudioEffectWaveshaper(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            // if (isTarget(msg,addressOffset,"/s*","bi")) {shape(msg.getBlob(0),msg.getInt(1));} // void shape(float* waveshape, int length);
          }
        }
};

// ============== AudioFilterBiquad ====================
class OSCAudioFilterBiquad : public AudioFilterBiquad, OSCAudioBase
{
    public:
        OSCAudioFilterBiquad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/setB*","iff")) {setBandpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2));} // void setBandpass(uint32_t stage, float frequency, float q = 1.0) {
            // else if (isTarget(msg,addressOffset,"/setC*","ib")) {setCoefficients(msg.getInt(0),msg.getBlob(1));} // void setCoefficients(uint32_t stage, const double *coefficients) {
            // else if (isTarget(msg,addressOffset,"/setC*","ib")) {setCoefficients(msg.getInt(0),msg.getBlob(1));} // void setCoefficients(uint32_t stage, const int *coefficients);
            else if (isTarget(msg,addressOffset,"/setHighS*","ifff")) {setHighShelf(msg.getInt(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3));} // void setHighShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
            else if (isTarget(msg,addressOffset,"/setHighp*","iff")) {setHighpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2));} // void setHighpass(uint32_t stage, float frequency, float q = 0.7071) {
            else if (isTarget(msg,addressOffset,"/setLowS*","ifff")) {setLowShelf(msg.getInt(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3));} // void setLowShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
            else if (isTarget(msg,addressOffset,"/setLowp*","iff")) {setLowpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2));} // void setLowpass(uint32_t stage, float frequency, float q = 0.7071f) {
            else if (isTarget(msg,addressOffset,"/setN*","iff")) {setNotch(msg.getInt(0),msg.getFloat(1),msg.getFloat(2));} // void setNotch(uint32_t stage, float frequency, float q = 1.0) {
          }
        }
};

// ============== AudioFilterFIR ====================
class OSCAudioFilterFIR : public AudioFilterFIR, OSCAudioBase
{
    public:
        OSCAudioFilterFIR(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            // if (isTarget(msg,addressOffset,"/b*","bi")) {begin(msg.getBlob(0),msg.getInt(1));} // void begin(const short *cp, int n_coeffs) {
            if (isTarget(msg,addressOffset,"/e*",NULL)) {end();} // void end(void) {
          }
        }
};

// ============== AudioFilterLadder ====================
class OSCAudioFilterLadder : public AudioFilterLadder, OSCAudioBase
{
    public:
        OSCAudioFilterLadder(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float FC);
            else if (isTarget(msg,addressOffset,"/inp*","f")) {inputDrive(msg.getFloat(0));} // void inputDrive(float drv);
            else if (isTarget(msg,addressOffset,"/int*","i")) {interpolationMethod((AudioFilterLadderInterpolation) msg.getInt(0));} // void interpolationMethod(AudioFilterLadderInterpolation im);
            else if (isTarget(msg,addressOffset,"/o*","f")) {octaveControl(msg.getFloat(0));} // void octaveControl(float octaves);
            else if (isTarget(msg,addressOffset,"/p*","f")) {passbandGain(msg.getFloat(0));} // void passbandGain(float passbandgain);
            else if (isTarget(msg,addressOffset,"/r*","f")) {resonance(msg.getFloat(0));} // void resonance(float reson);
          }
        }
};

// ============== AudioFilterStateVariable ====================
class OSCAudioFilterStateVariable : public AudioFilterStateVariable, OSCAudioBase
{
    public:
        OSCAudioFilterStateVariable(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/o*","f")) {octaveControl(msg.getFloat(0));} // void octaveControl(float n) {
            else if (isTarget(msg,addressOffset,"/r*","f")) {resonance(msg.getFloat(0));} // void resonance(float q) {
          }
        }
};

// ============== AudioInputAnalog ====================
class OSCAudioInputAnalog : public AudioInputAnalog, OSCAudioBase
{
    public:
        OSCAudioInputAnalog(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputAnalogStereo ====================
class OSCAudioInputAnalogStereo : public AudioInputAnalogStereo, OSCAudioBase
{
    public:
        OSCAudioInputAnalogStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputI2S ====================
class OSCAudioInputI2S : public AudioInputI2S, OSCAudioBase
{
    public:
        OSCAudioInputI2S(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputI2S2 ====================
class OSCAudioInputI2S2 : public AudioInputI2S2, OSCAudioBase
{
    public:
        OSCAudioInputI2S2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/v*",NULL)) {void dma_ch1_isr();} // friend void dma_ch1_isr(void);
          }
        }
};

// ============== AudioInputI2SHex ====================
class OSCAudioInputI2SHex : public AudioInputI2SHex, OSCAudioBase
{
    public:
        OSCAudioInputI2SHex(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputI2SOct ====================
class OSCAudioInputI2SOct : public AudioInputI2SOct, OSCAudioBase
{
    public:
        OSCAudioInputI2SOct(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputI2SQuad ====================
class OSCAudioInputI2SQuad : public AudioInputI2SQuad, OSCAudioBase
{
    public:
        OSCAudioInputI2SQuad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputPDM ====================
class OSCAudioInputPDM : public AudioInputPDM, OSCAudioBase
{
    public:
        OSCAudioInputPDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputPDM2 ====================
class OSCAudioInputPDM2 : public AudioInputPDM2, OSCAudioBase
{
    public:
        OSCAudioInputPDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputSPDIF3 ====================
class OSCAudioInputSPDIF3 : public AudioInputSPDIF3, OSCAudioBase
{
    public:
        OSCAudioInputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/i*",NULL)) {int sampleRate();} // static unsigned int sampleRate(void);
            else if (isTarget(msg,addressOffset,"/p*",NULL)) {pllLocked();} // static bool pllLocked(void);
          }
        }
};

// ============== AudioInputTDM ====================
class OSCAudioInputTDM : public AudioInputTDM, OSCAudioBase
{
    public:
        OSCAudioInputTDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioInputTDM2 ====================
class OSCAudioInputTDM2 : public AudioInputTDM2, OSCAudioBase
{
    public:
        OSCAudioInputTDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioMixer4 ====================
class OSCAudioMixer4 : public AudioMixer4, OSCAudioBase
{
    public:
        OSCAudioMixer4(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/g*","if")) {gain(msg.getInt(0),msg.getFloat(1));} // void gain(unsigned int channel, float gain) {
          }
        }
};

// ============== AudioOutputADAT ====================
class OSCAudioOutputADAT : public AudioOutputADAT, OSCAudioBase
{
    public:
        OSCAudioOutputADAT(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/m*",";")) {mute_PCM(msg.getBoolean(0));} // static void mute_PCM(const bool mute);
          }
        }
};

// ============== AudioOutputAnalog ====================
class OSCAudioOutputAnalog : public AudioOutputAnalog, OSCAudioBase
{
    public:
        OSCAudioOutputAnalog(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","i")) {analogReference(msg.getInt(0));} // void analogReference(int ref);
          }
        }
};

// ============== AudioOutputAnalogStereo ====================
class OSCAudioOutputAnalogStereo : public AudioOutputAnalogStereo, OSCAudioBase
{
    public:
        OSCAudioOutputAnalogStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","i")) {analogReference(msg.getInt(0));} // void analogReference(int ref);
          }
        }
};

// ============== AudioOutputI2S ====================
class OSCAudioOutputI2S : public AudioOutputI2S, OSCAudioBase
{
    public:
        OSCAudioOutputI2S(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/void dma_ch0*",NULL)) {void dma_ch0_isr();} // friend void dma_ch0_isr(void);
            else if (isTarget(msg,addressOffset,"/void dma_ch1*",NULL)) {void dma_ch1_isr();} // friend void dma_ch1_isr(void);
          }
        }
};

// ============== AudioOutputI2S2 ====================
class OSCAudioOutputI2S2 : public AudioOutputI2S2, OSCAudioBase
{
    public:
        OSCAudioOutputI2S2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/v*",NULL)) {void dma_ch0_isr();} // friend void dma_ch0_isr(void);
          }
        }
};

// ============== AudioOutputI2SHex ====================
class OSCAudioOutputI2SHex : public AudioOutputI2SHex, OSCAudioBase
{
    public:
        OSCAudioOutputI2SHex(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputI2SOct ====================
class OSCAudioOutputI2SOct : public AudioOutputI2SOct, OSCAudioBase
{
    public:
        OSCAudioOutputI2SOct(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputI2SQuad ====================
class OSCAudioOutputI2SQuad : public AudioOutputI2SQuad, OSCAudioBase
{
    public:
        OSCAudioOutputI2SQuad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputMQS ====================
class OSCAudioOutputMQS : public AudioOutputMQS, OSCAudioBase
{
    public:
        OSCAudioOutputMQS(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputPT8211 ====================
class OSCAudioOutputPT8211 : public AudioOutputPT8211, OSCAudioBase
{
    public:
        OSCAudioOutputPT8211(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputPT8211_2 ====================
class OSCAudioOutputPT8211_2 : public AudioOutputPT8211_2, OSCAudioBase
{
    public:
        OSCAudioOutputPT8211_2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputPWM ====================
class OSCAudioOutputPWM : public AudioOutputPWM, OSCAudioBase
{
    public:
        OSCAudioOutputPWM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputSPDIF ====================
class OSCAudioOutputSPDIF : public AudioOutputSPDIF, OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/m*",";")) {mute_PCM(msg.getBoolean(0));} // static void mute_PCM(const bool mute);
          }
        }
};

// ============== AudioOutputSPDIF2 ====================
class OSCAudioOutputSPDIF2 : public AudioOutputSPDIF2, OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/m*",";")) {mute_PCM(msg.getBoolean(0));} // static void mute_PCM(const bool mute);
          }
        }
};

// ============== AudioOutputSPDIF3 ====================
class OSCAudioOutputSPDIF3 : public AudioOutputSPDIF3, OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/m*",";")) {mute_PCM(msg.getBoolean(0));} // static void mute_PCM(const bool mute);
            // NOT DEFINED: else if (isTarget(msg,addressOffset,"/p*",NULL)) {pll_locked();} // static bool pll_locked(void);
          }
        }
};

// ============== AudioOutputTDM ====================
class OSCAudioOutputTDM : public AudioOutputTDM, OSCAudioBase
{
    public:
        OSCAudioOutputTDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioOutputTDM2 ====================
class OSCAudioOutputTDM2 : public AudioOutputTDM2, OSCAudioBase
{
    public:
        OSCAudioOutputTDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
          }
        }
};

// ============== AudioPlayMemory ====================
class OSCAudioPlayMemory : public AudioPlayMemory, OSCAudioBase
{
    public:
        OSCAudioPlayMemory(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/i*",NULL)) {isPlaying();} // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addressOffset,"/l*",NULL)) {lengthMillis();} // uint32_t lengthMillis(void);
            // else if (isTarget(msg,addressOffset,"/pl*","b")) {play(msg.getBlob(0));} // void play(const unsigned int *data);
            else if (isTarget(msg,addressOffset,"/po*",NULL)) {positionMillis();} // uint32_t positionMillis(void);
            else if (isTarget(msg,addressOffset,"/s*",NULL)) {stop();} // void stop(void);
          }
        }
};

// ============== AudioPlayQueue ====================
class OSCAudioPlayQueue : public AudioPlayQueue, OSCAudioBase
{
    public:
        OSCAudioPlayQueue(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // bool available(void);
            else if (isTarget(msg,addressOffset,"/g*",NULL)) {getBuffer();} // int16_t * getBuffer(void);
            else if (isTarget(msg,addressOffset,"/playB*",NULL)) {playBuffer();} // void playBuffer(void);
            // else if (isTarget(msg,addressOffset,"/play","bi")) {play(msg.getBlob(0),msg.getInt(1));} // void play(const int16_t *data, uint32_t len);
            else if (isTarget(msg,addressOffset,"/play","i")) {play(msg.getInt(0));} // void play(int16_t data);
            else if (isTarget(msg,addressOffset,"/se*","i")) {setMaxBuffers(msg.getInt(0));} // void setMaxBuffers(uint8_t);
            // NOT DEFINED: else if (isTarget(msg,addressOffset,"/st*",NULL)) {stop();} // void stop(void);
          }
        }
};

// ============== AudioPlaySdRaw ====================
class OSCAudioPlaySdRaw : public AudioPlaySdRaw, OSCAudioBase
{
    public:
        OSCAudioPlaySdRaw(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/b*",NULL)) {begin();} // void begin(void);
            else if (isTarget(msg,addressOffset,"/i*",NULL)) {isPlaying();} // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addressOffset,"/l*",NULL)) {lengthMillis();} // uint32_t lengthMillis(void);
            // else if (isTarget(msg,addressOffset,"/pl*","s")) {play(msg.getString(0));} // bool play(const char *filename);
            else if (isTarget(msg,addressOffset,"/po*",NULL)) {positionMillis();} // uint32_t positionMillis(void);
            else if (isTarget(msg,addressOffset,"/s*",NULL)) {stop();} // void stop(void);
          }
        }
};

// ============== AudioPlaySdWav ====================
class OSCAudioPlaySdWav : public AudioPlaySdWav, OSCAudioBase
{
    public:
        OSCAudioPlaySdWav(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/b*",NULL)) {begin();} // void begin(void);
            else if (isTarget(msg,addressOffset,"/isPa*",NULL)) {isPaused();} // bool isPaused(void);
            else if (isTarget(msg,addressOffset,"/isPl*",NULL)) {isPlaying();} // bool isPlaying(void);
            else if (isTarget(msg,addressOffset,"/isS*",NULL)) {isStopped();} // bool isStopped(void);
            else if (isTarget(msg,addressOffset,"/l*",NULL)) {lengthMillis();} // uint32_t lengthMillis(void);
            // else if (isTarget(msg,addressOffset,"/pl*","s")) {play(msg.getString(0));} // bool play(const char *filename);
            else if (isTarget(msg,addressOffset,"/po*",NULL)) {positionMillis();} // uint32_t positionMillis(void);
            else if (isTarget(msg,addressOffset,"/s*",NULL)) {stop();} // void stop(void);
            else if (isTarget(msg,addressOffset,"/t*",NULL)) {togglePlayPause();} // void togglePlayPause(void);
          }
        }
};

// ============== AudioPlaySerialflashRaw ====================
class OSCAudioPlaySerialflashRaw : public AudioPlaySerialflashRaw, OSCAudioBase
{
    public:
        OSCAudioPlaySerialflashRaw(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/b*",NULL)) {begin();} // void begin(void);
            else if (isTarget(msg,addressOffset,"/i*",NULL)) {isPlaying();} // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addressOffset,"/l*",NULL)) {lengthMillis();} // uint32_t lengthMillis(void);
            // else if (isTarget(msg,addressOffset,"/pl*","s")) {play(msg.getString(0));} // bool play(const char *filename);
            else if (isTarget(msg,addressOffset,"/po*",NULL)) {positionMillis();} // uint32_t positionMillis(void);
            else if (isTarget(msg,addressOffset,"/s*",NULL)) {stop();} // void stop(void);
          }
        }
};

// ============== AudioRecordQueue ====================
class OSCAudioRecordQueue : public AudioRecordQueue, OSCAudioBase
{
    public:
        OSCAudioRecordQueue(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*",NULL)) {available();} // int available(void);
            else if (isTarget(msg,addressOffset,"/b*",NULL)) {begin();} // void begin(void) {
            else if (isTarget(msg,addressOffset,"/c*",NULL)) {clear();} // void clear(void);
            else if (isTarget(msg,addressOffset,"/e*",NULL)) {end();} // void end(void) {
            else if (isTarget(msg,addressOffset,"/f*",NULL)) {freeBuffer();} // void freeBuffer(void);
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {readBuffer();} // int16_t * readBuffer(void);
          }
        }
};

// ============== AudioSynthKarplusStrong ====================
class OSCAudioSynthKarplusStrong : public AudioSynthKarplusStrong, OSCAudioBase
{
    public:
        OSCAudioSynthKarplusStrong(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/noteOf*","f")) {noteOff(msg.getFloat(0));} // void noteOff(float velocity) {
            else if (isTarget(msg,addressOffset,"/noteOn","ff")) {noteOn(msg.getFloat(0),msg.getFloat(1));} // void noteOn(float frequency, float velocity) {
          }
        }
};

// ============== AudioSynthNoisePink ====================
class OSCAudioSynthNoisePink : public AudioSynthNoisePink, OSCAudioBase
{
    public:
        OSCAudioSynthNoisePink(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
          }
        }
};

// ============== AudioSynthNoiseWhite ====================
class OSCAudioSynthNoiseWhite : public AudioSynthNoiseWhite, OSCAudioBase
{
    public:
        OSCAudioSynthNoiseWhite(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
          }
        }
};

// ============== AudioSynthSimpleDrum ====================
class OSCAudioSynthSimpleDrum : public AudioSynthSimpleDrum, OSCAudioBase
{
    public:
        OSCAudioSynthSimpleDrum(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq)
            else if (isTarget(msg,addressOffset,"/l*","i")) {length(msg.getInt(0));} // void length(int32_t milliseconds)
            else if (isTarget(msg,addressOffset,"/n*",NULL)) {noteOn();} // void noteOn();
            else if (isTarget(msg,addressOffset,"/p*","f")) {pitchMod(msg.getFloat(0));} // void pitchMod(float depth);
            else if (isTarget(msg,addressOffset,"/s*","f")) {secondMix(msg.getFloat(0));} // void secondMix(float level);
          }
        }
};

// ============== AudioSynthToneSweep ====================
class OSCAudioSynthToneSweep : public AudioSynthToneSweep, OSCAudioBase
{
    public:
        OSCAudioSynthToneSweep(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/i*",NULL)) {isPlaying();} // unsigned char isPlaying(void);
            else if (isTarget(msg,addressOffset,"/p*","fiif")) {play(msg.getFloat(0),msg.getInt(1),msg.getInt(2),msg.getFloat(3));} // boolean play(float t_amp,int t_lo,int t_hi,float t_time);
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {read();} // float read(void) {
          }
        }
};

// ============== AudioSynthWaveform ====================
class OSCAudioSynthWaveform : public AudioSynthWaveform, OSCAudioBase
{
    public:
        OSCAudioSynthWaveform(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/am*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {	// 0 to 1.0
            // else if (isTarget(msg,addressOffset,"/ar*","bf")) {arbitraryWaveform(msg.getBlob(0),msg.getFloat(1));} // void arbitraryWaveform(const int16_t *data, float maxFreq) {
            else if (isTarget(msg,addressOffset,"/b*","ffi")) {begin(msg.getFloat(0),msg.getFloat(1),msg.getInt(2));} // void begin(float t_amp, float t_freq, short t_type) {
            else if (isTarget(msg,addressOffset,"/b*","i")) {begin(msg.getInt(0));} // void begin(short t_type) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/o*","f")) {offset(msg.getFloat(0));} // void offset(float n) {
            else if (isTarget(msg,addressOffset,"/ph*","f")) {phase(msg.getFloat(0));} // void phase(float angle) {
            else if (isTarget(msg,addressOffset,"/pu*","f")) {pulseWidth(msg.getFloat(0));} // void pulseWidth(float n) {	// 0.0 to 1.0
          }
        }
};

// ============== AudioSynthWaveformDc ====================
class OSCAudioSynthWaveformDc : public AudioSynthWaveformDc, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformDc(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
            else if (isTarget(msg,addressOffset,"/a*","ff")) {amplitude(msg.getFloat(0),msg.getFloat(1));} // void amplitude(float n, float milliseconds) {
            else if (isTarget(msg,addressOffset,"/r*",NULL)) {read();} // float read(void) {
          }
        }
};

// ============== AudioSynthWaveformModulated ====================
class OSCAudioSynthWaveformModulated : public AudioSynthWaveformModulated, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformModulated(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/am*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {	// 0 to 1.0
            // else if (isTarget(msg,addressOffset,"/ar*","bf")) {arbitraryWaveform(msg.getBlob(0),msg.getFloat(1));} // void arbitraryWaveform(const int16_t *data, float maxFreq) {
            else if (isTarget(msg,addressOffset,"/b*","ffi")) {begin(msg.getFloat(0),msg.getFloat(1),msg.getInt(2));} // void begin(float t_amp, float t_freq, short t_type) {
            else if (isTarget(msg,addressOffset,"/b*","i")) {begin(msg.getInt(0));} // void begin(short t_type) {
            else if (isTarget(msg,addressOffset,"/frequencyM*","f")) {frequencyModulation(msg.getFloat(0));} // void frequencyModulation(float octaves) {
            else if (isTarget(msg,addressOffset,"/frequency","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/o*","f")) {offset(msg.getFloat(0));} // void offset(float n) {
            else if (isTarget(msg,addressOffset,"/p*","f")) {phaseModulation(msg.getFloat(0));} // void phaseModulation(float degrees) {
          }
        }
};

// ============== AudioSynthWaveformPWM ====================
class OSCAudioSynthWaveformPWM : public AudioSynthWaveformPWM, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformPWM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
          }
        }
};

// ============== AudioSynthWaveformSine ====================
class OSCAudioSynthWaveformSine : public AudioSynthWaveformSine, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSine(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/p*","f")) {phase(msg.getFloat(0));} // void phase(float angle) {
          }
        }
};

// ============== AudioSynthWaveformSineHires ====================
class OSCAudioSynthWaveformSineHires : public AudioSynthWaveformSineHires, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSineHires(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/p*","f")) {phase(msg.getFloat(0));} // void phase(float angle) {
          }
        }
};

// ============== AudioSynthWaveformSineModulated ====================
class OSCAudioSynthWaveformSineModulated : public AudioSynthWaveformSineModulated, OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSineModulated(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float n) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {frequency(msg.getFloat(0));} // void frequency(float freq) {
            else if (isTarget(msg,addressOffset,"/p*","f")) {phase(msg.getFloat(0));} // void phase(float angle) {
          }
        }
};

// ============== AudioSynthWavetable ====================
class OSCAudioSynthWavetable : public AudioSynthWavetable, OSCAudioBase
{
    public:
        OSCAudioSynthWavetable(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addressOffset)
        {
          if (isMine(msg,addressOffset))
          { 
            if (isTarget(msg,addressOffset,"/a*","f")) {amplitude(msg.getFloat(0));} // void amplitude(float v) {
            else if (isTarget(msg,addressOffset,"/f*","f")) {freqToNote(msg.getFloat(0));} // static int freqToNote(float freq) {
            else if (isTarget(msg,addressOffset,"/g*",NULL)) {getEnvState();} // envelopeStateEnum getEnvState(void) { return env_state; }
            else if (isTarget(msg,addressOffset,"/i*",NULL)) {isPlaying();} // bool isPlaying(void) { return env_state != STATE_IDLE; }
            else if (isTarget(msg,addressOffset,"/m*","i")) {midi_volume_transform(msg.getInt(0));} // static float midi_volume_transform(int midi_amp) {
            else if (isTarget(msg,addressOffset,"/n*","i")) {noteToFreq(msg.getInt(0));} // static float noteToFreq(int note) {
            else if (isTarget(msg,addressOffset,"/playF*","fi")) {playFrequency(msg.getFloat(0),msg.getInt(1));} // void playFrequency(float freq, int amp = DEFAULT_AMPLITUDE);
            else if (isTarget(msg,addressOffset,"/playN*","ii")) {playNote(msg.getInt(0),msg.getInt(1));} // void playNote(int note, int amp = DEFAULT_AMPLITUDE);
            else if (isTarget(msg,addressOffset,"/setF*","f")) {setFrequency(msg.getFloat(0));} // void setFrequency(float freq);
            // else if (isTarget(msg,addressOffset,"/setI*","b")) {setInstrument(msg.getBlob(0));} // void setInstrument(const instrument_data& instrument) {
            else if (isTarget(msg,addressOffset,"/st*",NULL)) {stop();} // void stop(void);
          }
        }
};

#define OSC_AUDIO_CLASSES \
	OSC_CLASS(AsyncAudioInputSPDIF3,AsyncOSCAudioInputSPDIF3) \
	OSC_CLASS(AudioAmplifier,OSCAudioAmplifier) \
	OSC_CLASS(AudioAnalyzeFFT1024,OSCAudioAnalyzeFFT1024) \
	OSC_CLASS(AudioAnalyzeFFT256,OSCAudioAnalyzeFFT256) \
	OSC_CLASS(AudioAnalyzeNoteFrequency,OSCAudioAnalyzeNoteFrequency) \
	OSC_CLASS(AudioAnalyzePeak,OSCAudioAnalyzePeak) \
	OSC_CLASS(AudioAnalyzePrint,OSCAudioAnalyzePrint) \
	OSC_CLASS(AudioAnalyzeRMS,OSCAudioAnalyzeRMS) \
	OSC_CLASS(AudioAnalyzeToneDetect,OSCAudioAnalyzeToneDetect) \
	OSC_CLASS(AudioControlAK4558,OSCAudioControlAK4558) \
	OSC_CLASS(AudioControlCS42448,OSCAudioControlCS42448) \
	OSC_CLASS(AudioControlCS4272,OSCAudioControlCS4272) \
	OSC_CLASS(AudioControlSGTL5000,OSCAudioControlSGTL5000) \
	OSC_CLASS(AudioControlTLV320AIC3206,OSCAudioControlTLV320AIC3206) \
	OSC_CLASS(AudioControlWM8731,OSCAudioControlWM8731) \
	OSC_CLASS(AudioControlWM8731master,OSCAudioControlWM8731master) \
	OSC_CLASS(AudioEffectBitcrusher,OSCAudioEffectBitcrusher) \
	OSC_CLASS(AudioEffectDelay,OSCAudioEffectDelay) \
	OSC_CLASS(AudioEffectDelayExternal,OSCAudioEffectDelayExternal) \
	OSC_CLASS(AudioEffectDigitalCombine,OSCAudioEffectDigitalCombine) \
	OSC_CLASS(AudioEffectEnvelope,OSCAudioEffectEnvelope) \
	OSC_CLASS(AudioEffectFade,OSCAudioEffectFade) \
	OSC_CLASS(AudioEffectFreeverb,OSCAudioEffectFreeverb) \
	OSC_CLASS(AudioEffectFreeverbStereo,OSCAudioEffectFreeverbStereo) \
	OSC_CLASS(AudioEffectGranular,OSCAudioEffectGranular) \
	OSC_CLASS(AudioEffectMidSide,OSCAudioEffectMidSide) \
	OSC_CLASS(AudioEffectMultiply,OSCAudioEffectMultiply) \
	OSC_CLASS(AudioEffectRectifier,OSCAudioEffectRectifier) \
	OSC_CLASS(AudioEffectReverb,OSCAudioEffectReverb) \
	OSC_CLASS(AudioEffectWaveFolder,OSCAudioEffectWaveFolder) \
	OSC_CLASS(AudioEffectWaveshaper,OSCAudioEffectWaveshaper) \
	OSC_CLASS(AudioFilterBiquad,OSCAudioFilterBiquad) \
	OSC_CLASS(AudioFilterFIR,OSCAudioFilterFIR) \
	OSC_CLASS(AudioFilterLadder,OSCAudioFilterLadder) \
	OSC_CLASS(AudioFilterStateVariable,OSCAudioFilterStateVariable) \
	OSC_CLASS(AudioInputAnalog,OSCAudioInputAnalog) \
	OSC_CLASS(AudioInputAnalogStereo,OSCAudioInputAnalogStereo) \
	OSC_CLASS(AudioInputI2S,OSCAudioInputI2S) \
	OSC_CLASS(AudioInputI2S2,OSCAudioInputI2S2) \
	OSC_CLASS(AudioInputI2SHex,OSCAudioInputI2SHex) \
	OSC_CLASS(AudioInputI2SOct,OSCAudioInputI2SOct) \
	OSC_CLASS(AudioInputI2SQuad,OSCAudioInputI2SQuad) \
	OSC_CLASS(AudioInputPDM,OSCAudioInputPDM) \
	OSC_CLASS(AudioInputPDM2,OSCAudioInputPDM2) \
	OSC_CLASS(AudioInputSPDIF3,OSCAudioInputSPDIF3) \
	OSC_CLASS(AudioInputTDM,OSCAudioInputTDM) \
	OSC_CLASS(AudioInputTDM2,OSCAudioInputTDM2) \
	OSC_CLASS(AudioMixer4,OSCAudioMixer4) \
	OSC_CLASS(AudioOutputADAT,OSCAudioOutputADAT) \
	OSC_CLASS(AudioOutputAnalog,OSCAudioOutputAnalog) \
	OSC_CLASS(AudioOutputAnalogStereo,OSCAudioOutputAnalogStereo) \
	OSC_CLASS(AudioOutputI2S,OSCAudioOutputI2S) \
	OSC_CLASS(AudioOutputI2S2,OSCAudioOutputI2S2) \
	OSC_CLASS(AudioOutputI2SHex,OSCAudioOutputI2SHex) \
	OSC_CLASS(AudioOutputI2SOct,OSCAudioOutputI2SOct) \
	OSC_CLASS(AudioOutputI2SQuad,OSCAudioOutputI2SQuad) \
	OSC_CLASS(AudioOutputMQS,OSCAudioOutputMQS) \
	OSC_CLASS(AudioOutputPT8211,OSCAudioOutputPT8211) \
	OSC_CLASS(AudioOutputPT8211_2,OSCAudioOutputPT8211_2) \
	OSC_CLASS(AudioOutputPWM,OSCAudioOutputPWM) \
	OSC_CLASS(AudioOutputSPDIF,OSCAudioOutputSPDIF) \
	OSC_CLASS(AudioOutputSPDIF2,OSCAudioOutputSPDIF2) \
	OSC_CLASS(AudioOutputSPDIF3,OSCAudioOutputSPDIF3) \
	OSC_CLASS(AudioOutputTDM,OSCAudioOutputTDM) \
	OSC_CLASS(AudioOutputTDM2,OSCAudioOutputTDM2) \
	OSC_CLASS(AudioPlayMemory,OSCAudioPlayMemory) \
	OSC_CLASS(AudioPlayQueue,OSCAudioPlayQueue) \
	OSC_CLASS(AudioPlaySdRaw,OSCAudioPlaySdRaw) \
	OSC_CLASS(AudioPlaySdWav,OSCAudioPlaySdWav) \
	OSC_CLASS(AudioPlaySerialflashRaw,OSCAudioPlaySerialflashRaw) \
	OSC_CLASS(AudioRecordQueue,OSCAudioRecordQueue) \
	OSC_CLASS(AudioSynthKarplusStrong,OSCAudioSynthKarplusStrong) \
	OSC_CLASS(AudioSynthNoisePink,OSCAudioSynthNoisePink) \
	OSC_CLASS(AudioSynthNoiseWhite,OSCAudioSynthNoiseWhite) \
	OSC_CLASS(AudioSynthSimpleDrum,OSCAudioSynthSimpleDrum) \
	OSC_CLASS(AudioSynthToneSweep,OSCAudioSynthToneSweep) \
	OSC_CLASS(AudioSynthWaveform,OSCAudioSynthWaveform) \
	OSC_CLASS(AudioSynthWaveformDc,OSCAudioSynthWaveformDc) \
	OSC_CLASS(AudioSynthWaveformModulated,OSCAudioSynthWaveformModulated) \
	OSC_CLASS(AudioSynthWaveformPWM,OSCAudioSynthWaveformPWM) \
	OSC_CLASS(AudioSynthWaveformSine,OSCAudioSynthWaveformSine) \
	OSC_CLASS(AudioSynthWaveformSineHires,OSCAudioSynthWaveformSineHires) \
	OSC_CLASS(AudioSynthWaveformSineModulated,OSCAudioSynthWaveformSineModulated) \
	OSC_CLASS(AudioSynthWavetable,OSCAudioSynthWavetable) \

