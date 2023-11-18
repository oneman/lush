#include <stdlib.h>
#include <math.h>
#include <krad/gen/metainfo.h>
#include <krad/app/debug.h>

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

static int patch_path_internal(kr_patch *patch, kr_path *path, const struct_info *info) {
  int res;
  int i;
  int index;
  int union_idx;
  char *string;
  int arr_val;
  char *ptr;
  struct_info dummy;
  int len;
  len = kr_path_cur_name(path, &string);
  if (len < 0) return -1;
  res = -1;
  index = -1;
  for (i = 0; i < info->member_count; i++) {
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      res = patch_path_internal(patch, path, &meta_info[index]);
      if (res < 0) return -2;
      if (res == 1) continue;
      if (kr_path_steps_ahead(path) != 0) return -3;
      return res;
    }
    if (info->members[i].type == T_AUNION || info->members[i].type == T_UNION) {
      if (i == 0 || info->members[i-1].type != T_ENUM) return -9;
      if (info->members[i].type == T_AUNION) {
        index = info->members[i].type_info.anon_info.idx;
      } else {
        index = info->members[i].type_info.subunion_info.idx;
      }
      res = str_to_enum(info->members[i-1].type_info.subenum_info.idx, string);
      if (res < 0) continue;
      union_idx = enum_to_index(info->members[i-1].type_info.subenum_info.idx, res);
      if (union_idx >= meta_info[index].member_count || union_idx < 0) return -11;
      memset(&dummy, 0, sizeof(dummy));
      dummy.member_count = 1;
      dummy.members[0] = meta_info[index].members[union_idx];
      memcpy(dummy.members[0].name, string, len);
      dummy.members[0].name[len] = '\0';
      res = patch_path_internal(patch, path, &dummy);
      if (res < 0) return -2;
      if (res == 1) continue;
      if (kr_path_steps_ahead(path) != 0) return -3;
      return res;
    }
    if (!strcmp(string, info->members[i].name)) {
      patch->offset += info->members[i].off;
      if (info->members[i].arr) {
       if (kr_path_step(path) != 0) return -2;
        len = kr_path_cur_name(path, &string);
        if (len < 0) return -3;
        arr_val = strtod(string, &ptr);
        if (arr_val == 0 && string == ptr) return -4;
        patch->offset += (info->members[i].sz / info->members[i].arr) * arr_val;
      }
      if (is_member_primitive(&info->members[i])) {
        if (kr_path_steps_ahead(path) != 0) return -13;
        patch->sz = info->members[i].sz;
        return 0;
      }
      if (info->members[i].type == T_STRUCT) {
        index = info->members[i].type_info.substruct_info.idx;
        if (kr_path_step(path) != 0) return -6;
        res = patch_path_internal(patch, path, &meta_info[index]);
        if (res < 0) return -2;
        if (res == 1) continue;
        if (kr_path_steps_ahead(path) != 0) return -3;
        return res;
      }
      if (info->members[i].type == T_ENUM) {
        if (kr_path_steps_ahead(path) == 0) {
          patch->sz = sizeof(int32_t);
          return 0;
        }
        continue;
      }
    }
  }
  return 1;
}

