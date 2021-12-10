#include "OSCAudioBase.h"
OSCAudioBase* OSCAudioBase::first_route = NULL;

static void dbgPrt(OSCMessage& msg, int addressOffset)
{
	char prt[50];
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
void OSCAudioBase::renameObject(OSCMessage& msg, int addressOffset)
{
	char oldName[50],newName[50];
	OSCAudioBase* pVictim;
	
	msg.getString(1,newName,50);
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
		}
	}
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


//=======================================================================================================
//============================== Dynamic Audio Objects ==================================================
/**
 *	Route message for the creation engine to the correct function.
 */
void OSCAudioBase::routeDynamic(OSCMessage& msg, int addressOffset)
{
    if (isStaticTarget(msg,addressOffset,"/ren*","ss")) {renameObject(msg,addressOffset);} 
#if defined(SAFE_RELEASE) // only defined in Dynamic Audio Objects library
    else if (isStaticTarget(msg,addressOffset,"/cr*O*","ss")) {createObject(msg,addressOffset);} 
    else if (isStaticTarget(msg,addressOffset,"/cr*C","s")) {createConnection(msg,addressOffset);} 
    else if (isStaticTarget(msg,addressOffset,"/d*","s")) {destroyObject(msg,addressOffset);} 
#endif // defined(SAFE_RELEASE)
}


#if defined(SAFE_RELEASE) // only defined in Dynamic Audio Objects library
/**
 *	Destroy an [OSC]AudioStream or Connection object.
 */
void OSCAudioBase::destroyObject(OSCMessage& msg, int addressOffset)
{
	char buf[50];
	OSCAudioBase* pVictim;
	msg.getString(0,buf,50);
	trimUnderscores(sanitise(buf,buf),buf); // make the name valid
	
	Serial.print("destroyObject: ");
	Serial.println(buf);
	dbgPrt(msg,addressOffset);
		
	pVictim = OSCAudioBase::find(buf);
	Serial.printf("Victim is at: 0x%08X\n",(uint32_t) pVictim);
	Serial.flush();
	if (NULL != pVictim)
		delete pVictim;
	else
		Serial.println("not found!");
}

//============================== OSCAudioStream =========================================================
/**
 *	Create a new [OSC]AudioStream object.
 */
void OSCAudioBase::createObject(OSCMessage& msg, int addressOffset)
{
	char name[50],typ[50];
	void* pNewObj = NULL;
	msg.getString(0,typ,50);
	msg.getString(1,name,50);
	trimUnderscores(sanitise(name,name),name); // make the name valid
	
	Serial.printf("createObject(%s,%s)\n",typ,name);
	dbgPrt(msg,addressOffset);
	
	if (0 == strlen(name)) 		 Serial.println("blank name"); // don't allow blank name
	else if (NULL != find(name)) Serial.println("duplicate");  // don't allow duplicate name
#define OSC_CLASS(a,o) else if (0 == strcmp(#a,typ)) pNewObj = new o(name);
	OSC_AUDIO_CLASSES // massive inefficient macro expansion to create object of required type
#undef OSC_CLASS
	
	if (NULL != pNewObj)
	{
		Serial.printf("Created %s as a new %s at %08X\n",name, typ, (uint32_t) pNewObj);
	}
}

//============================== OSCAudioConnection =====================================================
/**
 *	Create a new [OSC]AudioConnection object.
 */
void OSCAudioBase::createConnection(OSCMessage& msg, int addressOffset)
{
	char buf[50];
	
	Serial.println("createConnection");
	dbgPrt(msg,addressOffset);
	msg.getString(0,buf,50);
	trimUnderscores(sanitise(buf,buf),buf); // make the name valid
	Serial.println(buf);
	
	if (0 != strlen(buf))
	{
		OSCAudioConnection* pNewConn = new OSCAudioConnection(buf);
		(void) pNewConn;
		Serial.printf("Created at: 0x%08X\n",(uint32_t) pNewConn);
	}
}


/**
 *	Connect an [OSC]AudioConnection object to specific object ports
 */
void OSCAudioConnection::OSCconnect(OSCMessage& msg, 
								 int addressOffset, 
								 bool zeroToZero) //!< true to use port 0 on both, otherwise they're in the message
{
	char srcn[50],dstn[50];
	AudioStream* src,*dst;
	int srcp=0,dstp=0;
	OSCAudioBase* tmp;
	
	Serial.println("makeConnection");
	dbgPrt(msg,addressOffset);
	
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

	Serial.printf("%s:%d -> %s:%d\n",srcn,srcp,dstn,dstp);
	
	if (NULL != src && NULL != dst)
		connect(*src,(int) srcp,*dst,(int) dstp);
}
#endif // defined(SAFE_RELEASE)


