#if !defined(_adapter_sw_jack_error_H)
# define _adapter_sw_jack_error_H (1)

#define KR_ERR_JACK_SERVER_CONNECT -10
#define KR_ERR_JACK_MEGAFAIL -2000

#include <krad/app/fail.h>

static const char *kr_jack_strerr_idx[] = {
  [KR_ERRIDX(JACK_SERVER_CONNECT)] = "Jack: Could not connect to jackd",
  [KR_ERRIDX(JACK_MEGAFAIL)] = "Jack: Megafail"
};

#ifdef KR_COM_STRERR_IDX
#undef KR_COM_STRERR_IDX
#endif

#define KR_COM_STRERR_IDX kr_jack_strerr_idx

#endif
