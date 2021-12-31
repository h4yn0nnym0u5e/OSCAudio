# OSCAudio
Open Sound Control layer on top of Teensy Audio library
This library provides two fundamental layers on top of the Dynamic Audio Library (currently a fork of the original Teensy Audio library, though there is some indication that it could become mainstream at some point). Both build on the OSC  library already shipped as part of Teensyduino [note to self: is the shipped one sufficiently up to date?] and provide
- control of existing audio objects via OSC
- dynamic changes to the toplogy of the audio design, without needing a complete re-compile

The examples show these in action, along with simple filesystem access and MIDI mapping; these latter are not part of the core functionality, but may serve as inspiration for more fully-fledged projects.

## OSC
A complete description of OSC is beyond the scope of this ReadMe: please refer to https://github.com/CNMAT/OSC and the API and specification documentation linked from there for full details. Essentially OSC is a standard for packing information along with a destination, such that it can be interpreted and acted upon at that destination. The transport of the information is also not part of the specification: the examples use messages which arrive via a serial port, from the Teensy filesystem, or are internally generated from MIDI data.
 
Routing of messages depends upon their address. This will be of the form **/path/to/destination/method**: at any point in the delivery process part of the address may have been "consumed", with the remainder forwarded to the next level until something executes the final "method" using the attached parameters. Within the examples we have used /teensy1 as the top level, thus acknowledging that future systems might have multiple Teensys, with messages broadcast via Ethernet or similar.

Parameters to the methods are of types integer, float, string or blob. In the description below these will be shown as type codes i, f, s or b, or values will be shown in angle brackets, e.g. <220.0> is of type f. Optional parameters will be shown in square brackets, e.g. **/dynamic/crOb,s[s]** denotes that the dynamic createObject method may take one or two strings as its parameters.

### Internal routing
The two essential layers implemented by the OSCAudio library are named /audio and /dynamic. Whilst they could in principle be changed by the application writer, these names are built in to a separate development of the Audio System Design++ tool (https://manicken.github.io/#, henceforth called GUI++ for brevity) which provides an excellent way to make use of OSCAudio.
#### /audio
Invoke by calling **OSCAudioBase::routeDynamic(OSCMessage& msg,int addressOffset,OSCBundle& reply)**

This provides control of existing audio objects. For example, assuming you have an AudioSynthWaveform object called "wav", the message **/teensy1/audio/wav/begin<0.5><220.0><0>** will cause it to output a 220Hz sine wave at 50% amplitude. Almost every function for every audio object is available, and uses the exact parameters documented in the GUI++ help. This is important to remember when constructing your own messages, as the parameter types *must* be correct for a method to be invoked. **msg("/teensy1/audio/wav/begin").add(0.5).add(220.0f).add(0)** should work, but **msg("/teensy1/audio/wav/begin").add(0.5).add(220).add(0)** won't, because the frequency is an integer, not a float.

Some additional /audio methods are used to manipulate connection objects:
- **/c**onnect,ss: create an audio connection; first parameter is the source; second is the destination; uses port 0 in both cases
- **/c**onnect,sisi: create an audio connection; first two parameters are the source and output port; second two are the destination and input port
- **/d**isonnect: disconnect from source and destination (leaves the connection object in existence)


### /dynamic
This allows manipulation of the audio system's topology by creating and destroying objects and connections. Methods available are:
- **/crC**onnection,s[s]: create a connection; first parameter is the name; optional second is the group
- **/crO**bject,ss[s]: create an audio object; first parameter is the type; second is the name; optional third is the group
-  **/crG**roup,ss: create a group; first parameter is the name; second is the parent group - use "/" to create at the root
- **d**delete,s: delete object; parameter is object name [group?!]
- **ren**ame,ss: rename object; first parameter is object name; second is new name [group?!]
- **clearAll**: remove all dynamic objects

For example, **/teensy1/dynamic/crOb<AudioSynthWaveform><wav>** will create a new AudioSynthWaveform object called "wav", at the root level of your design. Here's a longer example which creates and connects a waveform and an output device, and starts an audible 220Hz tone:
~~~
/teensy1/dynamic/crOb<AudioSynthWaveform><wav>
/teensy1/dynamic/crOb<AudioOutputI2S><i2s>
/teensy1/dynamic/crOb<AudioControlSGTL5000><sgtl5000>
/teensy1/dynamic/crCo<wav_i2sL>
/teensy1/dynamic/crCo<wav_i2sR>
/teensy1/audio/wav_i2sL/co<wav><i2s>
/teensy1/audio/wav_i2sR/co<wav>0<i2s><1>
/teensy1/audio/sgtl5000/enable
/teensy1/audio/sgtl5000/vol<0.5>
/teensy1/audio/wav/begin<0.1><220.0><0>
~~~


