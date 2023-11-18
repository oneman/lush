#if !defined(_app_patch_H)
# define _app_patch_H (1)

#define KR_NPATCHES 128

typedef struct kr_patch kr_patch;
typedef struct kr_patchset kr_patchset;

#include <stdint.h>
#include <unistd.h>
#include <krad/gen/info_types.h>

struct kr_patch {
  int16_t offset;
  int16_t sz;
  union {
    uint8_t value[8];
    float real;
    double realreal;
    int8_t i8;
    uint8_t u8;
    int16_t i16;
    uint16_t u16;
    int integer;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
  };
};

struct kr_patchset {
  kr_patch patch[KR_NPATCHES];
  size_t len;
  kr_info_type type;
  void *on;
};

int kr_patch_apply(void *info, kr_patch *patch);
int kr_patchset_apply(void *info, kr_patchset *patchset);
ssize_t kr_patchset_to_json(char *json, size_t sz, kr_patchset *ps);
int kr_json_to_patchset(kr_patchset *patchset, char *json, size_t len);
int kr_patchset_on(kr_patchset *ps, void *to, void *from, int type);
int kr_patchset_gen(kr_patchset *ps, void *info, int type);

#endif
