#if !defined(_hash_hashids_H)
# define _hash_hashids_H (1)
#include <stdlib.h>
#include <stdint.h>

typedef struct hashids kr_hashids;

typedef struct {
  uint64_t id;
  char uid[16];
  uint32_t len;
} kr_hash_id;

typedef struct {
  void *mem;
  char salt[32];
  int len;
} kr_hashids_setup;

ssize_t kr_hashids_filename(char *name, size_t sz, size_t len,
 kr_hash_id *id);
ssize_t kr_hashids_filepath(char *name, size_t sz, size_t len,
 kr_hash_id *id);
size_t kr_hashids_sizeof();
kr_hashids *kr_hashids_init(kr_hashids_setup *setup);
int kr_hashids_gen(kr_hashids *hids, kr_hash_id *hid);

#endif
