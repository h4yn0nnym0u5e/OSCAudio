#include "OSCAudioBase.h"
OSCAudioBase* OSCAudioBase::first_route = NULL;

void OSCAudioBase::routeDynamic(OSCMessage& msg, int addressOffset)
{
	char prt[50];
	msg.getAddress(prt,addressOffset);

	Serial.println(addressOffset);
	Serial.println(prt);
	Serial.println(msg.size());
	Serial.println(); 
}
