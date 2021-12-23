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

#include "OSCAudioBase.h"
OSCAudioBase* OSCAudioBase::first_route = NULL;

static void dbgPrt(OSCMessage& msg, int addressOffset)
{
	char prt[50];
	(void) dbgPrt; // avoid warning
	msg.getAddress(prt,addressOffset);

	Serial.println(addressOffset);
	Serial.println(prt);
	Serial.println(msg.size());
	Serial.println(); 
}


/**
 *	Rename an [OSC]AudioStream or Connection object.
 *  This could be an /audio function, but that would pollute the 
 *	audio functions' name spaces, so we make it a /dynamic
 *  function instead.
 */
void OSCAudioBase::renameObject(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	error retval = OK;
	char oldName[50],newName[50],buf[150];
	OSCAudioBase* pVictim;
	//OSCAudioBase** thisGroupRoot = pParent;
	
	msg.getString(1,newName,50);
	OSC_SPTF("new name is %s, size %d\n",newName,msg.getDataLength(1));
	trimUnderscores(sanitise(newName,newName),newName); // make the new name valid
	pVictim = OSCAudioBase::find(newName);
	if (NULL == pVictim) // we're not duplicating the name of another object: good
	{
		msg.getString(0,oldName,50);
		trimUnderscores(sanitise(oldName,oldName),oldName); // make the old name valid
		if (0 != strlen(newName)) // zero-length names are not allowed
		{
			pVictim = OSCAudioBase::find(oldName);
			if (NULL != pVictim)
			{
				pVictim->setName(newName);
			}
			else
				retval = NOT_FOUND; // object to rename not found			
		}
		else
			retval = BLANK_NAME; // new name [sanitises to] an empty string			
	}
	else
		retval = DUPLICATE_NAME; // object by requested name already exists
	
	sprintf(buf,"%s -> %s",oldName,newName);
	staticPrepareReplyResult(msg,reply).add(buf).add(retval);
}


/**
 * Sanitise a string to contain only characters valid in an OSC container or method.
 * Replaces all invalid characters <space>#*,/?[]{} with _.
 * \return pointer to result string (same as dst)
 */
char* OSCAudioBase::sanitise(const char* src, //!< source string
							       char* dst) //!< destination string: may be same as src
{
	char* dstCopy = dst;
	
	while (0 != *src)
	{
		if (NULL == strchr(" #*,/?[]{}",*src))
			*dst++ = *src;
		else
			*dst++ = '_';
		src++;
	}
	*dst = 0;
	
	return dstCopy;
}


/**
 * Trim leading and trailing underscores from a string, and compress internal runs.
 * Replaces all invalid characters <space>#*,/?[]{} with _.
 * \return pointer to result string (same as dst)
 */
char* OSCAudioBase::trimUnderscores(const char* src, //!< source string
							         char* dst) //!< destination string: may be same as src
{
	char* dstCopy = dst;
	bool inBody = false,lastWas_ = false;

	while (0 != *src)
	{
		if ('_' == *src)
		{
			if (inBody && !lastWas_)
				*dst++ = *src;
			lastWas_ = true;
		}
		else
		{
			*dst++ = *src;
			inBody = true;
			lastWas_ = false;
		}
		src++;
	}
	
	if (lastWas_ && dst != dstCopy) // trailing underscore in non-empty result...
		dst--;	  					// ...overwrite it
	*dst = 0; // terminate result
	
	return dstCopy;
}

/**
 * Add to the reply bundle as a result of having executed an OSC message routed to us.
 */
void OSCAudioBase::addReplyExecuted(OSCMessage& msg, int addressOffset, OSCBundle& reply) 
{
	prepareReplyResult(msg,reply).add(true); // add a "true" bool to the response, because we did the method
}


/**
 * Prepare the initial part of a reply to a message routed to us.
 * Fills in the standard information generated for any successful routing.
 * \return reference to the OSCMessage, ready to add any extra information, dependent on the method called
 */
OSCMessage& OSCAudioBase::staticPrepareReplyResult(OSCMessage& msg, 	//!< the received message
														  OSCBundle& reply) //!< the bundle that will become the reply
{
	int msgCount = reply.size(); // number of messages in bundle
	OSCMessage* pLastMsg = reply.getOSCMessage(msgCount-1); // point to last message in reply bundle
	int dataCount = pLastMsg->size(); // how many pieces of data are in that?
	char replyAddress[50];
	pLastMsg->getAddress(replyAddress);
	
	char buf[50];
	msg.getAddress(buf);
	
	if (0 != dataCount) // message already in use...
		pLastMsg = &reply.add(replyAddress); // ... make ourselves a new one
		
	// start composing our reply:
	pLastMsg->add(buf);				// which address the routed message was destined for
	
	return *pLastMsg;
}