static int path_get_type_internal(kr_path *path, const struct_info *info) {
  int res;
  int i;
  int index;
  char *string;
  int arr_val;
  char *ptr;
  int union_idx;
  struct_info dummy;
  int len;
  len = kr_path_cur_name(path, &string);
  if (len < 0) return -1;
  res = -1;
  index = -1;
  for (i = 0; i < info->member_count; i++) {
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      res = path_get_type_internal(path, &meta_info[index]);
      if (res < 0) return -2;
      if (res == 0) continue;
      if (kr_path_steps_ahead(path) != 0) return -3;
      return res;
    }
    if (info->members[i].type == T_AUNION || info->members[i].type == T_UNION) {
      if (i == 0 || info->members[i-1].type != T_ENUM) return -7;
      if (info->members[i].type == T_AUNION) {
        index = info->members[i].type_info.anon_info.idx;
      } else {
        index = info->members[i].type_info.subunion_info.idx;
      }
      res = str_to_enum(info->members[i-1].type_info.subenum_info.idx, string);
      if (res < 0) continue;
      union_idx = enum_to_index(info->members[i-1].type_info.subenum_info.idx, res);
      if (union_idx >= meta_info[index].member_count || union_idx < 0) return -9;
      memset(&dummy, 0, sizeof(dummy));
      dummy.member_count = 1;
      dummy.members[0] = meta_info[index].members[union_idx];
      memcpy(dummy.members[0].name, string, len);
      dummy.members[0].name[len] = '\0';
      res = path_get_type_internal(path, &dummy);
      if (res < 0) return -2;
      if (res == 0) continue;
      if (kr_path_steps_ahead(path) != 0) return -3;
      return res;
    }
    if (!strcmp(string, info->members[i].name)) {
      if (info->members[i].arr) {
       if (kr_path_step(path) != 0) return -2;
        len = kr_path_cur_name(path, &string);
        if (len < 0) return -3;
        arr_val = strtod(string, &ptr);
        if (arr_val == 0 && string == ptr) return -4;
      }
      if (is_member_primitive(&info->members[i])) {
        if (kr_path_steps_ahead(path) != 0) return -3;
        return info->members[i].type;
      }
      if (info->members[i].type == T_STRUCT) {
        index = info->members[i].type_info.substruct_info.idx;
        if (kr_path_step(path) != 0) return -6;
        res = path_get_type_internal(path, &meta_info[index]);
        if (res < 0) return -2;
        if (res == 0) continue;
        if (kr_path_steps_ahead(path) != 0) return -3;
        return res;
      }
      if (info->members[i].type == T_ENUM) {
        if (kr_path_steps_ahead(path) == 0) return info->members[i].type;
        continue;
      }
    }
  }
  return 0;
}

static size_t find_max(const struct_info *info) {
  int i;
  size_t max;
  max = 0;
  for (i = 0; i < info->member_count; i++) {
    if ( (info->members[i].sz + info->members[i].off ) > max) {
      max = info->members[i].sz + info->members[i].off;
    }
  }
  return max;
}

static int union_check(const struct_info *meta, int idx) {
  if (idx == 0) return 0;
  if (meta->members[idx - 1].type != T_ENUM) return 0;
  if (meta->members[idx].type == T_UNION) return 1;
  if (meta->members[idx].type == T_AUNION) return 1;
  return 0;
}

