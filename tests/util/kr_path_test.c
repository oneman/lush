#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <krad/app/path.h>
#include <krad/app/debug.h>

int run_test() {
  int res;
  int len;
  char string[4096];
  kr_path *path;
  char *addr1 = "/root/letters";
  char *addr2 = "/a/b";
  char *addr3 = "/c/d/e";
  char *addr4 = "/f";
  kr_path_alloca(path);
  len = strlen(addr1);
  res = kr_path_parse(path, addr1, len);
  if (res < 0) return -1;
  res = kr_path_to_str(string, path, sizeof(string));
  if (res > 0)
    printk("%s",string);
  len = strlen(addr2);
  res = kr_path_join(path, addr2, len);
  if (res < 0) return -1;
  len = strlen(addr3);
  res = kr_path_join(path, addr3, len);
  if (res < 0) return -1;
  len = strlen(addr4);
  res = kr_path_join(path, addr4, len);
  if (res < 0) return -1;
  res = kr_path_to_str(string, path, sizeof(string));
  if (res > 0)
    printk("%s",string);
  kr_path_release(path);
  res = kr_path_to_str(string, path, sizeof(string));
  if (res > 0)
    printk("%s",string);
  kr_path_release(path);
  res = kr_path_to_str(string, path, sizeof(string));
  if (res > 0)
    printk("%s",string);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  ret = run_test();
  return ret;
}