#if !defined(COUNT_OF)
#define COUNT_OF(x) ((sizeof x) / (sizeof x[0]))
#endif // !defined(COUNT_OF)

#define INCLUDE_FS

void processMessage(OSCMessage* msg,OSCBundle& reply);