static int offset_to_path(char *path, size_t sz,
 const struct_info *info, size_t off, void *act) {
  const member_info *memb;
  int i;
  int ret;
  int idx;
  int arr_idx;
  int union_idx;
  char *string;
  struct_info dummy;
  ret = 0;
  idx = 0;
  memb = NULL;
  if (info->member_count == 0) return 0;
  for (i = 0; i < info->member_count; i++) {
    memb = &info->members[i];
    if (memb->type == T_ASTRUCT) {
      idx = memb->type_info.anon_info.idx;
      if (meta_info[idx].member_count == 0) return -1;
      if (off > (meta_info[idx].members[meta_info[idx].member_count - 1].off
       + meta_info[idx].members[meta_info[idx].member_count - 1].sz)) {
        continue;
      } else break;
    } else if (memb->type == T_AUNION) {
      idx = memb->type_info.anon_info.idx;
      if (meta_info[idx].member_count == 0) return -1;
      if (off > find_max(&meta_info[idx])) {
        continue;
      } else break;
    } else {
      if (off >= (memb->off + memb->sz)) continue;
      else break;
    }
  }
  if (i == info->member_count) return -1;
  if (union_check(info, i)) {
    idx = info->members[i-1].type_info.subenum_info.idx;
    union_idx = enum_to_index(idx, *(int32_t *)(act + info->members[i-1].off));
    if (union_idx < 0) return -1;
    string = enum_to_str(idx, *(int32_t *)(act + info->members[i-1].off));
    if (memb->type == T_UNION) {
      idx = memb->type_info.subunion_info.idx;
    }
    if (memb->type == T_AUNION) {
      idx = memb->type_info.anon_info.idx;
    }
    dummy.member_count = 1;
    dummy.members[0] = meta_info[idx].members[union_idx];
    dummy.members[0].off = 0;
    strcpy(dummy.members[0].name, string);
    ret = offset_to_path(path, sz, &dummy,
     off - meta_info[idx].members[union_idx].off,
      act + meta_info[idx].members[union_idx].off);
    if (ret < 0) return -1;
    return ret;
  }
  if (memb->type == T_ENUM) {
    ret = snprintf(path, sz, "/%s", memb->name);
    if (ret == sz) return -1;
    path += ret;
    sz -= ret;
    return T_ENUM;
  }
  if (is_member_primitive(memb)) {
    if (memb->arr && memb->type == T_CHAR) {
      if ((off - memb->off) != 0) return -1;
    } else if (memb->arr && memb->type != T_CHAR) {
      off -= memb->off;
      if ((off - memb->off) != 0) return -1;
      if (off % (memb->sz / memb->arr)) return -1;
      arr_idx = off / (memb->sz / memb->arr);
      ret = snprintf(path, sz, "/%s/%d", memb->name, arr_idx);
      if (ret == sz) return -1;
      path += ret;
      sz -= ret;
    } else {
      if ((off - memb->off) != 0) return -1;
      ret = snprintf(path, sz, "/%s", memb->name);
      if (ret == sz) return -1;
      path += ret;
      sz -= ret;
    }
    return memb->type;
  }
  if (memb->type == T_STRUCT) {
    idx = memb->type_info.substruct_info.idx;
    ret = snprintf(path, sz, "/%s", memb->name);
    if (ret == sz) return -1;
    path += ret;
    sz -= ret;
    if (memb->arr) {
      off -= memb->off;
      arr_idx = floor(off / (memb->sz / memb->arr));
      ret = snprintf(path, sz, "/%d", arr_idx);
      if (ret == sz) return -1;
      path += ret;
      sz -= ret;
      ret = offset_to_path(path, sz, &meta_info[idx],
       off - (arr_idx * (memb->sz / memb->arr)),
        act + memb->off + (arr_idx * (memb->sz / memb->arr)));
      if (ret < 0) return -1;
    } else {
      ret = offset_to_path(path, sz, &meta_info[idx], off - memb->off,
        act + memb->off);
      if (ret < 0) return -1;
    }
    return ret;
  }
  if (memb->type == T_ASTRUCT) {
    idx = memb->type_info.anon_info.idx;
    ret = snprintf(path, sz, "/%s", memb->name);
    if (ret == sz) return -1;
    path += ret;
    sz -= ret;
    ret = offset_to_path(path, sz, &meta_info[idx], off - memb->off,
     act + memb->off);
    if (ret < 0) return -1;
    return ret;
  }
  return -1;
}

int patch_get_path(char *path, size_t sz, kr_patch *patch,
 int idx, void *info) {
  int ret;
  if (!patch || !path || !sz || !info) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -2;
  if (meta_info[idx].type != ST_STRUCT) return -3;
  ret = offset_to_path(path, sz, &meta_info[idx],
   patch->offset, info);
  return ret;
}

int path_get_type(kr_path *path, int idx) {
  int ret;
  if (!path) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -2;
  if (meta_info[idx].type != ST_STRUCT) return -3;
  ret = path_get_type_internal(path, &meta_info[idx]);
  kr_path_rewind(path);
  if (ret == 0) return -45;
  return ret;
}

