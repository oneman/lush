#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/ebml/ebml.h>
#include <krad/util/util.h>
#include <krad/gen/metainfo.h>
#include "json/pretty.h"
#define MAX_JSON_LEN 1024 * 1024

void print_usage() {
  printf("Usage:\n  kr_jschema struct1,struct2,struct3,... \n"
    "Example usages:\n  kr_jschema mixer_path,v4l2\n");
  printf("  kr_jschema 50,7 -p\n");
  printf("kr_jschema list ---> list of all possible info structs\n");
}

int list_info() {
  int i;
  const char *name;
  char *prefix;
  char *suffix;
  for (i = 1; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type == ST_STRUCT) {
      name = meta_info[i].name;
      prefix = strstr(meta_info[i].name, "kr_");
      suffix = strstr(meta_info[i].name, "_info");;
      if (prefix) name = meta_info[i].name + 3;
      if (suffix)
        printf("[%d] %.*s\n", i, (int)(suffix - name) , name);
      else
        printf("[%d] %s\n", i, name);
    }
  }
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
  for (i = 0; i < KR_META_INFO_LAST; i++) {
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

int kr_jschema_gen(char *js, int type) {
  char *json;
  int res;
  int pos;
  pos = 0;
  json = calloc(1, 8192 * 8);
  res = metainfo_to_jschema(json, 8192 * 8, type);
  if (res >= (8192 * 8)) return -1;
  pos += sprintf(&js[pos], "%s", json);
  free(json);
  return pos;
}

int kr_jschema(char const *args[], int n) {
  int i;
  int res;
  int pos;
  int pretty;
  int all;
  char *comma;
  char *str;
  char *name;
  char *json;
  pos = 0;
  pretty = 0;
  all = 0;
  if (!strncmp(args[0], "list", 4)) {
    return list_info();
  }
  if (!strncmp(args[0], "all", 3)) {
    all = 1;
  }
  for (i = 0; i < n; i++) {
    if (!strncmp(args[i], "-p", 2)) {
      pretty = 1;
    }
  }
  json = malloc(MAX_JSON_LEN * sizeof(char));
  if (all) {
    pos += sprintf(&json[pos], "{\"oneOf\": [");
    for (i = 0; i < KR_META_INFO_LAST; i++) {
      if (meta_info[i].type == ST_STRUCT) {
        res = kr_jschema_gen(&json[pos], i);
        if (res < 0) return -1;
        pos += res;
        pos += sprintf(&json[pos], ",");
      }
    }
    if (json[pos - 1] == ',') --pos;
    pos += sprintf(&json[pos],"]}\n");
  } else {
    str = strdup(args[0]);
    if (str == NULL) return -1;
    comma = ",";
    name = strtok(str, comma);
    if (name == NULL) return -1;
    res = info_name_to_meta_type(name);
    if (res <= 0) return -1;
    pos += sprintf(&json[pos], "{\"oneOf\": [");
    res =  kr_jschema_gen(&json[pos], res);
    if (res < 0) return -1;
    pos += res;
    while ( (name = strtok(NULL, comma)) != NULL ) {
      res = info_name_to_meta_type(name);
      if (res <= 0) return -1;
      pos += sprintf(&json[pos], ",");
      pos += kr_jschema_gen(&json[pos], res);
      if (res < 0) return -1;
    }
    pos += sprintf(&json[pos],"]}\n");
    free(str);
  }
  if (pretty && !all) {
    pretty_json(json);
  } else {
    printf("%s", json);
  }
  free(json);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  if (argc != 2 && argc != 3) {
    print_usage();
    return -1;
  }
  res = kr_jschema(&argv[1], argc - 1);
  if (res < 0) fprintf(stderr, "Failure!\n");
  return res;
}
