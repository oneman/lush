#include <krad/gen/metainfo.h>
#include <ctype.h>

int metainfo_to_jschema(char *json, int max, int idx);
static int handle_aunion(char *json, int max , int aidx, int eidx);
static int properties_gen(char *json, int max, const struct_info *info);
static int required_gen(char *json, int max, const struct_info *info);

static char *memb_type_to_json_type(const member_info *memb) {
  switch (memb->type) {
    case T_CHAR:
    return "string";
    case T_INT8:
    case T_INT16:
    case T_INT32:
    case T_INT64:
    case T_UINT8:
    case T_UINT16:
    case T_UINT32:
    case T_UINT64:
    return "integer";
    case T_FLOAT:
    case T_DOUBLE:
    return "number";
    case T_STRUCT:
    return "object";
    case T_UNION:
    return "object";
    case T_ENUM:
    return "string";
    case T_ASTRUCT:
    return "object";
    case T_AUNION:
    return "object";
    case T_ANON:
    return "object";
    default: return "";
  }
}

static int gen_limits(char *json, int max, const member_info *memb) {
  int res;
  const member_type_info *info;
  member_type_info cmp;
  info = &memb->type_info;
  res = 0;
  memset(&cmp, 0, sizeof(member_type_info));
  if (!memcmp(info, &cmp, sizeof(member_type_info))) return res;
  res += snprintf(&json[res], max - res, ",");
  if (res >= max) return max;
  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        res += snprintf(&json[res], max - res,"\"maxLength\" : %d ",memb->arr);
        if (res >= max) return max;
        if (info->char_info.notnull) {
          res += snprintf(&json[res], max - res,",\"minLength\" : 1 ");
          if (res >= max) return max;
        }
      }
      return res;
    }
    case T_INT8: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %d, ",info->int8_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %d ",info->int8_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_INT16: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %d, ",info->int16_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %d ",info->int16_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_INT32: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %d, ",info->int32_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %d ",info->int32_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_INT64: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %"PRId64", ",info->int64_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %"PRId64,info->int64_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_UINT8: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %u, ",info->uint8_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %u ",info->uint8_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_UINT16: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %u, ",info->uint16_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %u ",info->uint16_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_UINT32: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %u, ",info->int32_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %u ",info->int32_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_UINT64: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %"PRIu64", ",info->uint64_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %"PRIu64,info->uint64_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_FLOAT: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %0.2f, ",info->float_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %0.2f ",info->float_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_DOUBLE: {
      res += snprintf(&json[res], max - res,"\"minimum\" : %0.2f, ",info->double_info.min);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,"\"maximum\" : %0.2f ",info->double_info.max);
      if (res >= max) return max;
      return res;
    }
    case T_STRUCT:
    case T_UNION:
    case T_ENUM:
    case T_ASTRUCT:
    case T_AUNION:
    case T_ANON:
    default: return 0;
  }
}

static int handle_enum(char *json, int max, int idx) {
  const struct_info *info;
  char *name;
  int res;
  int i;
  res = 0;
  name = NULL;
  info = &meta_info[idx];
  res += snprintf(&json[res], max - res, ",\"enum\":[");
  if (res >= max) return max;
  for (i = 0; i < info->member_count; i++) {
    name = enum_to_str(idx, i+1);
    res += snprintf(&json[res], max - res, "\"%s\"", name);
    if (res >= max) return max;
    if (i != (info->member_count - 1)) {
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
    }
  }
  res += snprintf(&json[res], max - res, "]");
  if (res >= max) return max;
  return res;
}

static int handle_aunion(char *json, int max , int aidx, int eidx) {
  int res;
  int i;
  char *name;
  const struct_info *ainfo;
  struct_info dummy;
  ainfo = &meta_info[aidx];
  res = 0;
  for (i = 0; i < ainfo->member_count; i++) {
    dummy.member_count = 1;
    dummy.members[0] = ainfo->members[i];
    name = enum_to_str(eidx, i + 1);
    strcpy(dummy.members[0].name, name);
    res += properties_gen(&json[res], max - res, &dummy);
    if (res >= max) return max;
    if (i != (ainfo->member_count - 1)) {
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
    }
  }
  return res;
}

