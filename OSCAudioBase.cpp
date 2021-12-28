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


/* ******************************************************************
 * Generate an array with the names of all the available AudioStream-derived objects,
 * together with pointers to the functions to call to create instances of each.
 */
#define OSC_CLASS(a,o) \
OSCAudioBase* mk1_##o(const char* nm) {return (OSCAudioBase*) new o(nm);} \
OSCAudioBase* mk2_##o(const char* nm,OSCAudioGroup& grp) {return (OSCAudioBase*) new o(nm,grp);} 
OSC_AUDIO_CLASSES
#undef OSC_CLASS

#define OSC_CLASS(a,o) {#a,mk1_##o,mk2_##o},
const OSCAudioTypes_t OSCAudioBase::audioTypes[] = {
  OSC_AUDIO_CLASSES
};
#undef OSC_CLASS

// Return number of available audio objects
size_t OSCAudioBase::countOfAudioTypes(void) {return COUNT_OF(audioTypes);}
// ********* end of magic array generator stuff ********************


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


// Link into a grouping list
void OSCAudioBase::linkInGroup(OSCAudioBase* p) 
{
	OSCAudioGroup* parent = (OSCAudioGroup*) p;
	if (NULL != parent)
	{
		next_route = parent->next_group; 
		parent->next_group = this;
		pParent = parent;
	}
}


