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
    int unsubscribe(OSCMessage& msg);
    int update(OSCBundle&);
};