/**
 * Prepare the initial part of a reply to a message routed to us.
 * Fills in the standard information generated for any successful routing.
 * \return reference to the OSCMessage, ready to add any extra information, dependent on the method called
 */
OSCMessage& OSCAudioBase::prepareReplyResult(OSCMessage& msg, 	//!< the received message
											 OSCBundle& reply) 	//!< the bundle that will become the reply
{
	char buf[50];
	msg.getAddress(buf);
	OSC_SPTF("%s executed %s; result was ",name+1,buf);
	
	return staticPrepareReplyResult(msg,reply).add(name+1); // add which element caught the routed message
}


// Despatch function overloaded with the various reply types we might append to the standard information
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, bool v) { prepareReplyResult(msg, reply).add(v); Serial.println(v); }
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, float v) { prepareReplyResult(msg, reply).add(v); Serial.println(v); }
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, int32_t v) { prepareReplyResult(msg, reply).add(v); Serial.println(v); }
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint32_t v) { prepareReplyResult(msg, reply).add((unsigned int)v); Serial.println(v); }
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint8_t v) { prepareReplyResult(msg, reply).add(v); Serial.println(v); }
void OSCAudioBase::addReplyResult(OSCMessage& msg, int addressOffset, OSCBundle& reply, uint16_t v) { prepareReplyResult(msg, reply).add(v); Serial.println(v); }



//=======================================================================================================
//============================== Dynamic Audio Objects ==================================================
static void hitCountCB(OSCAudioBase* ooi,OSCMessage& msg,int offset,int* count) {(*count)++;}
/**
 *	Count the number of objects that match the supplied address.
 */
int OSCAudioBase::hitCount(const char* addr,	//!< address to match
										OSCAudioBase* ooi = NULL,	//!< where in structure to start from (default is root)
										bool enterGroups = true)	//!< whether to allow matches in sub-groups
{
	int result = 0;
	
	callBack(addr,hitCountCB,&result,ooi,enterGroups);
	
	return result;
}
		 

/**
 *	Route message for the creation engine to the correct function.
 */
