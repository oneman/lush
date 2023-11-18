#include <string.h>
#include <stdio.h>
#include <alloca.h>
#include <krad/app/debug.h>
#include <krad/gen/metainfo.h>
#include <krad/gen/info_types.h>
#include <krad/util/util.h>
#include <krad/mem/mem.h>
#include <krad/app/path.h>
#include <krad/json/parser.h>
#include <krad/util/strton.h>
#include "patch.h"

static int patch_to_str_value(char *val, size_t sz,
 kr_patch *patch, int type) {
  switch (type) {
    case T_CHAR:
      snprintf(val, sz, "\"%.*s\"", patch->sz, patch->value);
      break;
    case T_INT8:
      snprintf(val, sz, "%d", patch->i8);
      break;
    case T_INT16:
      snprintf(val, sz, "%d", patch->i16);
      break;
    case T_INT32:
      snprintf(val, sz, "%d", patch->integer);
      break;
    case T_INT64:
      snprintf(val, sz, "%"PRIi64, patch->i64);
      break;
    case T_UINT8:
      snprintf(val, sz, "%u", patch->u8);
      break;
    case T_UINT16:
      snprintf(val, sz, "%u", patch->u16);
      break;
    case T_UINT32:
      snprintf(val, sz, "%u", patch->u32);
      break;
    case T_UINT64:
      snprintf(val, sz, "%"PRIu64"", patch->u64);
      break;
    case T_FLOAT:
      snprintf(val, sz, "%0.2f", patch->real);
      break;
    case T_DOUBLE:
      snprintf(val, sz, "%0.2f", patch->realreal);
      break;
    default: return -1;
  }
  return 0;
}

