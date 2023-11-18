#if !defined(_io_scan_H)
# define _io_scan_H (1)

#include <sys/types.h>
#include "dir.h"
#include "types.h"

typedef struct kr_scan kr_scan;

typedef struct {
  uint8_t *mem;
  char path[PATH_MAX];
  size_t len;
} kr_scan_setup;

size_t kr_scan_path(kr_scan *scan, char **path);
int kr_scan_probe(kr_scan *scan, char *subdir);
int kr_scan_iter(kr_scan *scan, kr_dir_entry *entry);
int kr_scan_destroy(kr_scan *scan);
kr_scan *kr_scan_create(kr_scan_setup *setup);
size_t kr_scan_sizeof();

#endif
