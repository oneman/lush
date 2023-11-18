#include "metainfo.h"

static int depth = 1;

static int check_zero(void *s1, int idx) {
  int i;
  unsigned char *p1;
  p1 = s1;
  for (i = 0; i < meta_info[idx].sz; i++) {
    if (*p1 != 0) return 1;
    p1++;
  }
  return 0;
}

static int handle_primitive(char *text, void *info, const member_info *memb, int max) {
  int i;
  int res;
  res = 0;
  char *actual;
  char indent[(depth*2)+1];
  actual = (char *)info;
  memset(indent,' ',depth*2);
  indent[depth*2] = '\0';
  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        res += snprintf(text, max, "%s%s: %s\n", indent, memb->name, actual + memb->off);
      } else  if (memb->ptr) {
        if (memb->ptr == 1) {
          res += snprintf(text, max, "%s%s: %s\n", indent, memb->name, actual + memb->off);
        } else {
          break;
        }
      } else {
        res += snprintf(text, max, "%s%s: %c\n", indent, memb->name, *(actual + memb->off));
      }
      break;
    }
    case T_INT8: {
      int8_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((int8_t *)(actual + memb->off + (i * sizeof(int8_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRId8"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((int8_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRId8"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_INT16: {
      int16_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((int16_t *)(actual + memb->off + (i * sizeof(int16_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRId16"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((int16_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRId16"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_INT32: {
      int32_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((int32_t *)(actual + memb->off + (i * sizeof(int32_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRId32"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((int32_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRId32"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_INT64: {
      int64_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((int64_t *)(actual + memb->off + (i * sizeof(int64_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRId64"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((int64_t *)(actual + memb->off));
        res += snprintf(text, max, "%s%s: %"PRId64"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_UINT8: {
      uint8_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRIu8"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((uint8_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRIu8"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_UINT16: {
      uint16_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((uint16_t *)(actual + memb->off + (i * sizeof(uint16_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRIu16"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((uint16_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRIu16"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_UINT32: {
      uint32_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((uint32_t *)(actual + memb->off + (i * sizeof(uint32_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRIu32"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((uint32_t *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %"PRIu32"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_UINT64: {
      uint64_t *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((uint64_t *)(actual + memb->off + (i * sizeof(uint64_t))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %"PRIu64"\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((uint64_t *)(actual + memb->off));
        res += snprintf(text, max, "%s%s: %"PRIu64"\n", indent, memb->name, *val);
      }
      break;
    }
    case T_FLOAT: {
      float *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((float *)(actual + memb->off + (i * sizeof(float))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %0.2f\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((float *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %0.2f\n", indent, memb->name, *val);
      }
      break;
    }
    case T_DOUBLE: {
      double *val;
      if (memb->arr) {
        res += snprintf(&text[res], max - res, "%s%s: \n",  indent, memb->name);
        if (res >= max) return max;
        for (i = 0; i < memb->arr; i++) {
          val = ((double *)(actual + memb->off + (i * sizeof(double))));
          res += snprintf(&text[res], max - res, "  %s[%d]: %0.2f\n", indent, i, *val);
          if (res >= max) return max;
        }
      } else {
        val = ((double *)(actual + memb->off));
        res += snprintf(&text[res], max - res, "%s%s: %0.2f\n", indent, memb->name, *val);
      }
      break;
    }
    default: break;
  }
  return res;
}

static int handle_members(char *text, void *act, int max, const struct_info *info) {
  int i;
  int j;
  int res;
  int ret;
  int index;
  int union_idx;
  char *actual;
  struct_info dummy;
  char indent[(depth*2)+1];
  memset(indent,' ',depth*2);
  indent[depth*2] = '\0';
  actual = (char *)act;
  res = 0;
  for (i = 0; i < info->member_count; i++) {
    if ( (ret = handle_primitive(&text[res], act, &info->members[i], max - res)) ) {
      res += ret;
      if (res >= max) return max;
    }
    if (info->members[i].type == T_STRUCT) {
      if (info->members[i].arr) {
        for (j = 0; j < info->members[i].arr; j++) {
          index = info->members[i].type_info.substruct_info.idx;
          if (!check_zero(&actual[info->members[i].off + (meta_info[index].sz * j)], index)) {
            continue;
          }
          res += snprintf(&text[res], max - res, "%s%s[%d]:\n", indent, info->members[i].name, j);
          if (res >= max) return max;
          depth += 2;
          res += handle_members(&text[res],
            &actual[info->members[i].off + (meta_info[index].sz * j)],
             max - res, &meta_info[index]);
          depth -= 2;
          if (res >= max) return max;
        }
      } else {
        res += snprintf(&text[res], max - res, "%s%s:\n", indent, info->members[i].name);
        if (res >= max) return max;
        depth += 2;
        index = info->members[i].type_info.substruct_info.idx;
        res += handle_members(&text[res], &actual[info->members[i].off], max - res, &meta_info[index]);
        depth -= 2;
        if (res >= max) return max;
      }
    }
    if (info->members[i].type == T_ENUM) {
      index = info->members[i].type_info.subenum_info.idx;
      res += snprintf(&text[res], max - res, "%s%s: %s\n",
        indent, info->members[i].name, enum_to_str(index, (int)actual[info->members[i].off]));
      if (res >= max) return max;
    }
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      if (meta_info[index].member_count > 0) {
        res += handle_members(&text[res], actual, max - res, &meta_info[index]);
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
          if (info->members[i].type == T_AUNION) {
            res += handle_members(&text[res], actual, max - res, &dummy);
          } else {
            res += handle_members(&text[res], &actual[info->members[i].off], max - res, &dummy);
          }
          if (res >= max) return max;
        }
      }
    }
  }
  return res;
}

int info_to_text(char *text, void *info, int max, int idx) {
  int res;
  if (text == NULL || info == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  //if (meta_info[idx].sz != sz) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  res = handle_members(text, info, max, &meta_info[idx]);
  return res;
}
