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
  struct stat s;
  int fd;
  char name[PATH_MAX];
} kr_dir;

typedef struct {
  char *name;
  mode_t entry_mode;
  off_t sz;
  struct timespec lastmod;
  struct dirent *entry;
  struct stat entry_stat;
} kr_dir_entry;

int kr_dir_exists(char *path);
int kr_dir_create(char *path);

int kr_dir_entry_is_dir(kr_dir_entry *e);
int kr_dir_entry_is_file(kr_dir_entry *e);
char *kr_dir_current_dir_name(kr_dir *dir);

int kr_dir_get_entry(kr_dir *dir, kr_dir_entry *entry);
int kr_dir_iter(kr_dir *dir);

int kr_dir_close(kr_dir *dir);
int kr_dir_is_open(kr_dir *dir);
int kr_dir_open(kr_dir *dir, char *path, size_t len);

#endif
