#include <krad/json/parser.h>
#include <krad/gen/metainfo.h>
#include <krad/util/strton.h>

static int is_member_primitive(const member_info *memb) {
  switch(memb->type) {
    case T_STRUCT: return 0;
    case T_ASTRUCT: return 0;
    case T_ENUM: return 0;
    case T_UNION: return 0;
    case T_AUNION: return 0;
    case T_ANON: return 0;
    default: break;
  }
  return 1;
}

static int memb_jps_tk_type (const member_info *memb) {
  if (memb->arr && memb->type != T_CHAR) return JS_ARRAY;
  switch(memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        return JS_STRING;
      }
      break;
    }
    case T_INT8: return JS_NUMBER;
    case T_INT16: return JS_NUMBER;
    case T_INT32: return JS_NUMBER;
    case T_INT64: return JS_NUMBER;
    case T_UINT8: return JS_NUMBER;
    case T_UINT16: return JS_NUMBER;
    case T_UINT32: return JS_NUMBER;
    case T_UINT64: return JS_NUMBER;
    case T_FLOAT: return JS_NUMBER;
    case T_DOUBLE: return JS_NUMBER;
    case T_STRUCT: return JS_OBJECT;
    case T_ASTRUCT: return -1;
    case T_ENUM: return JS_STRING;
    case T_UNION: return JS_OBJECT;
    case T_AUNION: return JS_OBJECT;
    case T_ANON: return JS_OBJECT;
    default: return -1;
  }
  return -1;
}

static int find_union(kr_jps *jps, const member_info *memb) {
  int val;
  jps_rewind_parent(jps);
  do {
    if (jps_curr_type(jps) == JS_STRING) {
      val = str_to_enum(memb->type_info.substruct_info.idx, jps_curr_start(jps));
      if (val >= 0) {
        jps_next(jps);
        return val;
      }
    }
  } while (jps_next_key_in_curr_obj(jps) == 0);
  return -1;
}

static int find_json_member(kr_jps *jps, const member_info *memb) {
  int type;
  jps_rewind_parent(jps);
  do {
  if (jps_curr_type(jps) == JS_STRING) {
      if (!strcmp(jps_curr_start(jps), memb->name)){
        if (jps_next(jps) < 0) return 0;
        type = memb_jps_tk_type(memb);
        if (jps_curr_type(jps) == type) {
          //printf("found %s at %d\n", memb->name, jps_curr(jps));
          return 1;
        } else {
          printf("JSON Unpack: wrong type for %s!\n", memb->name);
        }
      }
    }
  } while (jps_next_key_in_curr_obj(jps) == 0);
  if (memb->opt) {
    //printf("JSON Unpack: Missing %s but no worries, it's optional :p\n", memb->name);
    jps_rewind_parent(jps);
    return 0;
  }
  printf("JSON Unpack: Missing %s! %s Failure.\n", memb->name, jps_curr_start(jps));
  jps_rewind(jps);
  return -1;
}

static int handle_primitive(void *info, const member_info *memb, kr_jps *jps) {
  int res;
  int i;
  int arr_len;
  char *actual;
  actual = (char *)info;
  res = find_json_member(jps, memb);
  arr_len = 1;
  if (memb->arr) arr_len = memb->arr;
  if (res < 0) return -1;
  if (res == 0) return 0;
  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        strncpy(&actual[memb->off], jps_curr_start(jps), memb->arr);
      } else  if (memb->ptr) {
        break;
      } else {
        strncpy(&actual[memb->off], jps_curr_start(jps), sizeof(char));
      }
      break;
    }
    case T_INT8: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtoi8((int8_t *)(actual + memb->off + (i * sizeof(int8_t))),
         jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_INT16: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtoi16((int16_t *)(actual + memb->off + (i * sizeof(int16_t))),
         jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_INT32: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtoi32((int32_t *)(actual + memb->off + (i * sizeof(int32_t))),
         jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_INT64: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtoi64((int64_t *)(actual + memb->off + (i * sizeof(int64_t))),
         jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_UINT8: {
      /* checking for booleans */
      for (i = 0; i < arr_len; i++) {
        if (!strncmp(jps_curr_start(jps), "true", 4)) {
          if (arr_len > 1) jps_next(jps);
          *((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t)))) = 1;
        } else if (!strncmp(jps_curr_start(jps), "false", 5)) {
          if (arr_len > 1) jps_next(jps);
          *((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t)))) = 0;
        } else {
          if (arr_len > 1) jps_next(jps);
          res = kr_strtou8((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t))),
            jps_curr_start(jps), jps_curr_len(jps));
          if (res != 0) return -1;
        }
      }
      break;
    }
    case T_UINT16:{
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtou16((uint16_t *)(actual + memb->off + (i * sizeof(uint16_t))),
          jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_UINT32: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtou32((uint32_t *)(actual + memb->off + (i * sizeof(uint32_t))),
          jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_UINT64: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        res = kr_strtou64((uint64_t *)(actual + memb->off + (i * sizeof(uint64_t))),
          jps_curr_start(jps), jps_curr_len(jps));
        if (res != 0) return -1;
      }
      break;
    }
    case T_FLOAT: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        *((float *)(actual + memb->off + (i * sizeof(float)))) = atof(jps_curr_start(jps));
      }
      break;
    }
    case T_DOUBLE: {
      for (i = 0; i < arr_len; i++) {
        if (arr_len > 1) jps_next(jps);
        *((double *)(actual + memb->off + (i * sizeof(double)))) = atof(jps_curr_start(jps));
      }
      break;
    }
    default: break;
  }
  if (arr_len > 1) jps_next(jps);
  return 0;
}

