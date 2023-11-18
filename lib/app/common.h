#if !defined(_app_common_H)
# define _app_common_H (1)

#include "crate.h"

#define KRAD_SYSNAME_MIN 4
#define KRAD_SYSNAME_MAX 62
#define KRAD_SYSNAME_SZ 64

#define KR_EID_VOID 0xEC
#define KR_EID_CRATE 0xF1
#define KR_EID_ADDRESS 0xAE
#define KR_EID_METHOD 0xD7
#define KR_EID_PAYLOAD 0xB9
#define KR_EID_TERMINATOR 0xE2
#define KR_RWFDS_MAX 32

int kr_sysname_valid(char *sysname);
int kr_valid_host_and_port(char *string);

#endif
