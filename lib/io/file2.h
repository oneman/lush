#if !defined(_io_file2_H)
# define _io_file2_H (1)

#include <sys/types.h>
#include "types.h"

typedef struct kr_file2 kr_file2;
typedef struct kr_file_set kr_file_set;

typedef enum {
  KR_FILE_OPEN = 1,
  KR_FILE_CLOSE,
  KR_FILE_CREATE,
  KR_FILE_DATA,
  KR_FILE_WROTE
} kr_file_event_type;

typedef struct kr_file_event kr_file_event;
typedef int (kr_fs_event_cb)(kr_file_event *);

struct kr_file_event {
  kr_file_event_type type;
  void *user;
  kr_file2 *file;
};

typedef struct {
  int nfiles;
  void *user;
  kr_fs_event_cb *event_cb;
} kr_fs_setup;

int kr_file2_exists(char *path, size_t len);
uint8_t *kr_file2_get_data(kr_file2 *file);
int kr_file2_get_info(kr_file2 *file, kr_file2_info *info);
int kr_file2_unlink(kr_file2 *file);
int kr_file2_close(kr_file2 *file);
int kr_file2_truncate(kr_file2 *file, size_t sz);
int kr_file2_wrote(kr_file2 *file, size_t sz);
ssize_t kr_file2_append(kr_file2 *file, uint8_t *data, size_t sz);
ssize_t kr_file2_read(kr_file2 *file, uint8_t *data, size_t sz);
kr_file2 *kr_file2_open(kr_file_set *fs, char *path, size_t len);
kr_file2 *kr_file2_create(kr_file_set *fs, char *path, size_t len, size_t sz);
int kr_file_set_destroy(kr_file_set *fs);
kr_file_set *kr_file_set_create(kr_fs_setup *setup);

#endif
