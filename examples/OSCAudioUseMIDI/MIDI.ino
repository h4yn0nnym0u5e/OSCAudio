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

// Demo code fragment to illustrate using OSC messages in conjunction with MIDI

#include <OSCBundle.h>
#include "OSCAudioUseMIDI.h"

// one octave of well-tempered frequencies, starting at MIDI note 60 (middle C)
float wellTempered[] = {261.62557, 277.18263, 293.66477, 311.12698, 329.62756, 349.22823, 369.99442, 391.99544, 415.3047, 440, 466.16376, 493.8833};

float note2freq(byte note,float* scale)
{
  float freq;
  byte idx = note % 12;
  int octave;
  
  freq = scale[idx];
  octave = (note - idx - 60)/12; // figure out octave
  if (octave > 0)
    freq *= (float)(1<<octave);
  else if (octave < 0)
    freq /= (float)(1<<(-octave));

  return freq;
}

void dbgMIDI(byte ch, byte typ, byte d1, byte d2)
{
    Serial.printf("%02X: %02X,%02X,%02X\n",
                  ch,typ,d1,d2);
}

void dbgMIDIget(void)
{
    dbgMIDI(usbMIDI.getChannel(),
            usbMIDI.getType(),
            usbMIDI.getData1(),
            usbMIDI.getData2());
}

void OSCnoteOn(byte channel, byte note, byte velocity)
{
  float freq;
  OSCBundle bndl;
  OSCBundle reply;
  
  Serial.print("Note on: ");
  freq = note2freq(note,wellTempered);
  Serial.println(freq);
  dbgMIDI(channel,0x80,note,velocity);  

  bndl.add("/teensy1/audio/waveform1/be").add(velocity/127.0f).add(freq).add(2);
  bndl.add("/teensy1/audio/env/noteOn");
  //bndl.send(Serial); // for debug!
  reply.add("/noteOn");
  processBundle(&bndl,reply);
}


void OSCnoteOff(byte channel, byte note, byte velocity)
{
  OSCBundle bndl;
  OSCBundle reply;

  Serial.print("Note off: ");
  dbgMIDI(channel,0x90,note,velocity);    
  
  bndl.add("/teensy1/audio/env/noteOff");
  reply.add("/noteOff");
  processBundle(&bndl,reply);
}


void initMIDI(void)
{
  pinMode(LED_BUILTIN,OUTPUT); // use as MIDI monitor 
  usbMIDI.setHandleNoteOff(OSCnoteOff);
  usbMIDI.setHandleNoteOn(OSCnoteOn);
}


unsigned int offTime;
void updateMIDI(void)
{
  while (usbMIDI.read()) 
  {
    offTime = millis()+100;
    dbgMIDIget(); // mop up untrapped messages
  }

  if (millis() < offTime)
    digitalWrite(LED_BUILTIN,1);
  else
    digitalWrite(LED_BUILTIN,0);
}
