#if !defined(_ds_types_H)
# define _ds_types_H (1)

typedef struct {
  uint8_t sha256[64];
  char path[512];
  char name[512];
  char ext[16];
  uint64_t sz;
} kr_ds_file_info;

typedef struct {
  char name[128];
  char id[64];
  uint64_t used;
  uint64_t size;
  uint64_t nfiles;
  uint64_t lastmod;
} kr_ds_volume_info;

typedef enum {
  KR_DS_MOUNT_MASTER = 1,
  KR_DS_MOUNT_COPY
} kr_ds_mount_type;

typedef struct {
  kr_ds_mount_type type;
  kr_ds_volume_info info;
} kr_ds_mount_info;

#define KR_DS_NMOUNTS 16

typedef struct {
  int nmounts;
  kr_ds_mount_info mount[KR_DS_NMOUNTS];
} kr_ds_mounts_info;

#endif
