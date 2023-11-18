#if !defined(_app_fail_H)
# define _app_fail_H (1)

#include "debug.h"

#define KR_ERRIDX(x) KR_ERR_##x * -1

#define logfail(x) (printke("Code: %d Message: %s" \
 "\n %s:%d", (x), \
 KR_COM_STRERR_IDX[-1 * x], __FILE__, __LINE__))

#define fail(x) {(logfail((x))); return (x);}
#define nullfail(x) {(logfail((x))); return NULL;}

#endif