// Unlink from a grouping list
void OSCAudioBase::linkOutGroup(OSCAudioGroup** p) 
{
	OSCAudioGroup* parent = *p;
	if (NULL != parent)
	{
		OSCAudioBase** ppLink = &(parent->next_group);
		
		while (NULL != *ppLink && this != *ppLink)
		{
			ppLink = &((*ppLink)->next_route);
			OSC_SPTF("%08X ... ",(uint32_t) *ppLink); OSC_SFSH();
		}
		if (NULL != ppLink)
		{
			OSC_SPTF("Unlink!\n"); OSC_SFSH();
			*ppLink = next_route;
			next_route = NULL;
		}		
		*p = NULL;
	}	
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
							       char* dst, //!< destination string: may be same as src
								   int offset) //!< offset into sanitisation string: set to 1 to allow /
{
	char* dstCopy = dst;
	const char* toChange = "/ #*,?[]{}";
	
	while (0 != *src)
	{
		if (NULL == strchr(toChange+offset,*src))
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
static void hitCountCB(OSCAudioBase* ooi,OSCMessage& msg,int offset,void* count) {(*(int*) count)++;}
/**
 *	Count the number of objects that match the supplied address.
 */
int OSCAudioBase::hitCount(const char* addr,	//!< address to match
							OSCAudioBase* ooi,	//!< where in structure to start from (default is root)
							bool enterGroups)	//!< whether to allow matches in sub-groups
{
	int result = 0;
	
	callBack(addr,hitCountCB,(void*) &result,ooi,enterGroups);
	
	return result;
}


struct findMatch_s {int count; OSCAudioBase* hit;};
static void findMatchCB(OSCAudioBase* ooi,OSCMessage& msg,int offset,void* ctxt) 
{
	findMatch_s* context = (findMatch_s*) ctxt;
	context->count++;
	context->hit = ooi;
}
/**
 *	Count the number of objects that match the supplied address, and return a pointer to the last found
 */
int OSCAudioBase::findMatch(const char* addr,		//!< address to match
							OSCAudioBase** found,	//!< last-found matching object
							OSCAudioBase* ooi,		//!< where in structure to start from (default is root)
							bool enterGroups)		//!< whether to allow matches in sub-groups
{
	findMatch_s result = {0};
	
	callBack(addr,findMatchCB,(void*) &result,ooi,enterGroups);
	*found = result.hit;
	
	return result.count;
}
		 
		 

/**
 *	Route message for the creation engine to the correct function.
 */
void OSCAudioBase::routeDynamic(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
    if (isStaticTarget(msg,addressOffset,"/ren*","ss")) {renameObject(msg,addressOffset,reply);} 
#if defined(DYNAMIC_AUDIO_AVAILABLE)
    else if (isStaticTarget(msg,addressOffset,"/crC*","s"))  	{createConnection(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/crO*","ss"))	{createObject(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/crO*","sss"))	{createObject(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/crG*","ss"))	{Serial.println("group"); createGroup(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/d*","s"))			{destroyObject(msg,addressOffset,reply);} 
    else if (isStaticTarget(msg,addressOffset,"/clearAl*",NULL))    {clearAllObjects(msg,addressOffset,reply);} 
		else OSC_SPTF("No match\n");
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
OSCAudioBase::error DynamicAudioCreateObject(int objIdx,		//!< index of [OSC]AudioStream object to create
																						 const char* objName,		//!< name of object
																						 OSCAudioBase* parent) 	//!< parent object
{
	OSCAudioBase::error retval = OSCAudioBase::OK;
	void* pNewObj = NULL;
	
	if (NULL == parent)
		pNewObj = OSCAudioBase::audioTypes[objIdx].mkRoot(objName);
	else
		pNewObj = OSCAudioBase::audioTypes[objIdx].mkGroup(objName,*((OSCAudioGroup*) parent));
	
	if (NULL != pNewObj)
	{
		OSC_SPTF("Created %s as a new %s at %08X\n",objName, OSCAudioBase::audioTypes[objIdx].name, (uint32_t) pNewObj);
	}
	
	return retval;
}
#endif // defined(DISABLE_FULL_DYNAMIC)
//-------------------------------------------------------------------------------------------------------
struct crObContext_s {int objIdx; const char* name; OSCAudioBase::error retval;};
void createObjectCB(OSCAudioBase* parent,OSCMessage& msg,int offset,void* ctxt)
{
	crObContext_s* context = (crObContext_s*) ctxt;
	OSCAudioBase::error retval = OSCAudioBase::OK;
	OSCAudioBase* png = parent;
	
	if (NULL != png)
			png = parent->next_group;
	
	if (0 != OSCAudioBase::hitCount(context->name,png,false))
	{
		retval = OSCAudioBase::DUPLICATE_NAME;
		(void) retval;
	}
	else		
	{
		OSCAudioBase::error rv = DynamicAudioCreateObject(context->objIdx,context->name+1,parent);
		
		if (rv != OSCAudioBase::OK)
			context->retval = rv;
	}
}


/**
 *	Create a new [OSC]AudioStream object.
 */
void OSCAudioBase::createObject(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	error retval = OK;
	char objNameX[50],typ[50];
	char* objName = objNameX+1;
	int objIdx;
	crObContext_s context;
	
	msg.getString(0,typ,50);
	objIdx = OSCAudioBase::getTypeIndex(typ); // see if we've got a valid object type
	
	if (objIdx >= 0) // valid type
	{	
		msg.getString(1,objName,50);
		if (strlen(objName) > 0)
		{
			*objNameX = '/'; // needed for duplicate checking
			trimUnderscores(sanitise(objName,objName),objName); // make the name valid
			
			OSC_SPTF("createObject(%s,%s)\n",typ,objName);
			OSC_DBGP(msg,addressOffset);
			
			if (msg.size() < 3)
				retval = DynamicAudioCreateObject(objIdx,objName,NULL);
			else
			{
				msg.getString(2,typ,50); // re-use this buffer, not needed now
				context = {objIdx,objNameX,OK};
				callBack(typ,createObjectCB,&context);
			}
		}
		else
			retval = BLANK_NAME;
	}
	else 
		retval = NOT_FOUND;
		
	staticPrepareReplyResult(msg,reply).add(objName).add((int) retval);
}


//============================== OSCAudioGroup =====================================================
OSCAudioGroup::OSCAudioGroup(const char* _name, OSCAudioGroup* parent) 
							:  OSCAudioBase(_name), first_src(NULL), first_dst(NULL) 
{
	if (NULL != parent) // we're a group member not on the main routing list
	{
		linkOut();
		linkInGroup(parent);
		OSC_SPTF("%s group re-linked:\n",name);
		listObjects();
	}
}

	
OSCAudioGroup::~OSCAudioGroup()
{
	while (NULL != next_group)
		delete next_group;			
	while (NULL != first_src)
		delete first_src;			
	while (NULL != first_dst)
		delete first_dst;	
	linkOutGroup();
}

/**
 * Return pointer to next OSC audio group after given one
 */
OSCAudioBase* OSCAudioGroup::getNextGroup(void)
{
	return next_group;
}
	
	
/**
 * Route message to members of this group, if the group name matches.
 */
void OSCAudioGroup::route(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	int nameOff = isMine(msg,addressOffset);
	if (nameOff > 0)
	{ 
		routeFrom((OSCAudioBase**) &next_group,msg,addressOffset+nameOff,reply);
	}
}

// Link in and out of the grouping lists
void OSCAudioGroup::linkInGroup(OSCAudioGroup* parent) 
{
	if (NULL != parent)
	{
		next_route = parent->next_group; 
		parent->next_group = this;
		pParent = parent;
	}
}

void OSCAudioGroup::linkOutGroup() 
{
	if (NULL == pParent) // in main routing list...
		linkOut();				 // ...unlink from that
	else
	{
		OSCAudioBase** ppLink = &(pParent->next_group); 
		
		while (NULL != *ppLink && this != *ppLink)
			ppLink = &((*ppLink)->next_group);
		if (NULL != ppLink)
		{
			*ppLink = next_group;
			next_group = NULL;
		}
	}
}





void createGroupCB(OSCAudioBase* parent,OSCMessage& msg,int offset,void* context)
{
	char* buf = (char*) context;
	OSCAudioBase::error retval = OSCAudioBase::OK;
	OSCAudioBase* png = parent;
	
	if (NULL != png)
			png = parent->next_group;
	
	if (0 != OSCAudioBase::hitCount(buf,png,false))
	{
		retval = OSCAudioBase::DUPLICATE_NAME;
		(void) retval;
	}
	else
	{
		OSCAudioGroup* pNewGroup = new OSCAudioGroup(buf+1,(OSCAudioGroup*) parent);
		(void) pNewGroup;
		OSC_SPTF("Created at: 0x%08X, parent %08X\n",(uint32_t) pNewGroup,(uint32_t) (pNewGroup->pParent));
	}	
}


/**
 *	Create a new OSCAudioGroup object.
 */
void OSCAudioBase::createGroup(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	char path[50], bufn[50];
	error retval = OK;
	char* buf = bufn+1;
	
	OSC_SPLN("createGroup");
	OSC_DBGP(msg,addressOffset);
	msg.getString(0,path,50);
	msg.getString(1,buf,50);
	*bufn = '/'; // needed for duplicate checking
	trimUnderscores(sanitise(buf,buf),buf); // make the name valid
	OSC_SPLN(buf);
	
	if (0 != strlen(buf))
	{
		if ('/' == *path && 1 == strlen(path)) // create directly at root
			createGroupCB(NULL,msg,0,bufn);
		else	// sub-group of existing group
			callBack(path,createGroupCB,bufn);
	}
	else
		retval = BLANK_NAME;
	
	staticPrepareReplyResult(msg,reply).add(buf).add((int) retval);
}


//============================== OSCAudioConnection =====================================================
/**
 * Destroy an OSCAudioConnection: ensure it's unlinked from its lists.
 */
OSCAudioConnection::~OSCAudioConnection(void)
{
	linkOutSrc();
	linkOutDst();
}


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
	AudioStream* src = NULL,*dst = NULL;
	int srcp=0,dstp=0,count;
	OSCAudioBase* srcB,*dstB;
	error retval = OK;
	
	OSC_SPLN("makeConnection");
	OSC_DBGP(msg,addressOffset);
	
	msg.getString(0,srcn,50);
	trimUnderscores(sanitise(srcn,srcn,1),srcn); // make the source name valid
	if (!zeroToZero)
	{
		srcp = msg.getInt(1);
		msg.getString(2,dstn,50);
		dstp = msg.getInt(3);
	}
	else
		msg.getString(1,dstn,50);
	trimUnderscores(sanitise(dstn,dstn,1),dstn); // make the destination name valid
	
	// Find the named OSCAudioBase objects and convert to
	// the corresponding AudioStream: is there a better way?
	if (1 == (count = findMatch(srcn,&srcB))) // just one source, please
	{
		src = srcB->sibling;
		if (1 == (count = findMatch(dstn,&dstB))) // and one destination!
			dst = dstB->sibling;			
		else
			retval = AMBIGUOUS_PATH;
	}
	else
		retval = AMBIGUOUS_PATH;
	
	if (OK == retval)
	{
		if (NULL != src && NULL != dst)
		{
			sprintf(buf,"%s:%d -> %s:%d",srcn,srcp,dstn,dstp);
			connect(*src,(int) srcp,*dst,(int) dstp); // make the audio connection
			mkLinks(*srcB,*dstB); // make the OSC linkages
		}
		else
		{
			sprintf(buf,"Nothing!");
			retval = NOT_FOUND;
		}
	}
	else
		sprintf(buf,"Bad path");
	OSC_SPLN(buf);
	
	prepareReplyResult(msg,reply).set(1,buf).add((int) retval);
}


void OSCAudioConnection::mkLinks(OSCAudioBase& src, OSCAudioBase& dst)
{
	Serial.printf("Link %08X to source %08X (parent %08X)\n",
					(uint32_t) this,
					(uint32_t) &src,
					(uint32_t) src.pParent
					);
					
	linkInSrc(src.pParent);
	listObjects();
	Serial.printf(" and dest %08X (parent %08X)\n\n",
					(uint32_t) &dst,
					(uint32_t) dst.pParent
					);
					
	linkInDst(dst.pParent);
	listObjects();
}


void OSCAudioConnection::route(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
	int nameOff = isMine(msg,addressOffset);
	if (nameOff > 0)
	{ 
		Serial.printf("It's for %s!\n",name);
		addressOffset += nameOff;
		if (isTarget(msg,addressOffset,"/c*","ss")) {OSCconnect(msg,addressOffset,reply,true);}
		else if (isTarget(msg,addressOffset,"/c*","sisi")) {OSCconnect(msg,addressOffset,reply);} 
		else if (isTarget(msg,addressOffset,"/d*",NULL)) {disconnect();} 
	}
}


// Link in and out of the connection lists
void OSCAudioConnection::linkInSrc(OSCAudioGroup* parent) 
{
	if (NULL != parent)
	{
		next_src = parent->first_src; 
		parent->first_src = this;
		pSrcParent = parent;
	}
}


void OSCAudioConnection::linkOutSrc() 
{
	if (NULL != pSrcParent)
	{
		OSCAudioConnection** ppLink = &(pSrcParent->first_src); 			
		while (NULL != *ppLink && this != *ppLink)
			ppLink = &((*ppLink)->next_src);
		if (NULL != ppLink)
		{
			*ppLink = next_src;
			next_src = NULL;
		}
	}
}

// Link in and out of the connection lists
void OSCAudioConnection::linkInDst(OSCAudioGroup* parent) 
{
	if (NULL != parent)
	{
		next_dst = parent->first_dst; 
		parent->first_dst = this;
		pDstParent = parent;
	}
}

void OSCAudioConnection::linkOutDst() 
{
	if (NULL != pDstParent)
	{
		OSCAudioConnection** ppLink = &(pDstParent->first_dst); 			
		while (NULL != *ppLink && this != *ppLink)
			ppLink = &((*ppLink)->next_dst);
		if (NULL != ppLink)
		{
			*ppLink = next_dst;
			next_dst = NULL;
		}
	}
}

#endif // defined(DYNAMIC_AUDIO_AVAILABLE)


