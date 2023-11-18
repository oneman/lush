#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <krad/gen/metainfo.h>

typedef enum {
  H_INIT = 1,
  H_VALID,
  H_RANDOM
} helper_type;

static void lowercase(char *str1, char *str2) {
  uint32_t len;
  uint32_t i;
  len = strlen(str1);
  for(i = 0; i < len; i++) {
    str2[i] = tolower(str1[i]);
  }
  str2[len] = '\0';
  return;
}

static int enum_prefix_strip(const struct_info *enu, uint idx,
 char *buf, size_t sz) {
  int i;
  int k;
  size_t us;
  size_t prefix_len;
  prefix_len = 0;
  if (!enu || !buf || !sz) return -1;
  if (idx >= enu->member_count) return -2;
  for (i = 0; i < enu->member_count; i++) {
    if (enu->member_count == 1) break;
    if (i == 0) {
      prefix_len = strlen(enu->members[i].name);
      continue;
    }
    for (k = us = 0; k < prefix_len; k++) {
      if (enu->members[i].name[k] != enu->members[i - 1].name[k]) break;
      if (enu->members[i].name[k] == '_') us = k;
    }
    if (k < prefix_len) prefix_len = us;
  }
  if (sz <= (strlen(enu->members[idx].name + prefix_len))) return -3;
  buf[sz - 1] = '\0';
  if (enu->members[idx].name[prefix_len] == '_') prefix_len++;
  lowercase((char *)enu->members[idx].name + prefix_len, buf);
  return 0;
}

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

