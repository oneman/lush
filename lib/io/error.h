#if !defined(_io_error_H)
# define _io_error_H (1)

#define KR_ERR_IO_FILE_RAWR -1000
#define KR_ERR_IO_FILE_FRAKED -16

#include <krad/app/fail.h>

static const char *kr_io_strerr_idx[] = {
  [KR_ERRIDX(IO_FILE_RAWR)] = "File: Rawr",
  [KR_ERRIDX(IO_FILE_FRAKED)] = "File: Fraked"
};

#ifdef KR_COM_STRERR_IDX
#undef KR_COM_STRERR_IDX
#endif

#define KR_COM_STRERR_IDX kr_io_strerr_idx

#endif
