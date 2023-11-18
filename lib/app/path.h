#ifndef KRAD_PATH_H
#define KRAD_PATH_H

#include <inttypes.h>
#include <string.h>
#include <stddef.h>

#define KR_PATH_COMPONENT_MAX 16
#define KR_PATH_LENGTH_MAX 224

typedef struct kr_path kr_path;

size_t kr_path_sizeof(void);
#define kr_path_alloca(path) \
  path = alloca(kr_path_sizeof()); \
  memset(path, 0, kr_path_sizeof());
int kr_path_parse(kr_path *path, char *str, int len);
int kr_path_unparse(char *string, kr_path *path, size_t sz);
int kr_path_clear(kr_path *path);
int kr_path_step(kr_path *path);
int kr_path_next(kr_path *path, char **name);
int kr_path_steps(kr_path *path);
int kr_path_rewind(kr_path *path);
int kr_path_last(kr_path *path, char **name);
int kr_path_steps_ahead(kr_path *path);
int kr_path_steps_free(kr_path *path);
int kr_path_cur_name(kr_path *path, char **name);
char *kr_path_name_at(kr_path *path, int pos);
int kr_path_push(kr_path *path, char *str, int len);
int kr_path_clear_last(kr_path *path);
int kr_path_clear_first(kr_path *path);
size_t kr_path_to_str(char *string, kr_path *path, size_t sz);
int kr_path_join(kr_path *path, char *str, int len);
int kr_path_release(kr_path *path);

#endif