static int handle_primitive(void *info, const member_info *memb, int type) {
  int i;
  member_type_info cmp;
  double scale;
  char *actual;
  int arr_len;
  memset(&cmp,0,sizeof(member_type_info));
  if (!memcmp(&memb->type_info, &cmp, sizeof(member_type_info))) return 0;
  arr_len = 1;
  if (memb->arr) arr_len = memb->arr;
  actual = (char *)info;
  switch (memb->type) {
    int len;
    double scale2;
    case T_CHAR: {
      if (memb->arr) {
        if (type == H_VALID) {
          if (memb->type_info.char_info.notnull) {
            if (actual[memb->off] == '\0') return -1;
          }
        } else if (type == H_RANDOM) {
          scale = (double)(122-97) / RAND_MAX;
          scale2 = (double)abs(0 - (arr_len - 1)) / RAND_MAX;
          len = 0 + floor(rand() * scale2);
          for (i = 0; i < len; i++) {
            actual[memb->off + i] = 97 + floor(rand() * scale);
          }
          actual[memb->off + len] = '\0';
        }
      }
      break;
    }
    case T_INT8: {
      int8_t *val;
      int8_t init;
      int8_t min;
      int8_t max;
      init = memb->type_info.int8_info.init;
      min = memb->type_info.int8_info.min;
      max = memb->type_info.int8_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((int8_t *)(actual + memb->off + (i * sizeof(int8_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_INT16: {
      int16_t *val;
      int16_t init;
      int16_t min;
      int16_t max;
      init = memb->type_info.int16_info.init;
      min = memb->type_info.int16_info.min;
      max = memb->type_info.int16_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((int16_t *)(actual + memb->off + (i * sizeof(int16_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_INT32: {
      int32_t *val;
      int32_t init;
      int32_t min;
      int32_t max;
      init = memb->type_info.int32_info.init;
      min = memb->type_info.int32_info.min;
      max = memb->type_info.int32_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((int32_t *)(actual + memb->off + (i * sizeof(int32_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_INT64: {
      int64_t *val;
      int64_t init;
      int64_t min;
      int64_t max;
      init = memb->type_info.int64_info.init;
      min = memb->type_info.int64_info.min;
      max = memb->type_info.int64_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((int64_t *)(actual + memb->off + (i * sizeof(int64_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)labs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_UINT8: {
      uint8_t *val;
      uint8_t init;
      uint8_t min;
      uint8_t max;
      init = memb->type_info.uint8_info.init;
      min = memb->type_info.uint8_info.min;
      max = memb->type_info.uint8_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_UINT16: {
      uint16_t *val;
      uint16_t init;
      uint16_t min;
      uint16_t max;
      init = memb->type_info.uint16_info.init;
      min = memb->type_info.uint16_info.min;
      max = memb->type_info.uint16_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((uint16_t *)(actual + memb->off + (i * sizeof(uint16_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_UINT32: {
      uint32_t *val;
      uint32_t init;
      uint32_t min;
      uint32_t max;
      init = memb->type_info.uint32_info.init;
      min = memb->type_info.uint32_info.min;
      max = memb->type_info.uint32_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((uint32_t *)(actual + memb->off + (i * sizeof(uint32_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)abs((int32_t)(min - max)) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_UINT64: {
      uint64_t *val;
      uint64_t init;
      uint64_t min;
      uint64_t max;
      init = memb->type_info.uint64_info.init;
      min = memb->type_info.uint64_info.min;
      max = memb->type_info.uint64_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((uint64_t *)(actual + memb->off + (i * sizeof(uint64_t))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)labs((int64_t)(min - max)) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    case T_FLOAT: {
      float *val;
      float init;
      float min;
      float max;
      init = memb->type_info.float_info.init;
      min = memb->type_info.float_info.min;
      max = memb->type_info.float_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((float *)(actual + memb->off + (i * sizeof(float))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)fabs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
          *val = roundf(*val * 100) / 100;
        }
      }
      break;
    }
    case T_DOUBLE: {
      double *val;
      double init;
      double min;
      double max;
      init = memb->type_info.double_info.init;
      min = memb->type_info.double_info.min;
      max = memb->type_info.double_info.max;
      for (i = 0; i < arr_len; i++) {
        val = ((double *)(actual + memb->off + (i * sizeof(double))));
        if (type == H_INIT) {
          *val = init;
        } else if (type == H_VALID) {
          if ( (*val < min) || (*val > max) ) return -1;
        } else {
          scale = (double)fabs(min - max) / RAND_MAX;
          *val = min + floor(rand() * scale);
        }
      }
      break;
    }
    default: break;
  }
  return 0;
}

static int handle_enum(const member_info *memb,
 char *actual, int type, int index) {
  int res;
  int *eval;
  int val;
  double scale;
  int min;
  int max;
  char out[256];
  if (memb->type_info.subenum_info.enum_init[0]) {
    res = str_to_enum(index,
     (char *)memb->type_info.subenum_info.enum_init);
    if ( (res >= 0) && (type == H_INIT) ) {
      eval = (int32_t *)(actual + memb->off);
      *eval = res;
    }
  }
  if (type == H_RANDOM) {
    min = 0;
    max = meta_info[index].member_count;
    eval = (int32_t *)(actual + memb->off);
    scale = (double)abs(min - max) / RAND_MAX;
    val = min + floor(rand() * scale);
    enum_prefix_strip(&meta_info[index], val, out, sizeof(out));
    res = str_to_enum(index, out);
    if (res >= 0) *eval = res;
  }
  return 0;
}

static int handle_members(void *act, const struct_info *info, int type) {
  int i;
  int j;
  int res;
  int index;
  int union_idx;
  char *actual;
  struct_info dummy;
  actual = (char *)act;
  res = 0;
  for (i = 0; i < info->member_count; i++) {
    if (is_member_primitive(&info->members[i])) {
      res = handle_primitive(act, &info->members[i], type);
      if (res < 0) return -1;
    }
    if (info->members[i].type == T_STRUCT) {
      if (info->members[i].arr) {
        for (j = 0; j < info->members[i].arr; j++) {
          index = info->members[i].type_info.substruct_info.idx;
          res = handle_members(&actual[info->members[i].off + (meta_info[index].sz * j)], &meta_info[index], type);
          if (res < 0) return -1;
        }
      } else {
        index = info->members[i].type_info.substruct_info.idx;
        res = handle_members(&actual[info->members[i].off], &meta_info[index], type);
        if (res < 0) return -1;
      }
    }
    if (info->members[i].type == T_ENUM) {
      index = info->members[i].type_info.subenum_info.idx;
      res = handle_enum(&info->members[i], actual, type, index);
      if (res < 0) return -1;
    }
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      if (meta_info[index].member_count > 0) {
        res = handle_members(actual, &meta_info[index], type);
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
        union_idx = enum_to_index(info->members[i-1].type_info.substruct_info.idx, (int)actual[info->members[i-1].off]);
        if (union_idx >= meta_info[index].member_count) return -1;
        if (union_idx >= 0) {
          dummy.member_count = 1;
          dummy.members[0] = meta_info[index].members[union_idx];
          if (info->members[i].type == T_AUNION) {
            res = handle_members(actual, &dummy, type);
            if (res < 0) return -1;
          } else {
            res = handle_members(&actual[info->members[i].off], &dummy, type);
            if (res < 0) return -1;
          }
        }
      }
    }
  }
  return res;
}

int info_helpers(void *info, int idx, int type) {
  int res;
  if (info == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  //if (meta_info[idx].sz != sz) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  if (type < H_INIT || type > H_RANDOM) return -1;
  if (type != H_VALID) memset(info, 0, meta_info[idx].sz);
  res = handle_members(info, &meta_info[idx], type);
  return res;
}
