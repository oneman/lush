#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <krad/io/dir.h>

int dir_test(char *path) {
  int ret;
  ret = -1;
  kr_dir dir;
  kr_dir_entry entry;
  ret = kr_dir_open(&dir, path, strlen(path));
  if (ret) return ret;
  for (;;) {
    ret = kr_dir_get_entry(&dir, &entry);
    if (ret == 0) break;
    if (ret != 1) {
      fprintf(stderr, "kr_dir_get_entry ret %d\n", ret);
      break;
    }
    printf("%s", entry.name);
    if (kr_dir_entry_is_file(entry.entry_mode)) {
      printf("\n%zd bytes\n", entry.sz);
    } else if (kr_dir_entry_is_dir(entry.entry_mode)) {
      printf("/");
    }
    printf("\n");
    kr_dir_iter(&dir);
  }
  ret = kr_dir_close(&dir);
  return ret;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 2) return -1;
  ret = dir_test((char *)argv[1]);
  if (ret != 0) {
    fprintf(stderr, "Failure.");
    return -1;
  }
  return 0;
}
