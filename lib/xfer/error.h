#if !defined(_xfer_error_H)
# define _xfer_error_H (1)

#define KR_ERR_XFER_DISCONBOBULATED -1000
#define KR_ERR_XFER_FRAKERSNACKED -16

#include <krad/app/fail.h>

static const char *kr_xfer_strerr_idx[] = {
  [KR_ERRIDX(XFER_DISCONBOBULATED)] = "XFER: Disconbobulated",
  [KR_ERRIDX(XFER_FRAKERSNACKED)] = "XFER: Frackersnacked"
};

#ifdef KR_COM_STRERR_IDX
#undef KR_COM_STRERR_IDX
#endif

#define KR_COM_STRERR_IDX kr_xfer_strerr_idx

#endif
