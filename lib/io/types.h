#if !defined(_io_types_H)
# define _io_types_H (1)

#include <inttypes.h>

typedef enum {
  KR_FILE_ACCESS_ONLY,
  KR_FILE_ACCESS_APPEND,
  KR_FILE_ACCESS_MODIFY
} kr_file_access_mode;

typedef struct {
  kr_file_access_mode access_mode;
  uint16_t len;
  char path[512];
  uint64_t sz;
  uint64_t wrote;
  int remote_fs;
  int64_t lastmod;
  char type[26];
} kr_file2_info;

#include "gen/types_to_ebml.h"
#include "gen/types_from_ebml.h"
#include "gen/types_helpers.h"
#include "gen/types_to_text.h"
#include "gen/types_to_json.h"
#include "gen/types_from_json.h"

#endif
