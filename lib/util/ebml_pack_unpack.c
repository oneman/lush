#include <krad/gen/metainfo.h>

enum ftype {
  PACK,
  UNPACK
};

static int handle_primitive(kr_ebml *ebml, void *info, const member_info *memb, int ftype) {
  int i;
  int arr_len;
  int res;
  char *actual;
  actual = (char *)info;
  res = 0;
  arr_len = 1;
  if (memb->arr) {
    arr_len = memb->arr;
  }
  switch(memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        if (ftype == PACK) {
          res = kr_ebml_pack_string(ebml, 0xE1, actual + memb->off);
        } else {
          res = kr_ebml2_unpack_element_string(ebml, NULL, actual + memb->off, memb->type_info.char_info.max + 1);
        }
        return res;
      } else  if (memb->ptr) {
        if (memb->ptr == 1) {
          break;
        } else {
          break;
        }
      } else {
        if (ftype == PACK) {
          res = kr_ebml_pack_int8(ebml, 0xE1, *((int8_t *)(actual + memb->off)));
        } else {
          res = kr_ebml2_unpack_element_int8(ebml, NULL, (int8_t *)(actual + memb->off));
        }
      }
    }
    case T_INT8: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_int8(ebml, 0xE1, *((int8_t *)(actual + memb->off + (i * sizeof(int8_t)))));
          } else {
            res = kr_ebml2_unpack_element_int8(ebml, NULL, (int8_t *)(actual + memb->off + (i * sizeof(int8_t))));
          }
        }
      }
    }
    case T_INT16: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_int16(ebml, 0xE1, *((int16_t *)(actual + memb->off + (i * sizeof(int16_t)))));
          } else {
            res = kr_ebml2_unpack_element_int16(ebml, NULL, (int16_t *)(actual + memb->off + (i * sizeof(int16_t))));
          }
        }
      }
    }
    case T_INT32: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_int32(ebml, 0xE1, *((int32_t *)(actual + memb->off + (i * sizeof(int32_t)))));
          } else {
            res = kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)(actual + memb->off + (i * sizeof(int32_t))));
          }
        }
      }
    }
    case T_INT64: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_int64(ebml, 0xE1, *((int64_t *)(actual + memb->off + (i * sizeof(int64_t)))));
          } else {
            res = kr_ebml2_unpack_element_int64(ebml, NULL, (int64_t *)(actual + memb->off + (i * sizeof(int64_t))));
          }
        }
      }
    }
    case T_UINT8: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_uint8(ebml, 0xE1, *((uint8_t *)(actual + memb->off + (i * sizeof(uint8_t)))));
          } else {
            res = kr_ebml2_unpack_element_uint8(ebml, NULL, (uint8_t *)(actual + memb->off + (i * sizeof(uint8_t))));
          }
        }
      }
    }
    case T_UINT16: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_uint16(ebml, 0xE1, *((uint16_t *)(actual + memb->off + (i * sizeof(uint16_t)))));
          } else {
            res = kr_ebml2_unpack_element_uint16(ebml, NULL, (uint16_t *)(actual + memb->off + (i * sizeof(uint16_t))));
          }
        }
      }
    }
    case T_UINT32: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_uint32(ebml, 0xE1, *((uint32_t *)(actual + memb->off + (i * sizeof(uint32_t)))));
          } else {
            res = kr_ebml2_unpack_element_uint32(ebml, NULL, (uint32_t *)(actual + memb->off + (i * sizeof(uint32_t))));
          }
        }
      }
    }
    case T_UINT64: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_uint64(ebml, 0xE1, *((uint64_t *)(actual + memb->off + (i * sizeof(uint64_t)))));
          } else {
            res = kr_ebml2_unpack_element_uint64(ebml, NULL, (uint64_t *)(actual + memb->off + (i * sizeof(uint64_t))));
          }
        }
      }
    }
    case T_FLOAT: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_float(ebml, 0xE1, *((float *)(actual + memb->off + (i * sizeof(float)))));
          } else {
            res = kr_ebml2_unpack_element_float(ebml, NULL, (float *)(actual + memb->off + (i * sizeof(float))));
          }
        }
      }
    }
    case T_DOUBLE: {
      if (!memb->ptr) {
        for (i = 0; i < arr_len; i++) {
          if (ftype == PACK) {
            res = kr_ebml_pack_double(ebml, 0xE1, *((double *)(actual + memb->off + (i * sizeof(double)))));
          } else {
            res = kr_ebml2_unpack_element_double(ebml, NULL, (double *)(actual + memb->off + (i * sizeof(double))));
          }
        }
      }
    }
    default: break;
  }
  return res;
}

static int handle_members(kr_ebml *ebml, void *act, const struct_info *info, int ftype) {
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
    if ( (ret = handle_primitive(&ebml[res], act, &info->members[i], ftype)) ) {
      res += ret;
    }
    if (info->members[i].type == T_STRUCT) {
      if (info->members[i].arr) {
        for (j = 0; j < info->members[i].arr; j++) {
          index = info->members[i].type_info.substruct_info.idx;
          res += handle_members(&ebml[res], &actual[info->members[i].off + (meta_info[index].sz * j)], &meta_info[index], ftype);
        }
      } else {
        index = info->members[i].type_info.substruct_info.idx;
        res += handle_members(&ebml[res], &actual[info->members[i].off], &meta_info[index], ftype);
      }
    }
    if (info->members[i].type == T_ENUM) {
      index = info->members[i].type_info.subenum_info.idx;
      if (ftype == PACK) {
        res = kr_ebml_pack_int32(ebml, 0xE1, *((int32_t *)(actual + info->members[i].off)));
      } else {
        res = kr_ebml2_unpack_element_int32(ebml, NULL, (int32_t *)(actual + info->members[i].off));
      }
    }
    if (info->members[i].type == T_ASTRUCT) {
      index = info->members[i].type_info.anon_info.idx;
      if (meta_info[index].member_count > 0) {
        res += handle_members(&ebml[res], actual, &meta_info[index], ftype);
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
            res += handle_members(&ebml[res], actual, &dummy, ftype);
          } else {
            res += handle_members(&ebml[res], &actual[info->members[i].off], &dummy, ftype);
          }
        }
      }
    }
  }
  return res;
}

int info_pack_unpack_ebml(kr_ebml *ebml, void *info, int idx, int ftype) {
  int res;
  if (ebml == NULL || info == NULL) return -1;
  if (idx <= 0 || idx > KR_META_INFO_LAST) return -1;
  //if (meta_info[idx].sz != sz) return -1;
  if (meta_info[idx].type != ST_STRUCT) return -1;
  if (ftype != PACK && ftype != UNPACK) return -1;
  res = handle_members(ebml, info, &meta_info[idx], ftype);
  return res;
}
