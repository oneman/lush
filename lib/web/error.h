#if !defined(_web_error_H)
# define _web_error_H (1)

#define KR_ERR_WEB_DISASTER -1000
#define KR_ERR_WEB_MEGAFAIL -2000

#include <krad/app/fail.h>

/*static const char *kr_web_strerr_idx[] = {
  [KR_ERRIDX(WEB_DISASTER)] = "Web: Disaster",
  [KR_ERRIDX(WEB_MEGAFAIL)] = "Web: Megafail"
};*/

#ifdef KR_COM_STRERR_IDX
#undef KR_COM_STRERR_IDX
#endif

#define KR_COM_STRERR_IDX kr_web_strerr_idx

#endif
