#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <krad/gen/metainfo.h>
#include <krad/app/crate.h>
#include <krad/mem/mem.h>

void print_usage() {
  printf("Usage:\n  ./kr_sizeof \n");
}

int metacmp(const void *p1, const void *p2, void *user) {
  struct_info *m1;
  struct_info *m2;
  m1 = (struct_info *)p1;
  m2 = (struct_info *)p2;
  if (m1->sz > m2->sz) return 1;
  if (m1->sz < m2->sz) return -1;
  return 0;
}

int list_info() {
  int i;
  int j;
  const char *name;
  struct_info *meta_copy;
  meta_copy = kr_allocz(KR_META_INFO_LAST, sizeof(struct_info));
  j = 0;
  for (i = 1; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type == ST_STRUCT) {
      meta_copy[j] = meta_info[i];
      j++;
    }
  }
  qsort_r(meta_copy, j, sizeof(meta_copy[0]), metacmp, "bongo");
  for (i = 0; i < j; i++) {
    if (meta_copy[i].type == ST_STRUCT) {
      name = meta_copy[i].name;
      printf("%3d: %50s --> %8zu \n", i + 1, name , meta_copy[i].sz);
    }
  }
  free(meta_copy);
  return 0;
}

int info_name_to_meta_type(char *name) {
  int i;
  int val;
  const char *struct_name;
  char *prefix;
  char *suffix;
  val = atoi(name);
  if (val < 0 || val >= KR_META_INFO_LAST) return -1;
  if (val != 0) return val;
  for (i = 1; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type == ST_STRUCT) {
      struct_name = meta_info[i].name;
      prefix = strstr(meta_info[i].name, "kr_");
      suffix = strstr(meta_info[i].name, "_info");
      if (prefix) struct_name = meta_info[i].name + 3;
      if (suffix) {
        if (!strncmp(struct_name, name, suffix - struct_name))
          return i;
      } else {
        if (!strcmp(struct_name, name))
          return i;
      }
    }
  }
  return -1;
}

static int kr_sizeof(const char *args[], int n) {
  int ret;
  ret = list_info();
  printf("size of crate: %zu\n", sizeof(kr_crate));
  return ret;
}

int main(int argc, char const *argv[]) {
  int res;
  if (argc != 1 ) {
    print_usage();
    return -1;
  }
  res = kr_sizeof(&argv[1], argc - 1);
  if (res < 0) fprintf(stderr, "Failure!\n");
  return res;
}