static int properties_gen(char *json, int max, const struct_info *info) {
  int i;
  int idx;
  int res;
  res = 0;
  for (i = 0; i < info->member_count; i++) {
    if (info->members[i].arr && info->members[i].type != T_CHAR) {
      res += snprintf(&json[res], max - res, "\"%s\":{\"type\":\"array\",\"items\":",
       info->members[i].name);
      if (res >= max) return max;

      if (info->members[i].type == T_STRUCT) {
        res += metainfo_to_jschema(&json[res], max - res,
         info->members[i].type_info.substruct_info.idx);
        if (res >= max) return max;
      } else {
        res += snprintf(&json[res], max - res, "{\"type\":\"%s\"",
         memb_type_to_json_type(&info->members[i]));
        if (res >= max) return max;
        res += gen_limits(&json[res], max - res, &info->members[i]);
        if (res >= max) return max;
        res += snprintf(&json[res], max - res, "}");
        if (res >= max) return max;
      }
      res += snprintf(&json[res], max - res,",\"maxItems\" : %d ", info->members[i].arr);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res,",\"minItems\" : 0 ");
      if (res >= max) return max;
      res += snprintf(&json[res], max - res, "}");
      if (res >= max) return max;

      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;

    } else {
      if (info->members[i].type == T_STRUCT) {
        res += snprintf(&json[res], max - res, "\"%s\":",
          info->members[i].name);
        if (res >= max) return max;
        idx = info->members[i].type_info.substruct_info.idx;
        res += metainfo_to_jschema(&json[res], max - res, idx);
        if (res >= max) return max;

        res += snprintf(&json[res], max - res, ",");
        if (res >= max) return max;

        continue;
      }
      if (info->members[i].type ==T_AUNION) {
        continue;
      }
      res += snprintf(&json[res], max - res, "\"%s\":", info->members[i].name);
      res += snprintf(&json[res], max - res, "{\"type\":\"%s\"",
       memb_type_to_json_type(&info->members[i]));
      if (res >= max) return max;
      if (info->members[i].type == T_ENUM) {
        res += handle_enum(&json[res], max - res,
         info->members[i].type_info.subenum_info.idx);
        if (res >= max) return max;
      if (res >= max) return max;
      res += snprintf(&json[res], max - res, "}");
      if (res >= max) return max;
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
        if (i != (info->member_count - 1) &&
           (info->members[i+1].type == T_AUNION)) {
            idx = info->members[i+1].type_info.anon_info.idx;
            res += handle_aunion(&json[res], max - res,
             idx, info->members[i].type_info.subenum_info.idx);
            if (res >= max) return max;
            res += snprintf(&json[res], max - res, ",");
            if (res >= max) return max;
          }
      continue;
      }
      res += gen_limits(&json[res], max - res, &info->members[i]);
      if (res >= max) return max;
      res += snprintf(&json[res], max - res, "}");
      if (res >= max) return max;
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
    }
  }
  if (json[res - 1] == ',') json[--res] = '\0';
  return res;
}

static int required_gen(char *json, int max, const struct_info *info) {
  int i;
  int j;
  int res;
  int required;
  res = 0;
  for (i = required = 0; i < info->member_count; i++) {
    if (info->members[i].type == T_AUNION) continue;
    if (!info->members[i].opt) required++;
  }
  for (i = j = 0; i < info->member_count; i++) {
    if (info->members[i].type == T_AUNION) continue;
    if (info->members[i].opt) continue;
    res += snprintf(&json[res], max - res, "\"%s\"",
     info->members[i].name);
    if (res >= max) return max;
    if (j != (required - 1)) {
      res += snprintf(&json[res], max - res, ",");
      if (res >= max) return max;
    }
    j++;
  }
  return res;
}

int metainfo_to_jschema(char *json, int max, int idx) {
  int res;
  char name[64];
  char *prefix;
  char *suffix;
  memset(name, 0, sizeof(name));
  prefix = NULL;
  suffix = NULL;
  if (json == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  res = 0;
//  strncpy(name, meta_info[idx].name, sizeof(name));
  suffix = strstr(name, "_info");
  prefix = strstr(name, "kr_");
  if (suffix) suffix[0] = '\0';
  if (prefix) {
//    strncpy(name, name + 3, sizeof(name));
  } else {
//    strncpy(name, name, sizeof(name));
  }
  res += snprintf(&json[res], max - res, "{\"title\":\"%s\",\"type\":\"object\","
    , name);
  if (res >= max) return max;
  res += snprintf(&json[res], max - res, "\"properties\":{");
  if (res >= max) return max;
  res += properties_gen(&json[res], max - res, &meta_info[idx]);
  if (res >= max) return max;
  res += snprintf(&json[res], max - res, "},\"required\":[");
  if (res >= max) return max;
  res += required_gen(&json[res], max - res, &meta_info[idx]);
  if (res >= max) return max;
  res += snprintf(&json[res], max - res, "]");
  if (res >= max) return max;
  res += snprintf(&json[res], max - res, "}");
  if (res >= max) return max;
  return res;
}
