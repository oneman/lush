#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/ebml/ebml.h>
#include <krad/util/util.h>
#include <krad/gen/metainfo.h>
#include "json/pretty.h"
#define MAX_JSON_LEN 1024 * 1024

void print_usage() {
  printf("Usage:\n  kr_json struct1,struct2,struct3,... [-r] [-p]\n"
    "Example usages:\n  kr_json kr_mixer_path_info,kr_v4l2_info\n");
  printf("  kr_json 50,7 -r -p\n");
  printf("kr_json list ---> list of all possible info structs\n");
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
      suffix = strstr(meta_info[i].name, "_info");
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
  int len;
  int slen;
  const char *struct_name;
  char *prefix;
  char *suffix;
  val = atoi(name);
  len = strlen(name);
  if (val < 0 || val >= KR_META_INFO_LAST) return -1;
  if (val != 0) return val;
  for (i = 1; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type == ST_STRUCT) {
      struct_name = meta_info[i].name;
      prefix = strstr(meta_info[i].name, "kr_");
      suffix = strstr(meta_info[i].name, "_info");
      if (prefix) struct_name = meta_info[i].name + 3;
      slen = strlen(struct_name);
      if (suffix) slen -= 5;
      if (len != slen) continue;
      if (!strncmp(struct_name, name, len)) return i;
    }
  }
  return -1;
}

int json_init(char *json, int max, int idx, int type) {
  int res;
  kr_info info;
  memset(&info, 0, sizeof(info));
  if (!type) {
    res = info_helpers(&info, idx, 1);
    if (res != 0) return -1;
  } else {
    res = info_helpers(&info, idx, 3);
    if (res != 0) return -1;
  }
  res = info_pack_json(json, &info, max, idx);
  return res;
}

int kr_json(const char *args[], int n) {
  int i;
  int res;
  int pos;
  int max;
  int type;
  int pretty;
  int all;
  char *comma;
  char *str;
  char *name;
  char *json;
  srand(time(NULL));
  all = 0;
  pretty = 0;
  type = 0;
  max = MAX_JSON_LEN - 1;
  pos = 0;
  if (!strncmp(args[0], "list", 4)) {
    return list_info();
  }
  if (!strncmp(args[0], "all", 3)) {
    all = 1;
  }
  for (i = 0; i < n; i++) {
    if (!strncmp(args[i], "-r", 2)) {
      type = 1;
    }
    if (!strncmp(args[i], "-p", 2)) {
      pretty = 1;
    }
  }
  json = malloc(MAX_JSON_LEN * sizeof(char));
  if (all) {
    pos += snprintf(&json[pos], max - pos, "[");
    if (pos >= max) return -1;
    for (i = 0; i < KR_META_INFO_LAST; i++) {
      if (meta_info[i].type == ST_STRUCT) {
        pos += json_init(&json[pos], max - pos, i, type);
        if (pos >= max) return -1;
        pos += snprintf(&json[pos], max - pos, ",");
        if (pos >= max) return -1;
      }
    }
    if (json[pos] == ',') --pos;
    pos += snprintf(&json[pos], max - pos, "]");
    if (pos >= max) return -1;
  } else {
    str = strdup(args[0]);
    if (str == NULL) return -1;
    comma = ",";
    name = strtok(str, comma);
    if (name == NULL) return -1;
    res = info_name_to_meta_type(name);
    if (res <= 0) return -1;
    //pos += snprintf(&json[pos], max - pos, "{\"%s\":", meta_info[res].name);
    if (pos >= max) return -1;
    pos += json_init(&json[pos], max - pos, res, type);
    if (pos >= max) return -1;
    while ( (name = strtok(NULL, comma)) != NULL ) {
      pos += snprintf(&json[pos], max - pos , ",");
      if (pos >= max) return -1;
      res = info_name_to_meta_type(name);
      if (res <= 0) return -1;
      //pos += snprintf(&json[pos], max - pos, "\"%s\":", meta_info[res].name);
      pos += json_init(&json[pos], max - pos, res, type);
      if (pos >= max) return -1;
    }
    free(str);
  }
  //pos += snprintf(&json[pos], max - pos , "}\n");
  if (pretty && !all) {
    res = pretty_json(json);
  } else {
    printf("%s\n", json);
  }
  free(json);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  if (argc < 2 || argc > 4) {
    print_usage();
    return -1;
  }
  res = kr_json(&argv[1], argc - 1);
  if (res < 0) fprintf(stderr, "Failure!\n");
  return res;
}
