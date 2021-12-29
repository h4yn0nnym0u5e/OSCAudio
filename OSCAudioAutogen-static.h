/* Open Sound Control for Audio Library for Teensy 3.x, 4.x
 * Copyright (c) 2021, Jonathan Oakley, teensy-osc@0akley.co.uk
 *
 * Development of this library was enabled by PJRC.COM, LLC by sales of
 * Teensy and Audio Adaptor boards, implementing libraries, and maintaining
 * the forum at https://forum.pjrc.com/ 
 *
 * Please support PJRC's efforts to develop open source software by 
 * purchasing Teensy or other PJRC products.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice, development funding notice, and this permission
 * notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
 
// ============== AsyncAudioInputSPDIF3 ====================
class AsyncOSCAudioInputSPDIF3 : public AsyncAudioInputSPDIF3, public OSCAudioBase
{
    public:
        AsyncOSCAudioInputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/b*",NULL)) {begin(); addReplyExecuted(msg,addrOff,reply);} // void begin();
            else if (isTarget(msg,addrOff,"/getA*",NULL)) {addReplyResult(msg,addrOff,reply,(float)getAttenuation()); } // double getAttenuation() const;
            else if (isTarget(msg,addrOff,"/getB*",NULL)) {addReplyResult(msg,addrOff,reply,(float)getBufferedTime()); } // double getBufferedTime() const;
            else if (isTarget(msg,addrOff,"/getH*",NULL)) {addReplyResult(msg,addrOff,reply,getHalfFilterLength()); } // int32_t getHalfFilterLength() const;
            else if (isTarget(msg,addrOff,"/getI*",NULL)) {addReplyResult(msg,addrOff,reply,(float)getInputFrequency()); } // double getInputFrequency() const;
            else if (isTarget(msg,addrOff,"/getT*",NULL)) {addReplyResult(msg,addrOff,reply,(float)getTargetLantency()); } // double getTargetLantency() const;
            else if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,isLocked()); } // static bool isLocked();
			}
		}
};

// ============== AudioAmplifier ====================
class OSCAudioAmplifier : public AudioAmplifier, public OSCAudioBase
{
    public:
        OSCAudioAmplifier(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/g*","f")) {gain(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void gain(float n)
			}
		}
};

// ============== AudioAnalyzeFFT1024 ====================
class OSCAudioAnalyzeFFT1024 : public AudioAnalyzeFFT1024, public OSCAudioBase
{
    public:
        OSCAudioAnalyzeFFT1024(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/ava*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available() {
            else if (isTarget(msg,addrOff,"/ave*","i")) {averageTogether(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void averageTogether(uint8_t n) {
            else if (isTarget(msg,addrOff,"/r*","ii")) {addReplyResult(msg,addrOff,reply,read(msg.getInt(0),msg.getInt(1))); } // float read(unsigned int binFirst, unsigned int binLast) {
            else if (isTarget(msg,addrOff,"/r*","i")) {addReplyResult(msg,addrOff,reply,read(msg.getInt(0))); } // float read(unsigned int binNumber) {
            else if (isTarget(msg,addrOff,"/w*","s")) {addReplyResult(msg,addrOff,reply,windowFunction(msg)); } // void windowFunction(const int16_t *w) {
			}
		}
	private:
		bool windowFunction(OSCMessage& msg);
};

// ============== AudioAnalyzeFFT256 ====================
class OSCAudioAnalyzeFFT256 : public AudioAnalyzeFFT256, public OSCAudioBase
{
    public:
        OSCAudioAnalyzeFFT256(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/ava*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available() {
            else if (isTarget(msg,addrOff,"/ave*","i")) {averageTogether(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void averageTogether(uint8_t n) {
            else if (isTarget(msg,addrOff,"/r*","ii")) {addReplyResult(msg,addrOff,reply,read(msg.getInt(0),msg.getInt(1))); } // float read(unsigned int binFirst, unsigned int binLast) {
            else if (isTarget(msg,addrOff,"/r*","i")) {addReplyResult(msg,addrOff,reply,read(msg.getInt(0))); } // float read(unsigned int binNumber) {
            else if (isTarget(msg,addrOff,"/w*","s")) {addReplyResult(msg,addrOff,reply,windowFunction(msg)); } // void windowFunction(const int16_t *w) {
			}
		}
	private:
		bool windowFunction(OSCMessage& msg);
};

// ============== AudioAnalyzeNoteFrequency ====================
class OSCAudioAnalyzeNoteFrequency : public AudioAnalyzeNoteFrequency, public OSCAudioBase
{
    public:
        OSCAudioAnalyzeNoteFrequency(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available( void );
            else if (isTarget(msg,addrOff,"/b*","f")) {begin(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void begin( float threshold );
            else if (isTarget(msg,addrOff,"/p*",NULL)) {addReplyResult(msg,addrOff,reply,probability()); } // float probability( void );
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read( void );
            else if (isTarget(msg,addrOff,"/t*","f")) {threshold(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void threshold( float p );
			}
		}
};

// ============== AudioAnalyzePeak ====================
class OSCAudioAnalyzePeak : public AudioAnalyzePeak, public OSCAudioBase
{
    public:
        OSCAudioAnalyzePeak(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available(void) {
            else if (isTarget(msg,addrOff,"/readP*",NULL)) {addReplyResult(msg,addrOff,reply,readPeakToPeak()); } // float readPeakToPeak(void) {
            else if (isTarget(msg,addrOff,"/read",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read(void) {
			}
		}
};

// ============== AudioAnalyzePrint ====================
class OSCAudioAnalyzePrint : public AudioAnalyzePrint, public OSCAudioBase
{
    public:
        OSCAudioAnalyzePrint(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this), namePtr(0) {}
        ~OSCAudioAnalyzePrint() { free(namePtr);} 

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*","i")) {delay(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void delay(uint32_t num) { delay_length = num; }
            else if (isTarget(msg,addrOff,"/l*","i")) {length(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void length(uint32_t num) { print_length = num; }
            else if (isTarget(msg,addrOff,"/n*","s")) {addReplyResult(msg,addrOff,reply,name(msg)); } // void name(const char *str) { myname = str; }
            // NOT DEFINED: else if (isTarget(msg,addrOff,"/t*","fi")) {trigger(msg.getFloat(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void trigger(float level, int edge);
            else if (isTarget(msg,addrOff,"/t*",NULL)) {trigger(); addReplyExecuted(msg,addrOff,reply);} // void trigger(void);
			}
		}
	private:
		bool name(OSCMessage& msg);
		char* namePtr;
};

// ============== AudioAnalyzeRMS ====================
class OSCAudioAnalyzeRMS : public AudioAnalyzeRMS, public OSCAudioBase
{
    public:
        OSCAudioAnalyzeRMS(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available(void) {
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read(void);
			}
		}
};

// ============== AudioAnalyzeToneDetect ====================
class OSCAudioAnalyzeToneDetect : public AudioAnalyzeToneDetect, public OSCAudioBase
{
    public:
        OSCAudioAnalyzeToneDetect(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available(void) {
            else if (isTarget(msg,addrOff,"/f*","fi")) {frequency(msg.getFloat(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq, uint16_t cycles=10) {
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read(void);
            else if (isTarget(msg,addrOff,"/s*","iii")) {set_params(msg.getInt(0),msg.getInt(1),msg.getInt(2)); addReplyExecuted(msg,addrOff,reply);} // void set_params(int32_t coef, uint16_t cycles, uint16_t len);
            else if (isTarget(msg,addrOff,"/t*","f")) {threshold(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void threshold(float level) {
			}
		}
};

// ============== AudioControlAK4558 ====================
class OSCAudioControlAK4558 : public AudioControlAK4558, public OSCAudioBase
{
    public:
        OSCAudioControlAK4558(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/disableI*",NULL)) {addReplyResult(msg,addrOff,reply,disableIn()); } // bool disableIn(void);	//powers down ADC
            else if (isTarget(msg,addrOff,"/disableO*",NULL)) {addReplyResult(msg,addrOff,reply,disableOut()); } // bool disableOut(void);	//powers down DAC
            else if (isTarget(msg,addrOff,"/disable",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void) { return (disableIn()&&disableOut()); }	//powers down ADC/DAC
            else if (isTarget(msg,addrOff,"/enableI*",NULL)) {addReplyResult(msg,addrOff,reply,enableIn()); } // bool enableIn(void);	//powers up ADC
            else if (isTarget(msg,addrOff,"/enableO*",NULL)) {addReplyResult(msg,addrOff,reply,enableOut()); } // bool enableOut(void);	//powers up DAC
            else if (isTarget(msg,addrOff,"/enable",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);		//enables the CODEC, does not power up ADC nor DAC (use enableIn() and enableOut() for selective power up)
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float n) { return false; }	//not supported by AK4558
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n) { return false; }	//sets inputs to mono left, mono right, stereo (default stereo), not yet implemented
            else if (isTarget(msg,addrOff,"/volumeL*","f")) {addReplyResult(msg,addrOff,reply,volumeLeft(msg.getFloat(0))); } // bool volumeLeft(float n);	//sets LOUT volume to n (range 0.0 - 1.0)
            else if (isTarget(msg,addrOff,"/volumeR*","f")) {addReplyResult(msg,addrOff,reply,volumeRight(msg.getFloat(0))); } // bool volumeRight(float n);	//sets ROUT volume to n (range 0.0 - 1.0)
            else if (isTarget(msg,addrOff,"/volume","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float n);	//sets LOUT/ROUT volume to n (range 0.0 - 1.0)
			}
		}
};

// ============== AudioControlCS42448 ====================
class OSCAudioControlCS42448 : public AudioControlCS42448, public OSCAudioBase
{
    public:
        OSCAudioControlCS42448(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void) {
            else if (isTarget(msg,addrOff,"/e*",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float level) {
            else if (isTarget(msg,addrOff,"/inputL*","if")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getInt(0),msg.getFloat(1))); } // bool inputLevel(int channel, float level) {
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n) {
            else if (isTarget(msg,addrOff,"/s*","i")) {setAddress(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void setAddress(uint8_t addr) {
            else if (isTarget(msg,addrOff,"/v*","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float level) {
            else if (isTarget(msg,addrOff,"/v*","if")) {addReplyResult(msg,addrOff,reply,volume(msg.getInt(0),msg.getFloat(1))); } // bool volume(int channel, float level) {
			}
		}
};

// ============== AudioControlCS4272 ====================
class OSCAudioControlCS4272 : public AudioControlCS4272, public OSCAudioBase
{
    public:
        OSCAudioControlCS4272(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/da*","ff")) {addReplyResult(msg,addrOff,reply,dacVolume(msg.getFloat(0),msg.getFloat(1))); } // bool dacVolume(float left, float right);
            else if (isTarget(msg,addrOff,"/da*","f")) {addReplyResult(msg,addrOff,reply,dacVolume(msg.getFloat(0))); } // bool dacVolume(float n) { return volumeInteger(n * 127 + 0.499f); }
            else if (isTarget(msg,addrOff,"/disableD*",NULL)) {addReplyResult(msg,addrOff,reply,disableDither()); } // bool disableDither(void);
            else if (isTarget(msg,addrOff,"/disable",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void) { return false; }
            else if (isTarget(msg,addrOff,"/enableD*",NULL)) {addReplyResult(msg,addrOff,reply,enableDither()); } // bool enableDither(void);
            else if (isTarget(msg,addrOff,"/enable",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float n) { return false; }
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n) { return false; }
            else if (isTarget(msg,addrOff,"/muteI*",NULL)) {addReplyResult(msg,addrOff,reply,muteInput()); } // bool muteInput(void);
            else if (isTarget(msg,addrOff,"/muteO*",NULL)) {addReplyResult(msg,addrOff,reply,muteOutput()); } // bool muteOutput(void);
            else if (isTarget(msg,addrOff,"/unmuteI*",NULL)) {addReplyResult(msg,addrOff,reply,unmuteInput()); } // bool unmuteInput(void);
            else if (isTarget(msg,addrOff,"/unmuteO*",NULL)) {addReplyResult(msg,addrOff,reply,unmuteOutput()); } // bool unmuteOutput(void);
            else if (isTarget(msg,addrOff,"/v*","ff")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0),msg.getFloat(1))); } // bool volume(float left, float right);
            else if (isTarget(msg,addrOff,"/v*","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float n) { return volumeInteger(n * 127 + 0.499f); }
			}
		}
};

// ============== AudioControlSGTL5000 ====================
class OSCAudioControlSGTL5000 : public AudioControlSGTL5000, public OSCAudioBase
{
    public:
        OSCAudioControlSGTL5000(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/adcHighPassFilterD*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)adcHighPassFilterDisable()); } // unsigned short adcHighPassFilterDisable(void);
            else if (isTarget(msg,addrOff,"/adcHighPassFilterE*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)adcHighPassFilterEnable()); } // unsigned short adcHighPassFilterEnable(void);
            else if (isTarget(msg,addrOff,"/adcHighPassFilterF*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)adcHighPassFilterFreeze()); } // unsigned short adcHighPassFilterFreeze(void);
            else if (isTarget(msg,addrOff,"/audioPo*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)audioPostProcessorEnable()); } // unsigned short audioPostProcessorEnable(void);
            else if (isTarget(msg,addrOff,"/audioPre*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)audioPreProcessorEnable()); } // unsigned short audioPreProcessorEnable(void);
            else if (isTarget(msg,addrOff,"/audioPro*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)audioProcessorDisable()); } // unsigned short audioProcessorDisable(void);
            else if (isTarget(msg,addrOff,"/autoVolumeC*","iiifff")) {addReplyResult(msg,addrOff,reply,(uint16_t)autoVolumeControl(msg.getInt(0),msg.getInt(1),msg.getInt(2),msg.getFloat(3),msg.getFloat(4),msg.getFloat(5))); } // unsigned short autoVolumeControl(uint8_t maxGain, uint8_t lbiResponse, uint8_t hardLimit, float threshold, float attack, float decay);
            else if (isTarget(msg,addrOff,"/autoVolumeD*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)autoVolumeDisable()); } // unsigned short autoVolumeDisable(void);
            else if (isTarget(msg,addrOff,"/autoVolumeE*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)autoVolumeEnable()); } // unsigned short autoVolumeEnable(void);
            else if (isTarget(msg,addrOff,"/dacVolumeRampD*",NULL)) {addReplyResult(msg,addrOff,reply,dacVolumeRampDisable()); } // bool dacVolumeRampDisable();
            else if (isTarget(msg,addrOff,"/dacVolumeRampL*",NULL)) {addReplyResult(msg,addrOff,reply,dacVolumeRampLinear()); } // bool dacVolumeRampLinear();
            else if (isTarget(msg,addrOff,"/dacVolumeRamp",NULL)) {addReplyResult(msg,addrOff,reply,dacVolumeRamp()); } // bool dacVolumeRamp();
            else if (isTarget(msg,addrOff,"/dacVolume","ff")) {addReplyResult(msg,addrOff,reply,(uint16_t)dacVolume(msg.getFloat(0),msg.getFloat(1))); } // unsigned short dacVolume(float left, float right);
            else if (isTarget(msg,addrOff,"/dacVolume","f")) {addReplyResult(msg,addrOff,reply,(uint16_t)dacVolume(msg.getFloat(0))); } // unsigned short dacVolume(float n);
            else if (isTarget(msg,addrOff,"/di*",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void) { return false; }
            else if (isTarget(msg,addrOff,"/ena*","ii")) {addReplyResult(msg,addrOff,reply,enable(msg.getInt(0),msg.getInt(1))); } // bool enable(const unsigned extMCLK, const uint32_t pllFreq = (4096.0l * AUDIO_SAMPLE_RATE_EXACT) ); //With extMCLK > 0, the SGTL acts as Master
            else if (isTarget(msg,addrOff,"/ena*",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);//For Teensy LC the SGTL acts as master, for all other Teensys as slave.
            else if (isTarget(msg,addrOff,"/enhanceBassD*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)enhanceBassDisable()); } // unsigned short enhanceBassDisable(void);
            else if (isTarget(msg,addrOff,"/enhanceBassE*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)enhanceBassEnable()); } // unsigned short enhanceBassEnable(void);
            else if (isTarget(msg,addrOff,"/enhanceBass","ff")) {addReplyResult(msg,addrOff,reply,(uint16_t)enhanceBass(msg.getFloat(0),msg.getFloat(1))); } // unsigned short enhanceBass(float lr_lev, float bass_lev);
            else if (isTarget(msg,addrOff,"/enhanceBass","ffii")) {addReplyResult(msg,addrOff,reply,(uint16_t)enhanceBass(msg.getFloat(0),msg.getFloat(1),msg.getInt(2),msg.getInt(3))); } // unsigned short enhanceBass(float lr_lev, float bass_lev, uint8_t hpf_bypass, uint8_t cutoff);
            else if (isTarget(msg,addrOff,"/eqBands","fffff")) {eqBands(msg.getFloat(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3),msg.getFloat(4)); addReplyExecuted(msg,addrOff,reply);} // void eqBands(float bass, float mid_bass, float midrange, float mid_treble, float treble);
            else if (isTarget(msg,addrOff,"/eqBands","ff")) {eqBands(msg.getFloat(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void eqBands(float bass, float treble);
            else if (isTarget(msg,addrOff,"/eqBand","if")) {addReplyResult(msg,addrOff,reply,(uint16_t)eqBand(msg.getInt(0),msg.getFloat(1))); } // unsigned short eqBand(uint8_t bandNum, float n);
            else if (isTarget(msg,addrOff,"/eqFilterC*","i")) {addReplyResult(msg,addrOff,reply,(uint16_t)eqFilterCount(msg.getInt(0))); } // unsigned short eqFilterCount(uint8_t n);
            else if (isTarget(msg,addrOff,"/eqFilter","ib")) {eqFilter(msg); addReplyExecuted(msg,addrOff,reply);} // void eqFilter(uint8_t filterNum, int *filterParameters);
            else if (isTarget(msg,addrOff,"/eqS*","i")) {addReplyResult(msg,addrOff,reply,(uint16_t)eqSelect(msg.getInt(0))); } // unsigned short eqSelect(uint8_t n);
            else if (isTarget(msg,addrOff,"/h*","i")) {addReplyResult(msg,addrOff,reply,headphoneSelect(msg.getInt(0))); } // bool headphoneSelect(int n) {
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float n) {return false;}
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n) {
            else if (isTarget(msg,addrOff,"/k*",NULL)) {killAutomation(); addReplyExecuted(msg,addrOff,reply);} // void killAutomation(void) { semi_automated=false; }
            else if (isTarget(msg,addrOff,"/lineI*","ii")) {addReplyResult(msg,addrOff,reply,lineInLevel(msg.getInt(0),msg.getInt(1))); } // bool lineInLevel(uint8_t left, uint8_t right);
            else if (isTarget(msg,addrOff,"/lineI*","i")) {addReplyResult(msg,addrOff,reply,lineInLevel(msg.getInt(0))); } // bool lineInLevel(uint8_t n) { return lineInLevel(n, n); }
            else if (isTarget(msg,addrOff,"/lineO*","ii")) {addReplyResult(msg,addrOff,reply,(uint16_t)lineOutLevel(msg.getInt(0),msg.getInt(1))); } // unsigned short lineOutLevel(uint8_t left, uint8_t right);
            else if (isTarget(msg,addrOff,"/lineO*","i")) {addReplyResult(msg,addrOff,reply,(uint16_t)lineOutLevel(msg.getInt(0))); } // unsigned short lineOutLevel(uint8_t n);
            else if (isTarget(msg,addrOff,"/mi*","i")) {addReplyResult(msg,addrOff,reply,micGain(msg.getInt(0))); } // bool micGain(unsigned int dB);
            else if (isTarget(msg,addrOff,"/muteH*",NULL)) {addReplyResult(msg,addrOff,reply,muteHeadphone()); } // bool muteHeadphone(void) { return write(0x0024, ana_ctrl | (1<<4)); }
            else if (isTarget(msg,addrOff,"/muteL*",NULL)) {addReplyResult(msg,addrOff,reply,muteLineout()); } // bool muteLineout(void) { return write(0x0024, ana_ctrl | (1<<8)); }
            else if (isTarget(msg,addrOff,"/se*","i")) {setAddress(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void setAddress(uint8_t level);
            else if (isTarget(msg,addrOff,"/surroundSoundD*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)surroundSoundDisable()); } // unsigned short surroundSoundDisable(void);
            else if (isTarget(msg,addrOff,"/surroundSoundE*",NULL)) {addReplyResult(msg,addrOff,reply,(uint16_t)surroundSoundEnable()); } // unsigned short surroundSoundEnable(void);
            else if (isTarget(msg,addrOff,"/surroundSound","i")) {addReplyResult(msg,addrOff,reply,(uint16_t)surroundSound(msg.getInt(0))); } // unsigned short surroundSound(uint8_t width);
            else if (isTarget(msg,addrOff,"/surroundSound","ii")) {addReplyResult(msg,addrOff,reply,(uint16_t)surroundSound(msg.getInt(0),msg.getInt(1))); } // unsigned short surroundSound(uint8_t width, uint8_t select);
            else if (isTarget(msg,addrOff,"/unmuteH*",NULL)) {addReplyResult(msg,addrOff,reply,unmuteHeadphone()); } // bool unmuteHeadphone(void) { return write(0x0024, ana_ctrl & ~(1<<4)); }
            else if (isTarget(msg,addrOff,"/unmuteL*",NULL)) {addReplyResult(msg,addrOff,reply,unmuteLineout()); } // bool unmuteLineout(void) { return write(0x0024, ana_ctrl & ~(1<<8)); }
            else if (isTarget(msg,addrOff,"/v*","ff")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0),msg.getFloat(1))); } // bool volume(float left, float right);
            else if (isTarget(msg,addrOff,"/v*","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float n) { return volumeInteger(n * 129 + 0.499f); }
			}
		}
	private:
		void eqFilter(OSCMessage& msg);
};

// ============== AudioControlTLV320AIC3206 ====================
class OSCAudioControlTLV320AIC3206 : public AudioControlTLV320AIC3206, public OSCAudioBase
{
    public:
        OSCAudioControlTLV320AIC3206(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/aic_r*","ii")) {addReplyResult(msg,addrOff,reply,(uint32_t)aic_readPage(msg.getInt(0),msg.getInt(1))); } // unsigned int aic_readPage(uint8_t page, uint8_t reg);
            else if (isTarget(msg,addrOff,"/aic_w*","iii")) {addReplyResult(msg,addrOff,reply,aic_writePage(msg.getInt(0),msg.getInt(1),msg.getInt(2))); } // bool aic_writePage(uint8_t page, uint8_t reg, uint8_t val);
            else if (isTarget(msg,addrOff,"/d*",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void);
            else if (isTarget(msg,addrOff,"/enableA*",";i")) {addReplyResult(msg,addrOff,reply,enableAutoMuteDAC(msg.getBoolean(0),msg.getInt(1))); } // bool enableAutoMuteDAC(bool, uint8_t);
            else if (isTarget(msg,addrOff,"/enableM*",";")) {addReplyResult(msg,addrOff,reply,enableMicDetect(msg.getBoolean(0))); } // bool enableMicDetect(bool);
            else if (isTarget(msg,addrOff,"/enable",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);
            else if (isTarget(msg,addrOff,"/getH*",NULL)) {addReplyResult(msg,addrOff,reply,getHPCutoff_Hz()); } // float getHPCutoff_Hz(void) { return HP_cutoff_Hz; }
            else if (isTarget(msg,addrOff,"/getS*",NULL)) {addReplyResult(msg,addrOff,reply,getSampleRate_Hz()); } // float getSampleRate_Hz(void) { return sample_rate_Hz; }
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float n);  //dummy to be compatible with Teensy Audio Library
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n);   //use AIC3206_INPUT_IN1 or one of other choices defined earlier
            else if (isTarget(msg,addrOff,"/o*","i")) {addReplyResult(msg,addrOff,reply,outputSelect(msg.getInt(0))); } // bool outputSelect(int n);  //use AIC3206_OUTPUT_HEADPHONE_JACK_OUT or one of other choices defined earlier
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,(int32_t)readMicDetect()); } // int  readMicDetect(void);
            else if (isTarget(msg,addrOff,"/setH*",";ff")) {setHPFonADC(msg.getBoolean(0),msg.getFloat(1),msg.getFloat(2)); addReplyExecuted(msg,addrOff,reply);} // void setHPFonADC(bool enable, float cutoff_Hz, float fs_Hz);
            else if (isTarget(msg,addrOff,"/setII*","ib")) {setIIRCoeffOnADC(msg); addReplyExecuted(msg,addrOff,reply);} // void setIIRCoeffOnADC(int chan, uint32_t *coeff);  //for chan, use AIC3206_BOTH_CHAN or AIC3206_LEFT_CHAN or AIC3206_RIGHT_CHAN
            else if (isTarget(msg,addrOff,"/setIn*","f")) {addReplyResult(msg,addrOff,reply,setInputGain_dB(msg.getFloat(0))); } // bool setInputGain_dB(float n);
            else if (isTarget(msg,addrOff,"/setM*","i")) {addReplyResult(msg,addrOff,reply,setMicBias(msg.getInt(0))); } // bool setMicBias(int n);  //use AIC3206_MIC_BIAS_OFF or AIC3206_MIC_BIAS_2_5 or one of other choices defined earlier
            else if (isTarget(msg,addrOff,"/u*","i")) {addReplyResult(msg,addrOff,reply,updateInputBasedOnMicDetect(msg.getInt(0))); } // bool updateInputBasedOnMicDetect(int setting = AIC3206_INPUT_IN1); //which input to monitor
            else if (isTarget(msg,addrOff,"/volume_*","f")) {addReplyResult(msg,addrOff,reply,volume_dB(msg.getFloat(0))); } // bool volume_dB(float n);
            else if (isTarget(msg,addrOff,"/volume","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float n);
			}
		}
	private:
		void setIIRCoeffOnADC(OSCMessage& msg);
};

// ============== AudioControlWM8731 ====================
class OSCAudioControlWM8731 : public AudioControlWM8731, public OSCAudioBase
{
    public:
        OSCAudioControlWM8731(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*",NULL)) {addReplyResult(msg,addrOff,reply,disable()); } // bool disable(void) { return false; }
            else if (isTarget(msg,addrOff,"/e*",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);
            else if (isTarget(msg,addrOff,"/inputL*","f")) {addReplyResult(msg,addrOff,reply,inputLevel(msg.getFloat(0))); } // bool inputLevel(float n); // range: 0.0f to 1.0f
            else if (isTarget(msg,addrOff,"/inputS*","i")) {addReplyResult(msg,addrOff,reply,inputSelect(msg.getInt(0))); } // bool inputSelect(int n);
            else if (isTarget(msg,addrOff,"/v*","f")) {addReplyResult(msg,addrOff,reply,volume(msg.getFloat(0))); } // bool volume(float n) { return volumeInteger(n * 80.0f + 47.499f); }
			}
		}
};

// ============== AudioControlWM8731master ====================
class OSCAudioControlWM8731master : public AudioControlWM8731master, public OSCAudioBase
{
    public:
        OSCAudioControlWM8731master(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/e*",NULL)) {addReplyResult(msg,addrOff,reply,enable()); } // bool enable(void);
			}
		}
};

// ============== AudioEffectBitcrusher ====================
class OSCAudioEffectBitcrusher : public AudioEffectBitcrusher, public OSCAudioBase
{
    public:
        OSCAudioEffectBitcrusher(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/b*","i")) {bits(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void bits(uint8_t b) {
            else if (isTarget(msg,addrOff,"/s*","f")) {sampleRate(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void sampleRate(float hz) {
			}
		}
};

// ============== AudioEffectDelay ====================
class OSCAudioEffectDelay : public AudioEffectDelay, public OSCAudioBase
{
    public:
        OSCAudioEffectDelay(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/de*","if")) {delay(msg.getInt(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void delay(uint8_t channel, float milliseconds) {
            else if (isTarget(msg,addrOff,"/di*","i")) {disable(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void disable(uint8_t channel) {
			}
		}
};

// ============== AudioEffectDelayExternal ====================
class OSCAudioEffectDelayExternal : public AudioEffectDelayExternal, public OSCAudioBase
{
    public:
        OSCAudioEffectDelayExternal(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/de*","if")) {delay(msg.getInt(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void delay(uint8_t channel, float milliseconds) {
            else if (isTarget(msg,addrOff,"/di*","i")) {disable(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void disable(uint8_t channel) {
			}
		}
};

// ============== AudioEffectDigitalCombine ====================
class OSCAudioEffectDigitalCombine : public AudioEffectDigitalCombine, public OSCAudioBase
{
    public:
        OSCAudioEffectDigitalCombine(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/s*","i")) {setCombineMode(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void setCombineMode(int mode_in) {
			}
		}
};

// ============== AudioEffectEnvelope ====================
class OSCAudioEffectEnvelope : public AudioEffectEnvelope, public OSCAudioBase
{
    public:
        OSCAudioEffectEnvelope(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {attack(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void attack(float milliseconds) {
            else if (isTarget(msg,addrOff,"/dec*","f")) {decay(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void decay(float milliseconds) {
            else if (isTarget(msg,addrOff,"/del*","f")) {delay(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void delay(float milliseconds) {
            else if (isTarget(msg,addrOff,"/h*","f")) {hold(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void hold(float milliseconds) {
            else if (isTarget(msg,addrOff,"/isA*",NULL)) {addReplyResult(msg,addrOff,reply,isActive()); } // bool isActive();
            else if (isTarget(msg,addrOff,"/isS*",NULL)) {addReplyResult(msg,addrOff,reply,isSustain()); } // bool isSustain();
            else if (isTarget(msg,addrOff,"/noteOf*",NULL)) {noteOff(); addReplyExecuted(msg,addrOff,reply);} // void noteOff();
            else if (isTarget(msg,addrOff,"/noteOn",NULL)) {noteOn(); addReplyExecuted(msg,addrOff,reply);} // void noteOn();
            else if (isTarget(msg,addrOff,"/releaseN*","f")) {releaseNoteOn(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void releaseNoteOn(float milliseconds) {
            else if (isTarget(msg,addrOff,"/release","f")) {release(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void release(float milliseconds) {
            else if (isTarget(msg,addrOff,"/s*","f")) {sustain(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void sustain(float level) {
			}
		}
};

// ============== AudioEffectFade ====================
class OSCAudioEffectFade : public AudioEffectFade, public OSCAudioBase
{
    public:
        OSCAudioEffectFade(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/fadeI*","i")) {fadeIn(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void fadeIn(uint32_t milliseconds) {
            else if (isTarget(msg,addrOff,"/fadeO*","i")) {fadeOut(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void fadeOut(uint32_t milliseconds) {
			}
		}
};

// ============== AudioEffectFreeverb ====================
class OSCAudioEffectFreeverb : public AudioEffectFreeverb, public OSCAudioBase
{
    public:
        OSCAudioEffectFreeverb(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*","f")) {damping(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void damping(float n) {
            else if (isTarget(msg,addrOff,"/r*","f")) {roomsize(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void roomsize(float n) {
			}
		}
};

// ============== AudioEffectFreeverbStereo ====================
class OSCAudioEffectFreeverbStereo : public AudioEffectFreeverbStereo, public OSCAudioBase
{
    public:
        OSCAudioEffectFreeverbStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*","f")) {damping(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void damping(float n) {
            else if (isTarget(msg,addrOff,"/r*","f")) {roomsize(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void roomsize(float n) {
			}
		}
};

// ============== AudioEffectGranular ====================
class OSCAudioEffectGranular : public AudioEffectGranular, public OSCAudioBase
{
    public:
        OSCAudioEffectGranular(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this), sample_bank(0) {}
        ~OSCAudioEffectGranular() { free(sample_bank);} 

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/beginF*","f")) {beginFreeze(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void beginFreeze(float grain_length) {
            else if (isTarget(msg,addrOff,"/beginP*","f")) {beginPitchShift(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void beginPitchShift(float grain_length) {
            else if (isTarget(msg,addrOff,"/begin","bi")) {begin(msg); addReplyExecuted(msg,addrOff,reply);} // void begin(int16_t *sample_bank_def, int16_t max_len_def);
            else if (isTarget(msg,addrOff,"/se*","f")) {setSpeed(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void setSpeed(float ratio) {
            else if (isTarget(msg,addrOff,"/st*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop();
			}
		}
	private:
		void begin(OSCMessage& msg);
		int16_t* sample_bank;
};

// ============== AudioEffectMidSide ====================
class OSCAudioEffectMidSide : public AudioEffectMidSide, public OSCAudioBase
{
    public:
        OSCAudioEffectMidSide(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/d*",NULL)) {decode(); addReplyExecuted(msg,addrOff,reply);} // void decode() { encoding = false; }
            else if (isTarget(msg,addrOff,"/e*",NULL)) {encode(); addReplyExecuted(msg,addrOff,reply);} // void encode() { encoding = true; }
			}
		}
};

// ============== AudioEffectMultiply ====================
class OSCAudioEffectMultiply : public AudioEffectMultiply, public OSCAudioBase
{
    public:
        OSCAudioEffectMultiply(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioEffectRectifier ====================
class OSCAudioEffectRectifier : public AudioEffectRectifier, public OSCAudioBase
{
    public:
        OSCAudioEffectRectifier(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioEffectReverb ====================
class OSCAudioEffectReverb : public AudioEffectReverb, public OSCAudioBase
{
    public:
        OSCAudioEffectReverb(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/r*","f")) {reverbTime(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void reverbTime(float);
			}
		}
};

// ============== AudioEffectWaveFolder ====================
class OSCAudioEffectWaveFolder : public AudioEffectWaveFolder, public OSCAudioBase
{
    public:
        OSCAudioEffectWaveFolder(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioEffectWaveshaper ====================
class OSCAudioEffectWaveshaper : public AudioEffectWaveshaper, public OSCAudioBase
{
    public:
        OSCAudioEffectWaveshaper(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            // if (isTarget(msg,addrOff,"/s*","bi")) {shape(msg.getBlob(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void shape(float* waveshape, int length);
			}
		}
};

// ============== AudioFilterBiquad ====================
class OSCAudioFilterBiquad : public AudioFilterBiquad, public OSCAudioBase
{
    public:
        OSCAudioFilterBiquad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/setB*","iff")) {setBandpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2)); addReplyExecuted(msg,addrOff,reply);} // void setBandpass(uint32_t stage, float frequency, float q = 1.0) {
            else if (isTarget(msg,addrOff,"/setC*","iddddd")) {setCoefficients(msg); addReplyExecuted(msg,addrOff,reply);} // void setCoefficients(uint32_t stage, const double *coefficients) {
            else if (isTarget(msg,addrOff,"/setC*","iiiiii")) {setCoefficients(msg); addReplyExecuted(msg,addrOff,reply);} // void setCoefficients(uint32_t stage, const int *coefficients);
            else if (isTarget(msg,addrOff,"/setHighS*","ifff")) {setHighShelf(msg.getInt(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3)); addReplyExecuted(msg,addrOff,reply);} // void setHighShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
            else if (isTarget(msg,addrOff,"/setHighp*","iff")) {setHighpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2)); addReplyExecuted(msg,addrOff,reply);} // void setHighpass(uint32_t stage, float frequency, float q = 0.7071) {
            else if (isTarget(msg,addrOff,"/setLowS*","ifff")) {setLowShelf(msg.getInt(0),msg.getFloat(1),msg.getFloat(2),msg.getFloat(3)); addReplyExecuted(msg,addrOff,reply);} // void setLowShelf(uint32_t stage, float frequency, float gain, float slope = 1.0f) {
            else if (isTarget(msg,addrOff,"/setLowp*","iff")) {setLowpass(msg.getInt(0),msg.getFloat(1),msg.getFloat(2)); addReplyExecuted(msg,addrOff,reply);} // void setLowpass(uint32_t stage, float frequency, float q = 0.7071f) {
            else if (isTarget(msg,addrOff,"/setN*","iff")) {setNotch(msg.getInt(0),msg.getFloat(1),msg.getFloat(2)); addReplyExecuted(msg,addrOff,reply);} // void setNotch(uint32_t stage, float frequency, float q = 1.0) {
			}
		}
	private:
		void setCoefficients(OSCMessage& msg);
};

// ============== AudioFilterFIR ====================
class OSCAudioFilterFIR : public AudioFilterFIR, public OSCAudioBase
{
    public:
        OSCAudioFilterFIR(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            // if (isTarget(msg,addrOff,"/b*","bi")) {begin(msg.getBlob(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void begin(const short *cp, int n_coeffs) {
            if (isTarget(msg,addrOff,"/e*",NULL)) {end(); addReplyExecuted(msg,addrOff,reply);} // void end(void) {
			}
		}
};

// ============== AudioFilterLadder ====================
class OSCAudioFilterLadder : public AudioFilterLadder, public OSCAudioBase
{
    public:
        OSCAudioFilterLadder(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float FC);
            else if (isTarget(msg,addrOff,"/inp*","f")) {inputDrive(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void inputDrive(float drv);
            else if (isTarget(msg,addrOff,"/int*","i")) {interpolationMethod((AudioFilterLadderInterpolation) msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void interpolationMethod(AudioFilterLadderInterpolation im);
            else if (isTarget(msg,addrOff,"/o*","f")) {octaveControl(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void octaveControl(float octaves);
            else if (isTarget(msg,addrOff,"/p*","f")) {passbandGain(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void passbandGain(float passbandgain);
            else if (isTarget(msg,addrOff,"/r*","f")) {resonance(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void resonance(float reson);
			}
		}
};

// ============== AudioFilterStateVariable ====================
class OSCAudioFilterStateVariable : public AudioFilterStateVariable, public OSCAudioBase
{
    public:
        OSCAudioFilterStateVariable(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/o*","f")) {octaveControl(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void octaveControl(float n) {
            else if (isTarget(msg,addrOff,"/r*","f")) {resonance(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void resonance(float q) {
			}
		}
};

// ============== AudioInputAnalog ====================
class OSCAudioInputAnalog : public AudioInputAnalog, public OSCAudioBase
{
    public:
        OSCAudioInputAnalog(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputAnalogStereo ====================
class OSCAudioInputAnalogStereo : public AudioInputAnalogStereo, public OSCAudioBase
{
    public:
        OSCAudioInputAnalogStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputI2S ====================
class OSCAudioInputI2S : public AudioInputI2S, public OSCAudioBase
{
    public:
        OSCAudioInputI2S(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputI2S2 ====================
class OSCAudioInputI2S2 : public AudioInputI2S2, public OSCAudioBase
{
    public:
        OSCAudioInputI2S2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputI2SHex ====================
class OSCAudioInputI2SHex : public AudioInputI2SHex, public OSCAudioBase
{
    public:
        OSCAudioInputI2SHex(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputI2SOct ====================
class OSCAudioInputI2SOct : public AudioInputI2SOct, public OSCAudioBase
{
    public:
        OSCAudioInputI2SOct(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputI2SQuad ====================
class OSCAudioInputI2SQuad : public AudioInputI2SQuad, public OSCAudioBase
{
    public:
        OSCAudioInputI2SQuad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputPDM ====================
class OSCAudioInputPDM : public AudioInputPDM, public OSCAudioBase
{
    public:
        OSCAudioInputPDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputPDM2 ====================
class OSCAudioInputPDM2 : public AudioInputPDM2, public OSCAudioBase
{
    public:
        OSCAudioInputPDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputSPDIF3 ====================
class OSCAudioInputSPDIF3 : public AudioInputSPDIF3, public OSCAudioBase
{
    public:
        OSCAudioInputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/p*",NULL)) {addReplyResult(msg,addrOff,reply,pllLocked()); } // static bool pllLocked(void);
            else if (isTarget(msg,addrOff,"/s*",NULL)) {addReplyResult(msg,addrOff,reply,(uint32_t)sampleRate()); } // static unsigned int sampleRate(void);
			}
		}
};

// ============== AudioInputTDM ====================
class OSCAudioInputTDM : public AudioInputTDM, public OSCAudioBase
{
    public:
        OSCAudioInputTDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioInputTDM2 ====================
class OSCAudioInputTDM2 : public AudioInputTDM2, public OSCAudioBase
{
    public:
        OSCAudioInputTDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioMixer4 ====================
class OSCAudioMixer4 : public AudioMixer4, public OSCAudioBase
{
    public:
        OSCAudioMixer4(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/g*","if")) {gain(msg.getInt(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void gain(unsigned int channel, float gain) {
			}
		}
};

// ============== AudioOutputADAT ====================
class OSCAudioOutputADAT : public AudioOutputADAT, public OSCAudioBase
{
    public:
        OSCAudioOutputADAT(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/m*",";")) {mute_PCM(msg.getBoolean(0)); addReplyExecuted(msg,addrOff,reply);} // static void mute_PCM(const bool mute);
			}
		}
};

// ============== AudioOutputAnalog ====================
class OSCAudioOutputAnalog : public AudioOutputAnalog, public OSCAudioBase
{
    public:
        OSCAudioOutputAnalog(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","i")) {analogReference(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void analogReference(int ref);
			}
		}
};

// ============== AudioOutputAnalogStereo ====================
class OSCAudioOutputAnalogStereo : public AudioOutputAnalogStereo, public OSCAudioBase
{
    public:
        OSCAudioOutputAnalogStereo(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","i")) {analogReference(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void analogReference(int ref);
			}
		}
};

// ============== AudioOutputI2S ====================
class OSCAudioOutputI2S : public AudioOutputI2S, public OSCAudioBase
{
    public:
        OSCAudioOutputI2S(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputI2S2 ====================
class OSCAudioOutputI2S2 : public AudioOutputI2S2, public OSCAudioBase
{
    public:
        OSCAudioOutputI2S2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputI2SHex ====================
class OSCAudioOutputI2SHex : public AudioOutputI2SHex, public OSCAudioBase
{
    public:
        OSCAudioOutputI2SHex(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputI2SOct ====================
class OSCAudioOutputI2SOct : public AudioOutputI2SOct, public OSCAudioBase
{
    public:
        OSCAudioOutputI2SOct(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputI2SQuad ====================
class OSCAudioOutputI2SQuad : public AudioOutputI2SQuad, public OSCAudioBase
{
    public:
        OSCAudioOutputI2SQuad(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputMQS ====================
class OSCAudioOutputMQS : public AudioOutputMQS, public OSCAudioBase
{
    public:
        OSCAudioOutputMQS(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputPT8211 ====================
class OSCAudioOutputPT8211 : public AudioOutputPT8211, public OSCAudioBase
{
    public:
        OSCAudioOutputPT8211(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputPT8211_2 ====================
class OSCAudioOutputPT8211_2 : public AudioOutputPT8211_2, public OSCAudioBase
{
    public:
        OSCAudioOutputPT8211_2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputPWM ====================
class OSCAudioOutputPWM : public AudioOutputPWM, public OSCAudioBase
{
    public:
        OSCAudioOutputPWM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputSPDIF ====================
class OSCAudioOutputSPDIF : public AudioOutputSPDIF, public OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/m*",";")) {mute_PCM(msg.getBoolean(0)); addReplyExecuted(msg,addrOff,reply);} // static void mute_PCM(const bool mute);
			}
		}
};

// ============== AudioOutputSPDIF2 ====================
class OSCAudioOutputSPDIF2 : public AudioOutputSPDIF2, public OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/m*",";")) {mute_PCM(msg.getBoolean(0)); addReplyExecuted(msg,addrOff,reply);} // static void mute_PCM(const bool mute);
			}
		}
};

// ============== AudioOutputSPDIF3 ====================
class OSCAudioOutputSPDIF3 : public AudioOutputSPDIF3, public OSCAudioBase
{
    public:
        OSCAudioOutputSPDIF3(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/m*",";")) {mute_PCM(msg.getBoolean(0)); addReplyExecuted(msg,addrOff,reply);} // static void mute_PCM(const bool mute);
            // NOT DEFINED: else if (isTarget(msg,addrOff,"/p*",NULL)) {addReplyResult(msg,addrOff,reply,pll_locked()); } // static bool pll_locked(void);
			}
		}
};

// ============== AudioOutputTDM ====================
class OSCAudioOutputTDM : public AudioOutputTDM, public OSCAudioBase
{
    public:
        OSCAudioOutputTDM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioOutputTDM2 ====================
class OSCAudioOutputTDM2 : public AudioOutputTDM2, public OSCAudioBase
{
    public:
        OSCAudioOutputTDM2(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
			}
		}
};

// ============== AudioPlayMemory ====================
class OSCAudioPlayMemory : public AudioPlayMemory, public OSCAudioBase
{
    public:
        OSCAudioPlayMemory(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,isPlaying()); } // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addrOff,"/l*",NULL)) {addReplyResult(msg,addrOff,reply,lengthMillis()); } // uint32_t lengthMillis(void);
            else if (isTarget(msg,addrOff,"/pl*","i")) {play(msg); addReplyExecuted(msg,addrOff,reply);} // void play(const unsigned int *data);
            else if (isTarget(msg,addrOff,"/po*",NULL)) {addReplyResult(msg,addrOff,reply,positionMillis()); } // uint32_t positionMillis(void);
            else if (isTarget(msg,addrOff,"/s*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
			}
		}
	private:
		void play(OSCMessage& msg);
};

// ============== AudioPlayQueue ====================
class OSCAudioPlayQueue : public AudioPlayQueue, public OSCAudioBase
{
    public:
        OSCAudioPlayQueue(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,available()); } // bool available(void);
            else if (isTarget(msg,addrOff,"/g*",NULL)) {addReplyResult(msg,addrOff,reply,(uint32_t)getBuffer()); } // int16_t * getBuffer(void);
            else if (isTarget(msg,addrOff,"/playB*",NULL)) {playBuffer(); addReplyExecuted(msg,addrOff,reply);} // void playBuffer(void);
            // else if (isTarget(msg,addrOff,"/play","bi")) {play(msg.getBlob(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void play(const int16_t *data, uint32_t len);
            else if (isTarget(msg,addrOff,"/play","i")) {play(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void play(int16_t data);
            else if (isTarget(msg,addrOff,"/se*","i")) {setMaxBuffers(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void setMaxBuffers(uint8_t);
            // NOT DEFINED: else if (isTarget(msg,addrOff,"/st*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
			}
		}
};

// ============== AudioPlaySdRaw ====================
class OSCAudioPlaySdRaw : public AudioPlaySdRaw, public OSCAudioBase
{
    public:
        OSCAudioPlaySdRaw(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/b*",NULL)) {begin(); addReplyExecuted(msg,addrOff,reply);} // void begin(void);
            else if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,isPlaying()); } // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addrOff,"/l*",NULL)) {addReplyResult(msg,addrOff,reply,lengthMillis()); } // uint32_t lengthMillis(void);
            else if (isTarget(msg,addrOff,"/pl*","s")) {play(msg); addReplyExecuted(msg,addrOff,reply);} // bool play(const char *filename);
            else if (isTarget(msg,addrOff,"/po*",NULL)) {addReplyResult(msg,addrOff,reply,positionMillis()); } // uint32_t positionMillis(void);
            else if (isTarget(msg,addrOff,"/s*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
			}
		}
	private:
		void play(OSCMessage& msg);
};

// ============== AudioPlaySdWav ====================
class OSCAudioPlaySdWav : public AudioPlaySdWav, public OSCAudioBase
{
    public:
        OSCAudioPlaySdWav(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/b*",NULL)) {begin(); addReplyExecuted(msg,addrOff,reply);} // void begin(void);
            else if (isTarget(msg,addrOff,"/isPa*",NULL)) {addReplyResult(msg,addrOff,reply,isPaused()); } // bool isPaused(void);
            else if (isTarget(msg,addrOff,"/isPl*",NULL)) {addReplyResult(msg,addrOff,reply,isPlaying()); } // bool isPlaying(void);
            else if (isTarget(msg,addrOff,"/isS*",NULL)) {addReplyResult(msg,addrOff,reply,isStopped()); } // bool isStopped(void);
            else if (isTarget(msg,addrOff,"/l*",NULL)) {addReplyResult(msg,addrOff,reply,lengthMillis()); } // uint32_t lengthMillis(void);
            else if (isTarget(msg,addrOff,"/pl*","s")) {play(msg); addReplyExecuted(msg,addrOff,reply);} // bool play(const char *filename);
            else if (isTarget(msg,addrOff,"/po*",NULL)) {addReplyResult(msg,addrOff,reply,positionMillis()); } // uint32_t positionMillis(void);
            else if (isTarget(msg,addrOff,"/s*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
            else if (isTarget(msg,addrOff,"/t*",NULL)) {togglePlayPause(); addReplyExecuted(msg,addrOff,reply);} // void togglePlayPause(void);
			}
		}
	private:
		void play(OSCMessage& msg);
};

// ============== AudioPlaySerialflashRaw ====================
class OSCAudioPlaySerialflashRaw : public AudioPlaySerialflashRaw, public OSCAudioBase
{
    public:
        OSCAudioPlaySerialflashRaw(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/b*",NULL)) {begin(); addReplyExecuted(msg,addrOff,reply);} // void begin(void);
            else if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,isPlaying()); } // bool isPlaying(void) { return playing; }
            else if (isTarget(msg,addrOff,"/l*",NULL)) {addReplyResult(msg,addrOff,reply,lengthMillis()); } // uint32_t lengthMillis(void);
            else if (isTarget(msg,addrOff,"/pl*","s")) {play(msg); addReplyExecuted(msg,addrOff,reply);} // bool play(const char *filename);
            else if (isTarget(msg,addrOff,"/po*",NULL)) {addReplyResult(msg,addrOff,reply,positionMillis()); } // uint32_t positionMillis(void);
            else if (isTarget(msg,addrOff,"/s*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
			}
		}
	private:
		void play(OSCMessage& msg);
};

// ============== AudioRecordQueue ====================
class OSCAudioRecordQueue : public AudioRecordQueue, public OSCAudioBase
{
    public:
        OSCAudioRecordQueue(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*",NULL)) {addReplyResult(msg,addrOff,reply,(int32_t)available()); } // int available(void);
            else if (isTarget(msg,addrOff,"/b*",NULL)) {begin(); addReplyExecuted(msg,addrOff,reply);} // void begin(void) {
            else if (isTarget(msg,addrOff,"/c*",NULL)) {clear(); addReplyExecuted(msg,addrOff,reply);} // void clear(void);
            else if (isTarget(msg,addrOff,"/e*",NULL)) {end(); addReplyExecuted(msg,addrOff,reply);} // void end(void) {
            else if (isTarget(msg,addrOff,"/f*",NULL)) {freeBuffer(); addReplyExecuted(msg,addrOff,reply);} // void freeBuffer(void);
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,(uint32_t)readBuffer()); } // int16_t * readBuffer(void);
			}
		}
};

// ============== AudioSynthKarplusStrong ====================
class OSCAudioSynthKarplusStrong : public AudioSynthKarplusStrong, public OSCAudioBase
{
    public:
        OSCAudioSynthKarplusStrong(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/noteOf*","f")) {noteOff(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void noteOff(float velocity) {
            else if (isTarget(msg,addrOff,"/noteOn","ff")) {noteOn(msg.getFloat(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void noteOn(float frequency, float velocity) {
			}
		}
};

// ============== AudioSynthNoisePink ====================
class OSCAudioSynthNoisePink : public AudioSynthNoisePink, public OSCAudioBase
{
    public:
        OSCAudioSynthNoisePink(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
			}
		}
};

// ============== AudioSynthNoiseWhite ====================
class OSCAudioSynthNoiseWhite : public AudioSynthNoiseWhite, public OSCAudioBase
{
    public:
        OSCAudioSynthNoiseWhite(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
			}
		}
};

// ============== AudioSynthSimpleDrum ====================
class OSCAudioSynthSimpleDrum : public AudioSynthSimpleDrum, public OSCAudioBase
{
    public:
        OSCAudioSynthSimpleDrum(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq)
            else if (isTarget(msg,addrOff,"/l*","i")) {length(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void length(int32_t milliseconds)
            else if (isTarget(msg,addrOff,"/n*",NULL)) {noteOn(); addReplyExecuted(msg,addrOff,reply);} // void noteOn();
            else if (isTarget(msg,addrOff,"/p*","f")) {pitchMod(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void pitchMod(float depth);
            else if (isTarget(msg,addrOff,"/s*","f")) {secondMix(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void secondMix(float level);
			}
		}
};

// ============== AudioSynthToneSweep ====================
class OSCAudioSynthToneSweep : public AudioSynthToneSweep, public OSCAudioBase
{
    public:
        OSCAudioSynthToneSweep(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,(uint8_t)isPlaying()); } // unsigned char isPlaying(void);
            else if (isTarget(msg,addrOff,"/p*","fiif")) {addReplyResult(msg,addrOff,reply,(bool)play(msg.getFloat(0),msg.getInt(1),msg.getInt(2),msg.getFloat(3))); } // boolean play(float t_amp,int t_lo,int t_hi,float t_time);
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read(void) {
			}
		}
};

// ============== AudioSynthWaveform ====================
class OSCAudioSynthWaveform : public AudioSynthWaveform, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveform(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this), arbdata(0) {}
        ~OSCAudioSynthWaveform() { free(arbdata);} 

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/am*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {	// 0 to 1.0
            else if (isTarget(msg,addrOff,"/ar*","bf")) {addReplyResult(msg,addrOff,reply,arbitraryWaveform(msg)); } // void arbitraryWaveform(const int16_t *data, float maxFreq) {
            else if (isTarget(msg,addrOff,"/b*","ffi")) {begin(msg.getFloat(0),msg.getFloat(1),msg.getInt(2)); addReplyExecuted(msg,addrOff,reply);} // void begin(float t_amp, float t_freq, short t_type) {
            else if (isTarget(msg,addrOff,"/b*","i")) {begin(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void begin(short t_type) {
            else if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/o*","f")) {offset(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void offset(float n) {
            else if (isTarget(msg,addrOff,"/ph*","f")) {phase(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void phase(float angle) {
            else if (isTarget(msg,addrOff,"/pu*","f")) {pulseWidth(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void pulseWidth(float n) {	// 0.0 to 1.0
			}
		}
	private:
		bool arbitraryWaveform(OSCMessage& msg);
		int16_t* arbdata;
};

// ============== AudioSynthWaveformDc ====================
class OSCAudioSynthWaveformDc : public AudioSynthWaveformDc, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformDc(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
            else if (isTarget(msg,addrOff,"/a*","ff")) {amplitude(msg.getFloat(0),msg.getFloat(1)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n, float milliseconds) {
            else if (isTarget(msg,addrOff,"/r*",NULL)) {addReplyResult(msg,addrOff,reply,read()); } // float read(void) {
			}
		}
};

// ============== AudioSynthWaveformModulated ====================
class OSCAudioSynthWaveformModulated : public AudioSynthWaveformModulated, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformModulated(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this), arbdata(0) {}
        ~OSCAudioSynthWaveformModulated() { free(arbdata);} 

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/am*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {	// 0 to 1.0
            else if (isTarget(msg,addrOff,"/ar*","bf")) {addReplyResult(msg,addrOff,reply,arbitraryWaveform(msg)); } // void arbitraryWaveform(const int16_t *data, float maxFreq) {
            else if (isTarget(msg,addrOff,"/b*","ffi")) {begin(msg.getFloat(0),msg.getFloat(1),msg.getInt(2)); addReplyExecuted(msg,addrOff,reply);} // void begin(float t_amp, float t_freq, short t_type) {
            else if (isTarget(msg,addrOff,"/b*","i")) {begin(msg.getInt(0)); addReplyExecuted(msg,addrOff,reply);} // void begin(short t_type) {
            else if (isTarget(msg,addrOff,"/frequencyM*","f")) {frequencyModulation(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequencyModulation(float octaves) {
            else if (isTarget(msg,addrOff,"/frequency","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/o*","f")) {offset(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void offset(float n) {
            else if (isTarget(msg,addrOff,"/p*","f")) {phaseModulation(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void phaseModulation(float degrees) {
			}
		}
	private:
		bool arbitraryWaveform(OSCMessage& msg);
		int16_t* arbdata;
};

// ============== AudioSynthWaveformPWM ====================
class OSCAudioSynthWaveformPWM : public AudioSynthWaveformPWM, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformPWM(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
            else if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
			}
		}
};

// ============== AudioSynthWaveformSine ====================
class OSCAudioSynthWaveformSine : public AudioSynthWaveformSine, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSine(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
            else if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/p*","f")) {phase(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void phase(float angle) {
			}
		}
};

// ============== AudioSynthWaveformSineHires ====================
class OSCAudioSynthWaveformSineHires : public AudioSynthWaveformSineHires, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSineHires(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
            else if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/p*","f")) {phase(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void phase(float angle) {
			}
		}
};

// ============== AudioSynthWaveformSineModulated ====================
class OSCAudioSynthWaveformSineModulated : public AudioSynthWaveformSineModulated, public OSCAudioBase
{
    public:
        OSCAudioSynthWaveformSineModulated(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float n) {
            else if (isTarget(msg,addrOff,"/f*","f")) {frequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void frequency(float freq) {
            else if (isTarget(msg,addrOff,"/p*","f")) {phase(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void phase(float angle) {
			}
		}
};

// ============== AudioSynthWavetable ====================
class OSCAudioSynthWavetable : public AudioSynthWavetable, public OSCAudioBase
{
    public:
        OSCAudioSynthWavetable(const char* _name) :  OSCAudioBase(_name, (AudioStream*) this) {}

        void route(OSCMessage& msg, int addrOff, OSCBundle& reply)
        {
          int nameOff;
          if ((nameOff = isMine(msg,addrOff)) > 0)
          {
            addrOff += nameOff;
            if (isTarget(msg,addrOff,"/a*","f")) {amplitude(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void amplitude(float v) {
            else if (isTarget(msg,addrOff,"/f*","f")) {addReplyResult(msg,addrOff,reply,(int32_t)freqToNote(msg.getFloat(0))); } // static int freqToNote(float freq) {
            else if (isTarget(msg,addrOff,"/g*",NULL)) {addReplyResult(msg,addrOff,reply,(uint8_t)getEnvState()); } // envelopeStateEnum getEnvState(void) { return env_state; }
            else if (isTarget(msg,addrOff,"/i*",NULL)) {addReplyResult(msg,addrOff,reply,isPlaying()); } // bool isPlaying(void) { return env_state != STATE_IDLE; }
            else if (isTarget(msg,addrOff,"/m*","i")) {addReplyResult(msg,addrOff,reply,midi_volume_transform(msg.getInt(0))); } // static float midi_volume_transform(int midi_amp) {
            else if (isTarget(msg,addrOff,"/n*","i")) {addReplyResult(msg,addrOff,reply,noteToFreq(msg.getInt(0))); } // static float noteToFreq(int note) {
            else if (isTarget(msg,addrOff,"/playF*","fi")) {playFrequency(msg.getFloat(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void playFrequency(float freq, int amp = DEFAULT_AMPLITUDE);
            else if (isTarget(msg,addrOff,"/playN*","ii")) {playNote(msg.getInt(0),msg.getInt(1)); addReplyExecuted(msg,addrOff,reply);} // void playNote(int note, int amp = DEFAULT_AMPLITUDE);
            else if (isTarget(msg,addrOff,"/setF*","f")) {setFrequency(msg.getFloat(0)); addReplyExecuted(msg,addrOff,reply);} // void setFrequency(float freq);
            // else if (isTarget(msg,addrOff,"/setI*","b")) {setInstrument(msg.getBlob(0)); addReplyExecuted(msg,addrOff,reply);} // void setInstrument(const instrument_data& instrument) {
            else if (isTarget(msg,addrOff,"/st*",NULL)) {stop(); addReplyExecuted(msg,addrOff,reply);} // void stop(void);
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


