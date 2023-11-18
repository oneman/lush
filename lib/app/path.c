#include "path.h"
#include <stdio.h>
#include <string.h>

struct kr_path {
  char full[KR_PATH_LENGTH_MAX + 1];
  int full_len;
  char *component[KR_PATH_COMPONENT_MAX];
  int len[KR_PATH_COMPONENT_MAX];
  int count;
  int cur;
  int join[KR_PATH_COMPONENT_MAX];
  int join_len[KR_PATH_COMPONENT_MAX];
  int join_count;
};

static int path_parse(kr_path *path, char *str, int len);

static int path_parse(kr_path *path, char *str, int len) {
  int i;
  int count;
  char *slash;
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 1) return -1;
  if (len > KR_PATH_LENGTH_MAX) return -1;
  /* has to start with slash */
  if (str[0] != '/') return -1;
  slash = NULL;
  count = 0;
  for (i = 0; i < len; i++) {
    /* sanity check, in case they lied about len */
    if (str[i] == '\0') return -1;
    if (path->full_len == KR_PATH_LENGTH_MAX) return -1;
    path->full[path->full_len] = str[i];
    if (path->full[path->full_len] == '/') {
      slash = &path->full[path->full_len];
      /* no consecutives / allowed */
      if (slash[0] == str[i+1]) return -1;
      slash[0] = '\0';
      /* fail if str ends with slash */
      if (i == (len - 1)) return -1;
      if (path->count == KR_PATH_COMPONENT_MAX) break;
      path->component[path->count] = slash + 1;
      path->count++;
      count++;
    } else {
      path->len[path->count - 1]++;
    }
    path->full_len++;
  }
  return count;
}

size_t kr_path_sizeof() {
  return sizeof(kr_path);
}

int kr_path_parse(kr_path *path, char *str, int len) {
  int ret;
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 1) return -1;
  if (len > KR_PATH_LENGTH_MAX) return -1;
  kr_path_clear(path);
  ret = path_parse(path, str, len);
  if (ret < 0) {
    kr_path_clear(path);
    return -1;
  }
  return 0;
}

int kr_path_unparse(char *string, kr_path *path, size_t sz) {
  int i;
  int res;
  if (path == NULL) return -1;
  if (string == NULL) return -1;
  if (sz <= 0) return -1;
  for (i = res = 0; i < path->count; i++) {
    res += snprintf(string + res, sz - res, "/");
    if (res >= sz) return -1;
    res += snprintf(string + res, sz - res, "%s", path->component[i]);
    if (res >= sz) return -1;
  }
  return 0;
}

int kr_path_clear(kr_path *path) {
  if (path == NULL) return -1;
  memset(path, 0, sizeof(*path));
  return 0;
}

int kr_path_step(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  if ((path->count - 1) == path->cur) return -1;
  if ((path->count - 1) < path->cur) return -2;
  if ((path->count - 1) > path->cur) path->cur++;
  return 0;
}

int kr_path_next(kr_path *path, char **name) {
  int ret;
  ret = kr_path_step(path);
  if (ret != 0) return ret;
  return kr_path_cur_name(path, name);
}

int kr_path_steps(kr_path *path) {
  if (path == NULL) return -1;
  return path->count;
}

int kr_path_rewind(kr_path *path) {
  if (path == NULL) return -1;
  path->cur = 0;
  return 0;
}

int kr_path_last(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  if (path->count < 1) return -1;
  *name = path->component[path->count - 1];
  return path->len[path->count - 1];
}

int kr_path_steps_ahead(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  return (path->count - 1) - path->cur;
}

int kr_path_steps_free(kr_path *path) {
  if (path == NULL) return -1;
  return KR_PATH_COMPONENT_MAX - path->count;
}

int kr_path_cur_name(kr_path *path, char **name) {
  if (path == NULL) return -1;
  if (name == NULL) return -1;
  if (path->count < 1) return -1;
  *name = path->component[path->cur];
  return path->len[path->cur];
}

char *kr_path_name_at(kr_path *path, int pos) {
  if (path == NULL) return NULL;
  if (pos < 0) return NULL;
  if (pos >= KR_PATH_COMPONENT_MAX) return NULL;
  return path->component[pos];
}

int kr_path_push(kr_path *path, char *str, int len) {
  if (path == NULL) return -1;
  if (str == NULL) return -1;
  if (len < 0) return -1;
  if (path->count == KR_PATH_COMPONENT_MAX) return -1;
  path->component[path->count] = str;
  path->len[path->count] = len;
  path->count++;
  return 0;
}

int kr_path_clear_last(kr_path *path) {
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  path->component[path->count - 1] = NULL;
  path->len[path->count - 1] = 0;
  path->count--;
  return 0;
}

int kr_path_clear_first(kr_path *path) {
  int i;
  if (path == NULL) return -1;
  if (path->count < 1) return -1;
  path->count--;
  for (i = 0; i < path->count; i++) {
    path->component[i] = path->component[i + 1];
    path->len[i] = path->len[i + 1];
  }
  return 0;
}

size_t kr_path_to_str(char *string, kr_path *path, size_t sz) {
  size_t res;
  int i;
  int j;
  if (path == NULL || string == NULL || sz == 0) return 0;
  res = 0;
  res += snprintf(&string[res], sz - res, "Path of %d elements:\n",
   path->count);
  if (res >= sz) return sz;
  for (i = 0; i < path->count; i++) {
    res += snprintf(&string[res], sz - res, "  %s\n", path->component[i]);
    if (res >= sz) return sz;
  }
  if (path->join_count > 0) {
    res += snprintf(&string[res], sz - res,
     "\nPath contains %d joined subpath(s)\n", path->join_count+1);
    if (res >= sz) return sz;
    res += snprintf(&string[res], sz - res, "  0. ");
    if (res >= sz) return sz;
    for (i = 0; i < path->join[0]; i++) {
      res += snprintf(&string[res], sz - res, "/%s", path->component[i]);
      if (res >= sz) return sz;
    }
    res += snprintf(&string[res], sz - res, "\n");
    if (res >= sz) return sz;
    for (i = 0; i < path->join_count; i++) {
      res += snprintf(&string[res], sz - res, "  %d. ", i + 1);
      if (res >= sz) return sz;
      for (j = 0; j < path->join_len[i]; j++) {
        res += snprintf(&string[res], sz - res, "/%s",
         path->component[path->join[i] + j]);
        if (res >= sz) return sz;
      }
      res += snprintf(&string[res], sz - res, "\n");
      if (res >= sz) return sz;
    }
  }
  return res;
}

int kr_path_join(kr_path *path, char *str, int len) {
  int count;
  int fl;
  int cnt;
  int i;
  if (path == NULL) return -1;
  fl = path->full_len;
  cnt = path->count;
  count = path_parse(path, str, len);
  if (count < 0) {
    for (i = cnt; i < path->count; i++) {
      path->len[i] = 0;
      path->component[i] = NULL;
    }
    path->full_len = fl;
    path->count = cnt;
    return -1;
  }
  path->join[path->join_count] = cnt;
  path->join_len[path->join_count] = count;
  path->join_count++;
  return 0;
}

int kr_path_release(kr_path *path) {
  int i;
  if (path == NULL) return -1;
  if (path->join_count == 0) return 0;
  for (i = path->join[--path->join_count]; i < path->count; i++) {
    path->len[i] = 0;
    path->component[i] = NULL;
  }
  path->count -= path->join_len[path->join_count];
  path->join[path->join_count] = 0;
  return 0;
}
