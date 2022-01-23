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

#if !defined(_OSCUTILS_H_)
#define _OSCUTILS_H_

#include <OSCBundle.h>

#if !defined(COUNT_OF)
#define COUNT_OF(x) ((sizeof x) / (sizeof x[0]))
#endif // !defined(COUNT_OF)
 

#define noOSC_DEBUG_PRINT
#if defined(OSC_DEBUG_PRINT) && !defined(DEBUG_SER)
#define DEBUG_SER Serial
#define OSC_SPRT(...) DEBUG_SER.print(__VA_ARGS__)
#define OSC_SPLN(...) DEBUG_SER.println(__VA_ARGS__)
#define OSC_SPTF(...) DEBUG_SER.printf(__VA_ARGS__)
#define OSC_SFSH(...) DEBUG_SER.flush(__VA_ARGS__)
#define OSC_DBGP(...) dbgPrt(__VA_ARGS__)
#else
#define OSC_SPRT(...)
#define OSC_SPLN(...)
#define OSC_SPTF(...)
#define OSC_SFSH(...)
#define OSC_DBGP(...)
#endif // defined(OSC_DEBUG_PRINT)


class OSCUtils
{
  public:
	virtual ~OSCUtils(void) {}
    virtual void route(OSCMessage& msg, int addressOffset, OSCBundle&)=0;
	enum error {OK,NOT_FOUND,BLANK_NAME,DUPLICATE_NAME,NO_DYNAMIC,NO_MEMORY, // 0-5
				AMBIGUOUS_PATH,NOT_ROUTED,INVALID_METHOD,NOT_CONNECTED}; // 6-9

	/**
	 * Check to see if message's parameter types match those expected for the 
	 * candidate function to be called.
	 * 
	 * If the types string has a '*' then we say the match is OK; allows for optional parameters.
	 */
	static bool validParams(OSCMessage& msg,	//!< OSC message to check
						const char* types)		//!< expected parameter types: NULL imples none expected
	{
		size_t sl = 0;
		bool result = true;
	
		if (NULL != types)
			sl = strlen(types);
	
		for (size_t i=0;i<sl && result && '*' != types[i];i++)
		{
			char type = msg.getType(i);
			
			result = types[i] == type;
			if (!result && ';' == types[i]) // boolean: encoded directly in type
				result = type == 'T' || type == 'F';
		}
		
		return result;
	}


	/**
	 * Check to see if message matches expected target pattern and parameter types
	 */
	bool isTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
	{
		bool result = msg.fullMatch(pattern,addressOffset) && validParams(msg,types);
		
		return result;
	}
	
	/**
	 * Check to see if message matches expected target pattern and parameter types
	 */
	static bool isStaticTarget(OSCMessage& msg,int addressOffset,const char* pattern,const char* types)
	{
		return msg.fullMatch(pattern,addressOffset) && validParams(msg,types);
	}

	// Reply mechanisms:
	void addReplyExecuted(OSCMessage& msg, int addressOffset, OSCBundle& reply, char* name = NULL);
	
	static OSCMessage& staticPrepareReplyResult(OSCMessage& msg, OSCBundle& reply);
	OSCMessage& prepareReplyResult(OSCMessage& msg, OSCBundle& reply, char* name = NULL);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, bool v, char* name = NULL, error ret = OK);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, float v, char* name = NULL);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, int32_t v, char* name = NULL);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint32_t v, char* name = NULL);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint8_t v, char* name = NULL);
	void addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint16_t v, char* name = NULL);

	static char* getMessageAddress(OSCMessage& msg, void* buf, int len, int offset = 0); //!< read message address into memory assigned by alloca() (probably)
};

#endif // !defined(_OSCUTILS_H_)
