#include <OSCSubscribe.h>

static OSCSubscriptionList* pSubList; // current subscription list
static OSCBundle* pSubBundle;         // where subscription messages will be placed

void initSubscribe(void)
{
  long long tt = 0;
  
  pSubList = new OSCSubscriptionList(processMessage);
  pSubBundle = new OSCBundle;
  pSubBundle->setTimetag((uint8_t*) &tt).add("/subEvt");
}


void updateSubscribe(void)
{
  OSCMessage* pMsg;

  // poll subscriptions seeing if any need to generate an event
  pSubList->update(*pSubBundle);

  pMsg = pSubBundle->getOSCMessage(0);
  if (pMsg->size() > 0) // the update caused some activity
  {
    long long tt = 0;

    sendReply(*pSubBundle);
    
    // prepare for next subscription event
    pSubBundle->empty();
    pSubBundle->setTimetag((uint8_t*) &tt).add("/subEvt");
  }
}


// route messages to create / modify subscriptions
void routeSubscribe(OSCMessage& msg, int addressOffset, OSCBundle& reply)
{
  //Serial.println("subscription engine message!");
  pSubList->route(msg,addressOffset,reply);
}
