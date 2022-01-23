/* Open Sound Control subscription library for Teensy 3.x, 4.x
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
 
 #include <OSCUtils.h>

typedef uint32_t subTimer_t;
#define OSCSUB_NOW (millis())
class OSCSubscriptionList;

class OSCSubscribe : OSCUtils
{
	friend class OSCSubscriptionList;

	subTimer_t interval;
	subTimer_t lifeTime;
	size_t subAddrLen;
	OSCSubscribe* next;

	OSCMessage sub;
	subTimer_t nextTime;
	void doSub(void (*routeFn)(OSCMessage*,OSCBundle&),OSCBundle& bndl);
	void route(OSCMessage&, int, OSCBundle&){}
  public:
	OSCSubscribe(subTimer_t intvl, subTimer_t lifeT, char* addr,OSCSubscribe* nxt);
};

class OSCSubscriptionList : OSCUtils
{
    OSCSubscribe* pSubs = NULL;
	void (*routeFn)(OSCMessage*,OSCBundle&);
  public:
	OSCSubscriptionList(void (*rFn)(OSCMessage*,OSCBundle&)) : routeFn(rFn) {}
    void route(OSCMessage& msg, int addressOffset, OSCBundle& reply);
    bool subscribe(OSCMessage& msg);
	int renew(OSCMessage& msg);
    int unsubscribe(OSCMessage& msg);
    int update(OSCBundle&);
};
