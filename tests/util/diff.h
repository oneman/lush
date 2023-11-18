#if !defined(_app_diff_H)
# define _app_diff_H (1)

#include <stdint.h>

#define MAX_STR_LEN 256

typedef struct kr_diff kr_diff;
typedef struct kr_diffset kr_diffset;

#define KR_NDIFFS 128

enum diff_type {
  DIFF_STRING = 1,
  DIFF_INTEGER,
  DIFF_REAL,
  DIFF_ENUM
};

struct kr_diff {
  char path[MAX_STR_LEN];
  size_t sz;
  size_t off;
  int type;
  int enum_idx;
};

struct kr_diffset {
  kr_diff diffs[KR_NDIFFS];
  size_t len;
};

#endif