void OSCAudioBase::routeDynamic(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
    if (isStaticTarget(msg,addressOffset,"/ren*","ss")) {renameObject(msg,addressOffset,reply);} 
#if defined(DYNAMIC_AUDIO_AVAILABLE)
    else if (isStaticTarget(msg,addressOffset,"/cr*C*","s"))  {createConnection(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/cr*O*","ss")) {createObject(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/d*","s"))     {destroyObject(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/clearAl*",NULL))    {clearAllObjects(msg,addressOffset,reply);} 
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)
}


#if defined(DYNAMIC_AUDIO_AVAILABLE)
/**
 *	Destroy an [OSC]AudioStream or Connection object.
 */
void OSCAudioBase::destroyObject(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	char buf[50];
	OSCAudioBase* pVictim;
	msg.getString(0,buf,50);
	trimUnderscores(sanitise(buf,buf),buf); // make the name valid
	
	OSC_SPRT("destroyObject: ");
	OSC_SPLN(buf);
	OSC_DBGP(msg,addressOffset); 
		
	pVictim = OSCAudioBase::find(buf);
	OSC_SPTF("Victim is at: 0x%08X\n",(uint32_t) pVictim);
	OSC_SFSH();
	if (NULL != pVictim)
		delete pVictim;
	else
	{
		OSC_SPLN("not found!"); // but not really an error!
	}
	
	staticPrepareReplyResult(msg,reply).add(buf).add((int) OK);
}

/**
 *	Destroy all dynamic OSCAudioStream and OSCAudioConnection objects.
 *  Note that this will not destroy objects unknown to the OSCAudio system.
 */
void OSCAudioBase::clearAllObjects(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	OSC_SPRT("clearAllObjects: ");
	OSC_DBGP(msg,addressOffset);
	
	while (NULL != first_route)
	{
		OSC_SPLN(first_route->name);
		OSC_SFSH();
		delete first_route;
	}
	staticPrepareReplyResult(msg,reply).add("ALL").add((int) OK);
}

//============================== OSCAudioStream =========================================================
//-------------------------------------------------------------------------------------------------------
// Test code for switching between full dynamic audio support and simple control
// messaging. If full support is enabled then the object code is MUCH larger, but
// it is possible to create objects at runtime.
#define noDISABLE_FULL_DYNAMIC
#if defined(DISABLE_FULL_DYNAMIC)
// weak definition
OSCAudioBase::error DynamicAudioCreateObject(char* typ,char* objName) __attribute__((weak));
OSCAudioBase::error DynamicAudioCreateObject(char* typ,char* objName)
{
	return OSCAudioBase::NO_DYNAMIC;
}

#else
// strong definition: use this if object creation at runtime is wanted
/**
 *	Create a new [OSC]AudioStream object.
 */
OSCAudioBase::error DynamicAudioCreateObject(char* typ,			//!< type of [OSC]AudioStream object to create
																						 char* objName)	//!< name of object: may include /path/to/group
{
	OSCAudioBase::error retval = OSCAudioBase::OK;
	void* pNewObj = NULL;
	
	if (0 == strlen(objName)) 		 
	{
		retval = OSCAudioBase::BLANK_NAME;
		OSC_SPLN("blank name"); // don't allow blank name
	}
	else 
		if (NULL != OSCAudioBase::find(objName))
		{
			retval = OSCAudioBase::DUPLICATE_NAME;
			OSC_SPLN("duplicate");  // don't allow duplicate name
		}
#define OSC_CLASS(a,o) else if (0 == strcmp(#a,typ)) pNewObj = new o(objName);
	OSC_AUDIO_CLASSES // massive inefficient macro expansion to create object of required type
#undef OSC_CLASS
	
	if (NULL != pNewObj)
	{
		OSC_SPTF("Created %s as a new %s at %08X\n",objName, typ, (uint32_t) pNewObj);
	}
	
	return retval;
}
#endif // defined(DISABLE_FULL_DYNAMIC)
//-------------------------------------------------------------------------------------------------------


/**
 *	Create a new [OSC]AudioStream object.
 */
void OSCAudioBase::createObject(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	error retval = OK;
	char objName[50],typ[50];
	msg.getString(0,typ,50);
	msg.getString(1,objName,50);
	trimUnderscores(sanitise(objName,objName),objName); // make the name valid
	
	OSC_SPTF("createObject(%s,%s)\n",typ,objName);
	OSC_DBGP(msg,addressOffset);
	
	retval = DynamicAudioCreateObject(typ,objName);
		
	staticPrepareReplyResult(msg,reply).add(objName).add((int) retval);
}


//============================== OSCAudioConnection =====================================================
/**
 *	Create a new [OSC]AudioConnection object.
 */
void OSCAudioBase::createConnection(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	char buf[50];
	error retval = OK;
	
	OSC_SPLN("createConnection");
	OSC_DBGP(msg,addressOffset);
	msg.getString(0,buf,50);
	trimUnderscores(sanitise(buf,buf),buf); // make the name valid
	OSC_SPLN(buf);
	
	if (0 != strlen(buf))
	{
		if (NULL != find(buf))
		{
			retval = DUPLICATE_NAME;
		}
		else
		{
			OSCAudioConnection* pNewConn = new OSCAudioConnection(buf);
			(void) pNewConn;
			OSC_SPTF("Created at: 0x%08X\n",(uint32_t) pNewConn);
		}
	}
	else
		retval = BLANK_NAME;
	
	staticPrepareReplyResult(msg,reply).add(buf).add((int) retval);
}


/**
 *	Connect an [OSC]AudioConnection object to specific object ports
 */
void OSCAudioConnection::OSCconnect(OSCMessage& msg, 
								 int addressOffset,
								 OSCBundle& reply, 
								 bool zeroToZero)  	//!< true to use port 0 on both, otherwise they're in the message
{
	char srcn[50],dstn[50],buf[150];
	AudioStream* src,*dst;
	int srcp=0,dstp=0;
	OSCAudioBase* tmp;
	error retval = OK;
	
	OSC_SPLN("makeConnection");
	OSC_DBGP(msg,addressOffset);
	
	msg.getString(0,srcn,50);
	trimUnderscores(sanitise(srcn,srcn),srcn); // make the source name valid
	if (!zeroToZero)
	{
		srcp = msg.getInt(1);
		msg.getString(2,dstn,50);
		dstp = msg.getInt(3);
	}
	else
		msg.getString(1,dstn,50);
	trimUnderscores(sanitise(dstn,dstn),dstn); // make the destination name valid
	
	// Find the named OSCAudioBase objects and convert to
	// the corresponding AudioStream: is there a better way?
	tmp = find(srcn); src = (NULL != tmp)?tmp->sibling:NULL;
	tmp = find(dstn); dst = (NULL != tmp)?tmp->sibling:NULL;
		
	if (NULL != src && NULL != dst)
	{
		sprintf(buf,"%s:%d -> %s:%d",srcn,srcp,dstn,dstp);
		connect(*src,(int) srcp,*dst,(int) dstp);
	}
	else
	{
		sprintf(buf,"Nothing!");
		retval = NOT_FOUND;
	}
	OSC_SPLN(buf);
	
	prepareReplyResult(msg,reply).set(1,buf).add((int) retval);
}
#endif // defined(DYNAMIC_AUDIO_AVAILABLE)