static int json_patch_replace_parse(kr_patch *patch,
  int type, kr_jps *jps) {
  int mtype;
  int ret;
  kr_path *path;
  kr_path_alloca(path);
  if (jps_curr_type(jps) != JS_STRING) return -1;
  if (strncmp(jps_curr_start(jps), "path", 4)) return -2;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -3;
  printkd("JSON PATCH: parse path is %s", jps_curr_start(jps));
  kr_path_parse(path, jps_curr_start(jps), jps_curr_len(jps));
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -4;
  if (strncmp(jps_curr_start(jps), "value", 5)) return -5;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_NUMBER
    && jps_curr_type(jps) != JS_STRING) return -6;
  printkd("JSON PATCH: value is %s", jps_curr_start(jps));
  mtype = kr_info_type_to_metainfo_type(type);
  ret = path_get_type(path, mtype);
  if (ret == T_ENUM) {
    if (jps_curr_type(jps) != JS_STRING) return -7;
    ret = path_enum_val(path, jps_curr_start(jps), jps_curr_len(jps), mtype);
    if (ret >= 0) {
      patch->integer = ret;
      ret = info_functions[type].patch_path(patch, path);
      if (ret == 0) printkd("JSON PATCH: patch path success");
      else printke("JSON PATCH failed");
      return ret;
    }
  }
  switch (ret) {
  case T_CHAR:
    break;
  case T_INT8:
    ret = kr_strtoi8(&patch->i8, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_INT16:
    ret = kr_strtoi16(&patch->i16, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_INT32:
    ret = kr_strtoi32(&patch->integer, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_INT64:
    ret = kr_strtoi64(&patch->i64, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_UINT8:
    ret = kr_strtou8(&patch->u8, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_UINT16:
    ret = kr_strtou16(&patch->u16, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_UINT32:
    ret = kr_strtou32(&patch->u32, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_UINT64:
    ret = kr_strtou64(&patch->u64, jps_curr_start(jps), jps_curr_len(jps));
    break;
  case T_FLOAT:
    patch->real = atof(jps_curr_start(jps));
    break;
  case T_DOUBLE:
    patch->real = atof(jps_curr_start(jps));
    break;
  default:
    printke("JSON PATCH failed: wrong type! %d", ret);
    return -7;
  }
  ret = info_functions[type].patch_path(patch, path);
  if (ret == 0) printkd("JSON PATCH: patch path success");
  else printke("JSON PATCH failed");
  return ret;
}

static int json_patch_op_parse(kr_patch *patch, int type,
 kr_jps *jps) {
  int res;
  if (jps_curr_type(jps) != JS_STRING) return -1;
  printkd("JSON PATCH: op is %s",jps_curr_start(jps));
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
  if (jps_token_count(jps) < 7) return -2;
  if (jps_curr_type(jps) != JS_OBJECT) return -3;
  jps_next(jps);
  if (jps_curr_type(jps) != JS_STRING) return -4;
  if (strncmp(jps_curr_start(jps), "op", 2)) return -5;
  jps_next(jps);
  res = json_patch_op_parse(patch, type, jps);
  jps_rewind(jps);
  return res;
}

int kr_json_to_patchset(kr_patchset *patchset, char *json, size_t len) {
  kr_jps *jps;
  int i;
  int res;
  kr_jps_alloca(jps);
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  if (jps_curr_type(jps) != JS_ARRAY) jps_next(jps);
  patchset->len = jps_curr_size(jps);
  printkd("JSON PATCH: ok parsed");
  for (i = 0; i < patchset->len; i++) {
    jps_rewind(jps);
    jps_array_at(jps, i);
    res = json_patch_parse(&patchset->patch[i], patchset->type,
     jps_curr_start(jps));
    if (res < 0) return res;
  }
  return patchset->len;
}

int kr_patch_apply(void *info, kr_patch *patch) {
  if (!info || !patch) return -1;
  memcpy((char *)info + patch->offset, &patch->value, patch->sz);
  return 0;
}

int kr_patchset_apply(void *info, kr_patchset *patchset) {
  int i;
  int ret;
  if (!info || !patchset) return -1;
  for (i = 0; i < patchset->len; i++) {
    ret = kr_patch_apply(info, &patchset->patch[i]);
    if (ret) return -2;
  }
  patchset->on = info;
  return 0;
}

int kr_patchset_on(kr_patchset *ps, void *to, void *from, int type) {
  int ret;
  int mtype;
  kr_diffset ds;
 //  kr_info init_info;
  if (!ps || !to || !from || !type) return -1;
  memset(&ds, 0, sizeof(ds));
  //memset(&init_info, 0, sizeof(init_info));
  mtype = kr_info_type_to_metainfo_type(type);
  if (mtype < 0) return -2;
  //ret = info_helpers(&init_info, type, 1); /* init function */
  //if (ret) return -3;
  ret = info_diff(&ds, to, from, mtype);
  if (ret) return -4;
  ret = kr_diffset_to_patchset(ps, &ds);
  if (ret) return -5;
  return 0;
}

int kr_patchset_gen(kr_patchset *ps, void *info, int type) {
  //int ret;
  kr_diffset ds;
  kr_info init_info;
  if (!ps || !info || !type) return -1;
  memset(&ds, 0, sizeof(ds));
  memset(&init_info, 0, sizeof(init_info));
  //type = kr_info_type_to_metainfo_type(type);
  //if (type < 0) return -2;
  //ret = info_helpers(&init_info, type, 1); /* init function */
  //if (ret) return -3;
  return kr_patchset_on(ps, info, &init_info, type);
}

ssize_t kr_patchset_to_json(char *json, size_t sz, kr_patchset *ps) {
  int i;
  int type;
  ssize_t ret;
  int res;
  char *str;
  char path[256];
  char value[128];
  kr_path *kpath;
  if (!json || !sz || !ps) return -1;
  if (!ps->on) {
    printke("Patchset to JSON: ps->on is null");
    return -11;
  }
  memset(path, 0, sizeof(path));
  ret = 0;
  str = NULL;
  ret += snprintf(json + ret, sz, "[");
  if (ret >= sz) return -2;
  sz -= ret;
  type = kr_info_type_to_metainfo_type(ps->type);
  if (ret > 0) {
    for (i = 0; i < ps->len; i++) {
      res = patch_get_path(path, sizeof(path), &ps->patch[i], type, ps->on);
      if (res < 0) printke("Patchset to JSON: patch_get_path failed.");
      if (res < 0) return -5;
      if (res == T_ENUM) {
        kr_path_alloca(kpath);
        kr_path_parse(kpath, path, strlen(path));
        str = path_enum_str(kpath, ps->patch[i].integer, type);
        if (!str) return -3;
        res = snprintf(value, sizeof(value), "\"%s\"", str);
        if (res >= sizeof(value)) return -4;
        str = value;
      } else {
        res = patch_to_str_value(value, sizeof(value), &ps->patch[i], res);
        if (res < 0) break;
        str = value;
      }
      ret += snprintf(json + ret, sz, "{\"op\":\"replace\","
        "\"path\": \"%s\", \"value\": %s}", path, str);
      str = NULL;
      if (ret >= sz) break;
      sz -= ret;
      if (i != (ps->len - 1)) {
        ret += snprintf(json + ret, sz, ",");
        if (ret >= sz) break;
        sz -= ret;
      }
    }
  }
  ret += snprintf(json + ret, sz, "]");
  if (ret >= sz) {
    printke("Patchset to JSON: sz limit: %zu ret %d", sz, ret);
    return -5;
  }
  return ret;
}
