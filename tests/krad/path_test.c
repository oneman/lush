#include <stdio.h>
#include <alloca.h>
#include <krad/app/path.h>

static void path_print(kr_path *path);
static int test_path(char *str);
static int test_kr_path();

static void path_print(kr_path *path) {
  char *name;
  int len;
  int ret;
  if (path == NULL) {
    fprintf(stderr, "path_print called with null kr_path\n");
    return;
  }
  printf("printing %d steps\n", kr_path_steps(path));
  for (;;) {
    len = kr_path_cur_name(path, &name);
    printf("Component: (%d chars) %s\n", len, name);
    ret = kr_path_step(path);
    if (ret != 0) break;
  }
  len = kr_path_last(path, &name);
  printf("Last is (%d chars) %s\n", len, name);
}

static int test_path(char *str) {
  int ret;
  int len;
  kr_path *path;
  kr_path_alloca(path);
  len = strlen(str);
  printf("testing path %s\n", str);
  ret = kr_path_parse(path, str, len);
  if (ret != 0) {
    printf("kr_path_parse on %s returned %d\n", str, ret);
    return ret;
  }
  path_print(path);
  printf("rewind and again\n");
  kr_path_rewind(path);
  path_print(path);
  return 0;
}

static int test_kr_path() {
  char *str;
  int ret;
  str  = "/monkey/coconut";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/mixer/Music1/analog/drive";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/compositor/Video4";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/mixer/mic/volume/level";
  ret = test_path(str);
  if (ret != 0) return ret;
  str  = "/transponder/jackin";
  ret = test_path(str);
  if (ret != 0) return ret;
  return ret;
}

int main (int argc, char *argv[]) {
  int ret;
  ret = test_kr_path();
  return ret;
}
