#include <alloca.h>
#include <krad/gen/metainfo.h>
#include <krad/app/path.h>
#include <krad/app/diff.h>

typedef struct {
  kr_diffset *diffset;
  void *info1;
  void *info2;
  size_t sz;
} diff_st;

static int is_primitive(const member_info *memb) {
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

static int union_check(const struct_info *meta, int idx) {
  if (idx == (meta->member_count - 1)) return 0;
  if (meta->members[idx].type != T_ENUM) return 0;
  if (meta->members[idx + 1].type == T_UNION) return 1;
  if (meta->members[idx + 1].type == T_AUNION) return 1;
  return 0;
}

static int handle_primitive(diff_st *diffst, kr_diff *diff, const member_info *memb,
 void *info1, void *info2) {
  kr_diffset *diffset;
  int res;
  int j;
  char index_str[16];
  diffset = diffst->diffset;
  if (memb->arr && memb->type != T_CHAR) {
    for (j = 0; j < memb->arr; j++) {
      diff->sz = memb->sz / memb->arr;
      res = snprintf(index_str, sizeof(index_str), "%d", j);
      if (res >= sizeof(index_str)) return -1;
      if (memcmp(info1 + memb->off + (diff->sz * j),
       info2 + memb->off + (diff->sz * j), diff->sz)) {
        if (memb->type == T_FLOAT || memb->type == T_DOUBLE) {
          diff->type = DIFF_REAL;
        } else {
          diff->type = DIFF_INTEGER;
        }
        diff->off = info1 - diffst->info1 + memb->off + (diff->sz * j);
        if (diffset->len == (KR_NDIFFS - 1)) return -1;
        diffset->len++;
      }
    }
  } else if (memb->arr && memb->type == T_CHAR) {
    return 0;
    /* diff->sz = memb->arr;
    if (memcmp(info1 + memb->off, info2 + memb->off, memb->arr)) {
      diff->type = DIFF_STRING;
      diff->off = info1 - diffst->info1 + memb->off;
      if (diffset->len == (KR_NDIFFS - 1)) return -1;
      diffset->len++;
    }*/
  } else {
    diff->sz = memb->sz;
    if (memcmp(info1 + memb->off, info2 + memb->off, diff->sz)) {
      if (memb->type == T_FLOAT || memb->type == T_DOUBLE) {
        diff->type = DIFF_REAL;
      } else {
        diff->type = DIFF_INTEGER;
      }
      diff->off = info1 - diffst->info1 + memb->off;
      if (diffset->len == (KR_NDIFFS - 1)) return -1;
      diffset->len++;
    }
  }
  return 0;
}

static int idiff(diff_st *diffst, void *info1, void *info2,
 const struct_info *meta) {
  int i;
  int j;
  int res;
  int index;
  int utype;
  size_t off;
  size_t sz;
  char index_str[16];
  int uindex;
  int eindex;
  kr_diffset *diffset;
  kr_diff *diff;
  index = 0;
  off = 0;
  struct_info dummy;
  diffset = diffst->diffset;
  if (!meta->member_count || !diffst->sz) return 0;
  if (diffst->sz <= (info1 - diffst->info1)) {
    printf("diff: FRAK %zu %zu %s\n",
     diffst->sz, (info1 - diffst->info1), meta->name);
    return -1;
  }
  for (i = 0; i < meta->member_count; i++) {
    diff = &diffset->diffs[diffset->len];
    sz = meta->members[i].sz;
    off = meta->members[i].off;
    if (union_check(meta, i)) {
      eindex = meta->members[i].type_info.subenum_info.idx;
      memcpy(&utype, info1 + off, sz);
      if (memcmp(info1 + off, info2 + off, sz)) {
        if (*(int32_t *)(info2 + off) != 0) {
          printf("Error: union content mismatch! %s %zu %zu\n",
           diff->path, diff->off, diff->sz);
          return -1;
        } else {
          uindex = enum_to_index(eindex, utype);
          diff->off = info1 - diffst->info1 + off;
          diff->type = DIFF_ENUM;
          diff->enum_idx = eindex;
          diff->sz = sz;
          if (diffset->len == (KR_NDIFFS - 1)) return -1;
          diffset->len++;
        }
      }
      if (meta->members[i+1].type == T_UNION) {
        index = meta->members[i+1].type_info.subunion_info.idx;
      }
      if (meta->members[i+1].type == T_AUNION) {
        index = meta->members[i+1].type_info.anon_info.idx;
      }
      uindex = enum_to_index(eindex, utype);
      if (uindex < 0) return -1;
      dummy.member_count = 1;
      dummy.members[0] = meta_info[index].members[uindex];
      strcpy(dummy.members[0].name, enum_to_str(eindex, utype));
      res = idiff(diffst, info1, info2, &dummy);
      if (res != 0) return -1;
      i++;
      continue;
    }
    if (meta->members[i].type == T_ENUM) {
      if (memcmp(info1 + off, info2 + off, sz)) {
        index = meta->members[i].type_info.subenum_info.idx;
        diff->type = DIFF_ENUM;
        diff->enum_idx = index;
        diff->off = info1 - diffst->info1 + off;
        diff->sz = sz;
        diffset->len++;
      }
      continue;
    }
    if (is_primitive(&meta->members[i])) {
      res = handle_primitive(diffst, diff, &meta->members[i], info1, info2);
      if (res != 0) return -1;
      continue;
    }
    if (meta->members[i].type == T_STRUCT) {
      index = meta->members[i].type_info.substruct_info.idx;
      if (meta->members[i].arr) {
        sz = (meta->members[i].sz / meta->members[i].arr);
        for (j = 0; j < meta->members[i].arr; j++) {
          off = meta->members[i].off + (sz * j);
          res = snprintf(index_str, sizeof(index_str), "%d", j);
          if (res >= sizeof(index_str)) return -1;
          res = idiff(diffst, info1 + off, info2 + off, &meta_info[index]);
          if (res != 0) return -1;
        }
      } else {
        if (index < 0) return -1;
        off = meta->members[i].off;
        res = idiff(diffst, info1 + off, info2 + off, &meta_info[index]);
        if (res != 0) return -1;
      }
      continue;
    }
    if (meta->members[i].type == T_ASTRUCT) {
      index = meta->members[i].type_info.anon_info.idx;
      if (index < 0) return -1;
      off = meta->members[i].off;
      res = idiff(diffst, info1 + off, info2 + off, &meta_info[index]);
      if (res != 0) return -1;
      continue;
    }
  }
  return 0;
}

int info_diff(kr_diffset *diffset, void *info1, void *info2, int idx) {
  int res;
  diff_st diffst;
  if (diffset == NULL) return -1;
  if (info1 == NULL || info2 == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  memset(&diffst, 0, sizeof(diffst));
  diffst.diffset = diffset;
  diffst.info1 = info1;
  diffst.info2 = info2;
  diffst.sz = meta_info[idx].sz;
  diffset->type = kr_metainfo_type_to_info_type(idx);
  diffset->on = info1;
  res = idiff(&diffst, info1, info2, &meta_info[idx]);
  return res;
}
