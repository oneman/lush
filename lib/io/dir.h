#if !defined(_io_dir_H)
# define _io_dir_H (1)

#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

typedef struct {
  DIR *dh;
  long pos;
} kr_dir;

typedef struct {
  char *name;
  mode_t entry_mode;
  off_t sz;
  struct timespec lastmod;
  struct dirent entry;
  struct stat entry_stat;
} kr_dir_entry;

#define kr_dir_entry_is_file S_ISREG
#define kr_dir_entry_is_dir S_ISDIR

int kr_dir_exists(char *path);
int kr_dir_create(char *path);

int kr_dir_get_entry(kr_dir *dir, kr_dir_entry *entry);
int kr_dir_iter(kr_dir *dir);
int kr_dir_close(kr_dir *dir);
int kr_dir_is_open(kr_dir *dir);
int kr_dir_open(kr_dir *dir, char *path, size_t len);

size_t kr_dir_sizeof();

#endif