int patch_path(kr_patch *patch, kr_path *path, int idx) {
  int res;
  if (patch == NULL || path == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -2;
  if (meta_info[idx].type != ST_STRUCT) return -3;
  res = patch_path_internal(patch, path, &meta_info[idx]);
  kr_path_rewind(path);
  if (res == 1) return -45;
  return res;
}

static int path_enum_idx(kr_path *path, const struct_info *info) {
  int res;
  int i;
  int index;
  char *string;
  int arr_val;
  char *ptr;
  int union_idx;
  struct_info dummy;
  int len;
  len = kr_path_cur_name(path, &string);
  if (len < 0) return -1;
  res = -1;
  index = -1;
  for (i = 0; i < info->member_count; i++) {
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      res = path_enum_idx(path, &meta_info[index]);
      if (res < 0) return -11;
      if (res == 0) continue;
      if (kr_path_steps_ahead(path) != 0) return -1;
      return res;
    }
    if (info->members[i].type == T_AUNION || info->members[i].type == T_UNION) {
      if (i == 0 || info->members[i-1].type != T_ENUM) return -8;
      if (info->members[i].type == T_AUNION) {
        index = info->members[i].type_info.anon_info.idx;
      } else {
        index = info->members[i].type_info.subunion_info.idx;
      }
      res = str_to_enum(info->members[i-1].type_info.subenum_info.idx, string);
      if (res < 0) continue;
      union_idx = enum_to_index(info->members[i-1].type_info.subenum_info.idx, res);
      if (union_idx >= meta_info[index].member_count || union_idx < 0) return -10;
      memset(&dummy, 0, sizeof(dummy));
      dummy.member_count = 1;
      dummy.members[0] = meta_info[index].members[union_idx];
      memcpy(dummy.members[0].name, string, len);
      dummy.members[0].name[len] = '\0';
      res = path_enum_idx(path, &dummy);
      if (res < 0) return -11;
      if (res == 0) continue;
      if (kr_path_steps_ahead(path) != 0) return -1;
      return res;
    }
    if (!strcmp(string, info->members[i].name)) {
      if (is_member_primitive(&info->members[i])) {
        if (kr_path_steps_ahead(path) != 0) continue;
        return -5;
      }
      if (info->members[i].arr) {
       if (kr_path_step(path) != 0) return -2;
        len = kr_path_cur_name(path, &string);
        if (len < 0) return -3;
        arr_val = strtod(string, &ptr);
        if (arr_val == 0 && string == ptr) return -4;
      }
      if (info->members[i].type == T_STRUCT) {
        index = info->members[i].type_info.substruct_info.idx;
        if (kr_path_step(path) != 0) return -7;
        res = path_enum_idx(path, &meta_info[index]);
        if (res < 0) return -11;
        if (res == 0) continue;
        if (kr_path_steps_ahead(path) != 0) return -1;
        return res;
      }
      if (info->members[i].type == T_ENUM) {
        index = info->members[i].type_info.subenum_info.idx;
        if (kr_path_steps_ahead(path) == 0) return index;
        continue;
      }
    }
  }
  return 0;
}

char *path_enum_str(kr_path *path, int val, int idx) {
  int ret;
  if (!path || val < 0) return NULL;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return NULL;
  if (meta_info[idx].type != ST_STRUCT) return NULL;
  ret = path_enum_idx(path, &meta_info[idx]);
  if (ret <= 0) return NULL;
  kr_path_rewind(path);
  return enum_to_str(ret, val);
}

int path_enum_val(kr_path *path, char *str, size_t len, int idx) {
  int ret;
  if (!path || !str || len <= 0) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -2;
  if (meta_info[idx].type != ST_STRUCT) return -3;
  ret = path_enum_idx(path, &meta_info[idx]);
  if (ret < 0) return -4;
  kr_path_rewind(path);
  return str_to_enum(ret, str);
}