static int handle_members(void *act, const struct_info *info, kr_jps *jps) {
  int i;
  int j;
  int res;
  int ret;
  int index;
  int union_idx;
  int tk;
  char *actual;
  struct_info dummy;
  actual = (char *)act;
  res = 0;
  for (i = 0; i < info->member_count; i++) {
    if (is_member_primitive(&info->members[i])) {
      ret = handle_primitive(act, &info->members[i], jps);
      if (ret < 0) return -1;
    }
    if (info->members[i].type == T_STRUCT) {
      ret = find_json_member(jps, &info->members[i]);
      if (ret < 0) return -1;
      if (ret == 0) continue;
      index = info->members[i].type_info.substruct_info.idx;
      tk = jps_curr(jps);
      if (info->members[i].arr) {
        for (j = 0; jps_array_at(jps, j) == 0; j++) {
          jps_next(jps);
          if (j >= info->members[i].arr) return -1;
          res = handle_members(&actual[info->members[i].off + (meta_info[index].sz * j)], &meta_info[index], jps);
          if (res < 0) return -1;
          jps_rewind_at(jps, tk);
        }
      } else {
        jps_next(jps);
        res = handle_members(&actual[info->members[i].off], &meta_info[index], jps);
        if (res < 0) return -1;
      }
      jps_rewind_at(jps, tk);
      jps_rewind_parent(jps);
    }
    if (info->members[i].type == T_ENUM) {
      if (i != (info->member_count - 1) &&
       (info->members[i+1].type == T_AUNION ||
        info->members[i+1].type == T_UNION)) continue;
      index = info->members[i].type_info.subenum_info.idx;
      ret = find_json_member(jps, &info->members[i]);
      if (ret == 0) continue;
      if (ret < 0) return -1;
      ret = str_to_enum(index, jps_curr_start(jps));
      if (ret < 0) return -1;
      if (ret == 0) continue;
      *((int32_t *)(actual + info->members[i].off)) = ret;
    }
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      if (meta_info[index].member_count > 0) {
        res = handle_members(actual, &meta_info[index], jps);
        if (res < 0) return -1;
      }
    }
    if (info->members[i].type == T_AUNION || info->members[i].type == T_UNION) {
      if (info->members[i].type == T_AUNION) {
        index = info->members[i].type_info.anon_info.idx;
      } else {
        index = info->members[i].type_info.subunion_info.idx;
      }
      memset(&dummy, 0, sizeof(dummy));
      if (i > 0 && info->members[i-1].type == T_ENUM) {
        if ((ret = find_union(jps, &info->members[i-1])) < 0) return -1;
        *((int32_t *)(actual + info->members[i-1].off)) = ret;
        union_idx = enum_to_index(info->members[i-1].type_info.substruct_info.idx, ret);
        if (union_idx >= meta_info[index].member_count) return -1;
        if (union_idx >= 0) {
          dummy.member_count = 1;
          dummy.members[0] = meta_info[index].members[union_idx];
          jps_prev(jps);
          strcpy(dummy.members[0].name, jps_curr_start(jps));
          jps_next(jps);
          if (info->members[i].type == T_AUNION) {
            res = handle_members(actual, &dummy, jps);
            if (ret < 0) return -1;
          } else {
            res = handle_members(&actual[info->members[i].off], &dummy, jps);
            if (ret < 0) return -1;
          }
        }
      }
    }
  }
  return res;
}

int info_unpack_json(char *json, void *info, int idx) {
  int res;
  kr_jps *jps;
  if (json == NULL || info == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  //if (meta_info[idx].sz != sz) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  res = 0;
  kr_jps_alloca(jps);
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  if (jps_curr_type(jps) != JS_OBJECT) return -1;
  if (jps_next(jps) < 0) return 0; /* empty json, :/ */
  res = handle_members(info, &meta_info[idx], jps);
  if (res < 0) printf("JSON Unpack: Invalid JSON!!\n");
  jps_rewind(jps);
  return res;
}
