#include <stdio.h>
#include <stdlib.h>
#include <krad/io/file.h>
#include <krad/app/debug.h>
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/json/parser.h>
#include <krad/app/crate.h>
#include <krad/mixer/types.h>

static void print_usage(char *cmd) {
  printf("Usage: %s input.json\n",cmd);
}

static int json_array_len(char *json) {
  kr_jps *jps;
  kr_jps_alloca(jps);
  int res;
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return 0;
  return jps_curr_size(jps);
}

static int json_patch_replace_parse(kr_patch *patch,
  int type, kr_jps *jps) {
  int ret;
  kr_path *path;
  kr_path_alloca(path);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  if (strncmp(jps_curr_start(jps), "path", 4)) return -1;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  printk("path is %s", jps_curr_start(jps));
  kr_path_parse(path, jps_curr_start(jps), jps_curr_len(jps));
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  if (strncmp(jps_curr_start(jps), "value", 5)) return -1;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_NUMBER) return -1;
  printk("value is %s", jps_curr_start(jps));
  if (strchr(jps_curr_start(jps), '.')) {
    patch->real = atof(jps_curr_start(jps));
  } else {
    patch->integer = atoi(jps_curr_start(jps));
  }
  ret = info_functions[type].patch_path(patch, path);
  printk("res %d",ret);
  if (ret == 0) printk("patch path success");
  return ret;
}

static int json_patch_op_parse(kr_patch *patch, int type,
 kr_jps *jps) {
  int res;
  if (jps_curr_type(jps) != JS_STRING) return -1;
  printk("op is %s",jps_curr_start(jps));
  if (!strncmp(jps_curr_start(jps),"replace",6)) {
    jps_next(jps);
    res = json_patch_replace_parse(patch, type, jps);
    if (res < 0) return res;
  }
  return 0;
}

static int json_patch_parse(kr_patch *patch, int type, char *json) {
  kr_jps *jps;
  int res;
  kr_jps_alloca(jps);
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  if (jps_token_count(jps) < 7) return -1;
  if (jps_curr_type(jps) != JS_OBJECT) return -1;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  if (strncmp(jps_curr_start(jps), "op", 2)) return -1;
  jps_next(jps);
  res = json_patch_op_parse(patch, type, jps);
  jps_rewind(jps);
  return res;
}

static int json_patchset(kr_patchset *patchset, kr_path *path, char *json, size_t len) {
  kr_jps *jps;
  int i;
  int res;
  kr_jps_alloca(jps);
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return 0;
  printk("ok parsed");
  for (i = 0; i < patchset->len; i++) {
    jps_rewind(jps);
    jps_array_at(jps, i);
    res = json_patch_parse(&patchset->patch[i], patchset->type, jps_curr_start(jps));
    if (res < 0) return 0;
  }
  return patchset->len;
}

static int run_test(char *filename) {
  kr_file *file;
  kr_patchset pset;
  kr_path *path;
  kr_mixer_path_info info;
  char *address = "/mixer";
  char json[8092];
  char string[4096];
  int res;
  int len;
  size_t read;
  if (filename == NULL) return -1;
  memset(&pset, 0, sizeof(pset));
  memset(&info, 0, sizeof(info));
  file = kr_file_open(filename);
  if (file == NULL) return -1;
  printk("%s opened.",filename);
  memset(json, 0, sizeof(json));
  read = kr_file_read(file, json, sizeof(json));
  if (read <= 0) return -1;
  printk("read %d bytes.",read);
  len = json_array_len(json);
  if (len <= 0 || len > KR_NPATCHES) return -1;
  printk("read json array of %d elements.",len);
  pset.len = len;
  pset.type = KR_MIXER_PATH_INFO;
  kr_path_alloca(path);
  len = strlen(address);
  res = kr_path_parse(path, address, len);
  if (res < 0) return -1;
  printk("address parsed.");
  res = json_patchset(&pset, path, json, read);
  if (res != pset.len) return -1;
  printk("Ok! Found and converted %d patches!",res);
  info.type = KR_MXR_SOURCE;
  info.channels = KR_STEREO;
  res = info_functions[pset.type].to_text(string, &info, sizeof(string));
  if (res <= 0) return -1;
  printk("%s",string);
  kr_patchset_apply(&info, &pset);
  printk("Patchset applied!\n");
  res = info_functions[pset.type].to_text(string, &info, sizeof(string));
  if (res <= 0) return -1;
  printk("%s",string);
  kr_file_close(&file);
  return 0;
}

int main (int argc, char *argv[]) {
  int ret;
  if (argc != 2) {
    print_usage(argv[0]);
    return 1;
  }
  ret = run_test(argv[1]);
  return ret;
}
