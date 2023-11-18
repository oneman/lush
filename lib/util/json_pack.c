#include <krad/gen/metainfo.h>

static int check_zero(void *s, size_t sz) {
  uint8_t mem[sz];
  memset(mem, 0, sz);
  return memcmp(mem, s, sz);
}

static int handle_primitive(char *json, void *info, const member_info *memb, int max) {
  int i;
  int res;
  res = 0;
  char *actual;
  actual = (char *)info;
  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": \"%s\"",  memb->name, actual + memb->off);
      } else  if (memb->ptr) {
        if (memb->ptr == 1) {
          res += snprintf(&json[res], max - res, "\"%s\": \"%s\"",  memb->name, actual + memb->off);
        } else {
          return 0;
        }
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": \"%c\"",  memb->name, *(actual + memb->off));
      }
      return res;
    }
    case T_INT8: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%d", *((int8_t *)(actual + memb->off + (i * sizeof(int8_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %d",  memb->name, *((int8_t *)(actual + memb->off)));
      }
      break;
    }
    case T_INT16: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%d", *((int16_t *)(actual + memb->off + (i * sizeof(int16_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %d",  memb->name, *((int16_t *)(actual + memb->off)));
      }
      break;
    }
    case T_INT32: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%d", *((int32_t *)(actual + memb->off + (i * sizeof(int32_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %d",  memb->name, *((int32_t *)(actual + memb->off)));
      }
      break;
    }
    case T_INT64: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%"PRId64, *((int64_t *)(actual + memb->off + (i * sizeof(int64_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %"PRId64,  memb->name, *((int64_t *)(actual + memb->off)));
      }
      break;
    }
    case T_UINT8: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%u", *((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %u",  memb->name, *((uint8_t *)(actual + memb->off)));
      }
      break;
    }
    case T_UINT16: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%u", *((uint16_t *)(actual + memb->off + (i * sizeof(uint16_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %u",  memb->name, *((uint16_t *)(actual + memb->off)));
      }
      break;
    }
    case T_UINT32: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%u", *((uint32_t *)(actual + memb->off + (i * sizeof(uint32_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %u",  memb->name, *((uint32_t *)(actual + memb->off)));
      }
      break;
    }
    case T_UINT64: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%"PRIu64, *((uint64_t *)(actual + memb->off + (i * sizeof(uint64_t)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %"PRIu64,  memb->name, *((uint64_t *)(actual + memb->off)));
      }
      break;
    }
    case T_FLOAT: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%0.2f", *((float *)(actual + memb->off + (i * sizeof(float)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %0.2f",  memb->name, *((float *)(actual + memb->off)));
      }
      break;
    }
    case T_DOUBLE: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res, "\"%s\": ",  memb->name);
        res += snprintf(&json[res], max - res, "[");
        for (i = 0; i < memb->arr; i++) {
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "%0.2f", *((double *)(actual + memb->off + (i * sizeof(double)))));
          if (i != (memb->arr - 1)) {
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
        }
        res += snprintf(&json[res], max - res, "]");
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "\"%s\": %0.2f",  memb->name, *((double *)(actual + memb->off)));
      }
      break;
    }
    default: return 0;
  }
  return res;
}

static int handle_members(char *json, void *act, int max, const struct_info *info) {
  int i;
  int j;
  int res;
  int ret;
  int index;
  int union_idx;
  char *actual;
  struct_info dummy;
  actual = (char *)act;
  res = 0;
  for (i = 0; i < info->member_count; i++) {
    if ( (ret = handle_primitive(&json[res], act, &info->members[i], max - res)) ) {
      res += ret;
      if (res >= max) return max;
    }
    if (info->members[i].type == T_STRUCT) {
      if (info->members[i].arr) {
        res += snprintf(&json[res], max - res,
         "\"%s\": [",  info->members[i].name);
          for (j = 0; j < info->members[i].arr; j++) {
            index = info->members[i].type_info.substruct_info.idx;
            res += snprintf(&json[res], max - res, "{");
            if (res >= max) return max;
            res += handle_members(&json[res],
             &actual[info->members[i].off + (meta_info[index].sz * j)],
              max - res, &meta_info[index]);
            if (res >= max) return max;
            res += snprintf(&json[res], max - res, "}");
            if (res >= max) return max;
            if (j != (info->members[i].arr - 1)) {
              if (info->members[i].arr_notnull) {
                if (!check_zero(&actual[info->members[i].off + (meta_info[index].sz * (j + 1))], meta_info[index].sz)) break;
              }
              res += snprintf(&json[res], max - res, ",");
              if (res >= max) return max;
            }
          }
          res += snprintf(&json[res], max - res, "]");
          if (res >= max) return max;
        } else {
          res += snprintf(&json[res], max - res, "\"%s\": {",  info->members[i].name);
          if (res >= max) return max;
          index = info->members[i].type_info.substruct_info.idx;
          res += handle_members(&json[res],
           &actual[info->members[i].off], max - res, &meta_info[index]);
          if (res >= max) return max;
          res += snprintf(&json[res], max - res, "}");
          if (res >= max) return max;
        }
    }
    if (info->members[i].type == T_ENUM) {
      if (i != (info->member_count - 1) &&
       (info->members[i+1].type == T_AUNION ||
        info->members[i+1].type == T_UNION)) continue;
      index = info->members[i].type_info.subenum_info.idx;
      res += snprintf(&json[res], max - res, "\"%s\": \"%s\"",
        info->members[i].name, enum_to_str(index,
         (int)actual[info->members[i].off]));
      if (res >= max) return max;
    }
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      if (meta_info[index].member_count > 0) {
        res += handle_members(&json[res], actual, max - res, &meta_info[index]);
      }
      if (res >= max) return max;
    }
    if (info->members[i].type == T_AUNION || info->members[i].type == T_UNION) {
      if (info->members[i].type == T_AUNION) {
        index = info->members[i].type_info.anon_info.idx;
      } else {
        index = info->members[i].type_info.subunion_info.idx;
      }
      memset(&dummy, 0, sizeof(dummy));
      if (i > 0 && info->members[i-1].type == T_ENUM) {
        union_idx = enum_to_index(info->members[i-1].type_info.substruct_info.idx,
         (int)actual[info->members[i-1].off]);
        if (union_idx >= meta_info[index].member_count) return max;
        if (union_idx >= 0) {
          dummy.member_count = 1;
          dummy.members[0] = meta_info[index].members[union_idx];
          strcpy(dummy.members[0].name,
            enum_to_str(info->members[i-1].type_info.substruct_info.idx,
             (int)actual[info->members[i-1].off]));
          if (info->members[i].type == T_AUNION) {
            res += handle_members(&json[res], actual, max - res, &dummy);
          } else {
            res += handle_members(&json[res],
              &actual[info->members[i].off], max - res, &dummy);
          }
          if (res >= max) return max;
        }
      }
    }
    if (i != (info->member_count - 1)) {
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
    }
  }
  return res;
}

int info_pack_json(char *json, void *info, int max, int idx) {
  int res;
  if (json == NULL || info == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  //if (meta_info[idx].sz != sz) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  res = 0;
  res += snprintf(json, max, "{");
  if (res >= max) return -2;
  res += handle_members(&json[res], info, max - res, &meta_info[idx]);
  if (res >= max) return -3;
  res += snprintf(&json[res], max - res, "}");
  if (res >= max) return -2;
  return res;
}
