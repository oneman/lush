#include "codegen_bootstrap_utils.h"
#include "bootstrapped.h"

static void memb_data_info_to_init(member_info *memb, FILE *out) {
  char upp[256];
  char enum_init[256];
  switch (memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        fprintf(out,"          .char_info = {\n            .max = %d,\n            .notnull = %d\n          }",memb->type_info.char_info.max,memb->type_info.char_info.notnull);
      }
      break;
    }
    case T_INT32: {
      fprintf(out,"          .int32_info = {\n            .init = %d,\n            .min = %d,\n            .max = %d\n          }",
        memb->type_info.int32_info.init,
        memb->type_info.int32_info.min,
        memb->type_info.int32_info.max);
      break;
    }
    case T_INT64: {
      fprintf(out,"          .int64_info = {\n            .init = %" PRId64 ",\n            .min = %" PRId64 ",\n            .max = %" PRId64 "\n          }",
        memb->type_info.int64_info.init,
        memb->type_info.int64_info.min,
        memb->type_info.int64_info.max);
      break;
    }
    case T_UINT32: {
      fprintf(out,"          .uint32_info = {\n            .init = %u,\n            .min = %u,\n            .max = %u\n          }",
        memb->type_info.uint32_info.init,
        memb->type_info.uint32_info.min,
        memb->type_info.uint32_info.max);
      break;
    }
    case T_UINT64: {
      fprintf(out,"          .uint64_info = {\n            .init = %" PRIu64 ",\n            .min = %" PRIu64 ",\n            .max = %" PRIu64 "\n          }",
        memb->type_info.uint64_info.init,
        memb->type_info.uint64_info.min,
        memb->type_info.uint64_info.max);
      break;
    }
    case T_FLOAT: {
      fprintf(out,"          .float_info = {\n            .init = %0.2f,\n            .min = %0.2f,\n            .max = %0.2f\n          }",
        memb->type_info.float_info.init,
        memb->type_info.float_info.min,
        memb->type_info.float_info.max);
      break;
    }
    case T_DOUBLE: {
      fprintf(out,"          .double_info = {\n            .init = %0.2f,\n            .min = %0.2f,\n            .max = %0.2f\n          }",
        memb->type_info.double_info.init,
        memb->type_info.double_info.min,
        memb->type_info.double_info.max);
      break;
    }
    case T_STRUCT: {
      uppercase(memb->type_info.substruct_info.type_name,upp);
      fprintf(out,"          .substruct_info = {\n            .type_name = \"%s\",\n            .idx = KR_%s_META_INFO\n          }",memb->type_info.substruct_info.type_name,
        upp);
      break;
    }
    case T_UNION: {
      uppercase(memb->type_info.substruct_info.type_name,upp);
      fprintf(out,"          .subunion_info = {\n            .type_name = \"%s\",\n            .idx = KR_%s_META_INFO\n          }",memb->type_info.substruct_info.type_name,
        upp);
      break;
    }
    case T_ENUM: {
      lowercase(memb->type_info.substruct_info.enum_init, enum_init);
      uppercase(memb->type_info.substruct_info.type_name,upp);
      fprintf(out,"          .subenum_info = {\n            .type_name = \"%s\",\n            .enum_init = \"%s\",\n            .idx = KR_%s_META_INFO\n          }",memb->type_info.substruct_info.type_name,
       enum_init,
        upp);
      break;
    }
    case T_ASTRUCT: {
      fprintf(out,"          .anon_info = {\n            .info = NULL,\n            .idx = %d\n          }",memb->type_info.anon_info.idx + 1);
      break;
    }
    case T_AUNION: {
      fprintf(out,"          .anon_info = {\n            .info = NULL,\n            .idx = %d\n          }",memb->type_info.anon_info.idx + 1);
      break;
    }
    default : break;
  }
  return;
}

void handle_anon(struct_info *ainfo, int root) {
  int i;
  ainfo->root_idx = root;
  for (i = 0; i < ainfo->member_count; i++) {
    if (ainfo->members[i].type == T_AUNION || ainfo->members[i].type == T_ASTRUCT) {
      handle_anon(ainfo->members[i].type_info.anon_info.info, root);
    }
  }
}

int codegen_anon_to_idx(struct_info **info, int n, struct_info *ainfo) {
  int i;
  for (i = 0; i < n; i++) {
    if (info[i] == ainfo) {
      return i;
    }
  }
  return 0;
}

int init_members(struct_info **info, int n) {
  int i;
  int j;
  int idx;
  char *name;
  for (i = 0; i < n; i++) {
    for (j = 0; j < info[i]->member_count; j++) {
      if (info[i]->members[j].type == T_STRUCT) {
        name = info[i]->members[j].type_info.substruct_info.type_name;
        if ((idx = codegen_is_union(name))) {
          info[i]->members[j].type = T_UNION;
          info[i]->members[j].type_info.substruct_info.idx = idx;
        } else if ((idx = codegen_is_enum(name))) {
          info[i]->members[j].type = T_ENUM;
          info[i]->members[j].type_info.substruct_info.idx = idx;
        } else if ( (idx = codegen_string_to_enum(name)) ) {
          info[i]->members[j].type_info.substruct_info.idx = idx;
        }
      } else if (info[i]->members[j].type == T_ANON) {
        info[i]->members[j].type_info.anon_info.idx = codegen_anon_to_idx(info, n, info[i]->members[j].type_info.anon_info.info);
        if (info[i]->members[j].type_info.anon_info.info->type == ST_AUNION) {
          info[i]->members[j].type = T_AUNION;
        } else {
          info[i]->members[j].type = T_ASTRUCT;
        }
        if (info[i]->type == ST_ASTRUCT || info[i]->type == ST_AUNION) {
          handle_anon(info[i]->members[j].type_info.anon_info.info, info[i]->root_idx);
        } else {
          handle_anon(info[i]->members[j].type_info.anon_info.info, i);
        }
      }
    }
  }
  return 0;
}

static int enum_prefix_strip(struct_info *enu, uint idx, int min,
 char *buf, size_t sz) {
  int i;
  int j;
  int plen;
  int us;
  if (!enu || !buf || !sz) return -1;
  if (idx >= enu->member_count) return -2;
  plen = 0;
  us = 0;
  //printf("%s %d\n", enu->members[idx].name, min);
  for (i = 0; i < min; i++) {
    for (j = 0; j < enu->member_count; j++) {
      if (j == idx) continue;
      if (strncmp(enu->members[idx].name, enu->members[j].name, i + 1)) break;
    }
    if (j != enu->member_count) break;
    if (enu->members[idx].name[i] == '_') us = i + 1;
    plen = i + 1;
  }

  if (!us) plen = 0;
  if (plen) plen = us;

  lowercase(enu->members[idx].name + plen, buf);
  return 0;
}

void enum_to_str_util(struct_info **enums, int n, FILE *out) {
  int i;
  int j;
  int min;
  int len;
  char upp[256];
  char low[256];
  fprintf(out, "char *enum_to_str(int idx, int value) {\n");
  fprintf(out, "  switch(idx) {\n");
  for (i = 0; i < n; i++) {
    uppercase(enums[i]->name, upp);
    fprintf(out, "    case KR_%s_META_INFO: {\n",upp);
    fprintf(out, "      switch(value) {\n");
    for (j = 0; j < enums[i]->member_count; j++) {
      if (!strchr(enums[i]->members[j].name, '_')) break;
      len = strlen(enums[i]->members[j].name);
      if (j == 0) min = len;
      if (len < min) min = len;
    }
    if (j != enums[i]->member_count) min = 0;
    for (j = 0; j < enums[i]->member_count; j++) {
      if (enums[i]->member_count == 1) min = 0;
      enum_prefix_strip(enums[i], j, min, low, sizeof(low));
      fprintf(out, "        case %s: return \"%s\";\n",
        enums[i]->members[j].name,low);
    }
    fprintf(out, "      }\n      break;\n    }\n");
  }
  fprintf(out, "  }\n  return \"unknown\";\n}\n\n");
}

void str_to_enum_util(struct_info **enums, int n, FILE *out) {
  int i;
  int j;
  int min;
  int len;
  char upp[256];
  char low[256];
  fprintf(out, "int str_to_enum(int idx, char *str) {\n  const char *name;\n");
  fprintf(out, "  name = strstr(str, \"KR_\");\n");
  fprintf(out, "  if (name) name = str + 3;\n  else name = str;\n");
  fprintf(out, "  switch(idx) {\n");
  for (i = 0; i < n; i++) {
    uppercase(enums[i]->name, upp);
    fprintf(out, "    case KR_%s_META_INFO: {\n",upp);
    for (j = 0; j < enums[i]->member_count; j++) {
      if (!strchr(enums[i]->members[j].name, '_')) break;
      len = strlen(enums[i]->members[j].name);
      if (j == 0) min = len;
      if (len < min) min = len;
    }
    if (j != enums[i]->member_count) min = 0;
    for (j = 0; j < enums[i]->member_count; j++) {
      if (enums[i]->member_count == 1) min = 0;
      enum_prefix_strip(enums[i], j, min, low, sizeof(low));
      fprintf(out, "      if (!strcasecmp(name, \"%s\")) {\n",low);
      fprintf(out, "        return %s;\n", enums[i]->members[j].name);
      fprintf(out, "      }\n");
    }
    fprintf(out, "      break;\n    }\n");
  }
  fprintf(out, "  }\n  return -1;\n}\n\n");
}

void enum_to_index_util(struct_info **enums, int n, FILE *out) {
  int i;
  int j;
  char upp[256];
  fprintf(out, "int enum_to_index(int idx, int value) {\n");
  fprintf(out, "  switch(idx) {\n");
  for (i = 0; i < n; i++) {
    uppercase(enums[i]->name, upp);
    fprintf(out, "    case KR_%s_META_INFO: {\n",upp);
    fprintf(out, "      switch(value) {\n");
    for (j = 0; j < enums[i]->member_count; j++) {
      fprintf(out, "        case %s: return %d;\n",enums[i]->members[j].name,j);
    }
    fprintf(out, "      }\n      break;\n    }\n");
  }
  fprintf(out, "  }\n  return -1;\n}\n\n");
}

int enum_utils(struct_info **info, int n, FILE *out) {
  struct_info *enums[n];
  int i;
  int count;
  for (i = count = 0; i < n; i++) {
    if (info[i]->type == ST_ENUM) {
      enums[count++] = info[i];
    }
  }
  enum_to_str_util(enums, count, out);
  str_to_enum_util(enums, count, out);
  enum_to_index_util(enums, count, out);
  return 0;
}

int enum_gen(struct_info **info, int n, FILE *out) {
  int i;
  char upp[256];
  fprintf(out,"enum kr_meta_info_type {\n");
  for (i = 0; i < n; i++) {
    uppercase(info[i]->name,upp);
    fprintf(out, "  KR_%s_META_INFO", upp);
    if (i == 0) {
      fprintf(out, " = 1");
    }
    fprintf(out, ",\n");
  }
  fprintf(out,"};\n\n");
  fprintf(out,"enum kr_meta_info_last {\n");
  fprintf(out,"  KR_META_INFO_LAST = %d\n",n);
  fprintf(out,"};\n\n");
  return 0;
}

int init_code_gen(struct_info **info, int n, FILE *out) {
  int i;
  int j;
  char upp[256];
  for (i = 0; i < n; i++) {
    uppercase(info[i]->name,upp);
    fprintf(out,"\n  [KR_%s_META_INFO] = {\n",upp);
    fprintf(out,"    .type = %d,\n",info[i]->type);
    fprintf(out,"    .name = \"%s\",\n",info[i]->name);
    fprintf(out,"    .member_count = %d,\n",info[i]->member_count);
    fprintf(out,"    .members = {\n");
    for (j = 0; j < info[i]->member_count; j++) {
      fprintf(out,"      {\n");
      fprintf(out,"        .name = \"%s\",\n",info[i]->members[j].name);
      if (info[i]->members[j].len_def[0]) {
        fprintf(out,"        .arr = %s,\n",info[i]->members[j].len_def);
      } else {
        fprintf(out,"        .arr = %d,\n",info[i]->members[j].arr);
      }
      fprintf(out,"        .arr_notnull = %d,\n",info[i]->members[j].arr_notnull);
      fprintf(out,"        .len_def = \"%s\",\n",info[i]->members[j].len_def);
      fprintf(out,"        .ptr = %d,\n",info[i]->members[j].ptr);
      if (info[i]->type != ST_AUNION && info[i]->type != ST_ASTRUCT) {
        if (info[i]->members[j].type != T_ASTRUCT && info[i]->members[j].type != T_AUNION) {
          if (info[i]->type == ST_STRUCT || info[i]->type == ST_UNION) {
            if (info[i]->is_typedef) {
              fprintf(out,"        .off = offsetof(%s, %s),\n",info[i]->name,info[i]->members[j].name);
              fprintf(out,"        .sz = sizeof(((%s*)0)->%s),\n",info[i]->name,info[i]->members[j].name);
            } else {
              if (info[i]->type == ST_STRUCT) {
                fprintf(out,"        .off = offsetof(struct %s, %s),\n",info[i]->name,info[i]->members[j].name);
                fprintf(out,"        .sz = sizeof(((struct %s*)0)->%s),\n",info[i]->name,info[i]->members[j].name);
              } else {
                fprintf(out,"        .off = offsetof(union %s, %s),\n",info[i]->name,info[i]->members[j].name);
                fprintf(out,"        .sz = sizeof(((union %s*)0)->%s),\n",info[i]->name,info[i]->members[j].name);
              }
            }
          }
        }
      } else {
        if (info[i]->members[j].type != T_ASTRUCT && info[i]->members[j].type != T_AUNION) {
          if (info[info[i]->root_idx]->type == ST_STRUCT || info[info[i]->root_idx]->type == ST_UNION) {
            if (info[info[i]->root_idx]->is_typedef) {
              fprintf(out,"        .off = offsetof(%s, %s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
              fprintf(out,"        .sz = sizeof(((%s*)0)->%s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
            } else {
              if (info[info[i]->root_idx]->type == ST_STRUCT) {
                fprintf(out,"        .off = offsetof(struct %s, %s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
                fprintf(out,"        .sz = sizeof(((struct %s*)0)->%s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
              } else {
                fprintf(out,"        .off = offsetof(union %s, %s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
                fprintf(out,"        .sz = sizeof(((union %s*)0)->%s),\n",info[info[i]->root_idx]->name,info[i]->members[j].name);
              }
            }
          }
        }
      }
      fprintf(out,"        .opt = %d,\n",info[i]->members[j].opt);
      fprintf(out,"        .type = %d,\n",info[i]->members[j].type);
      fprintf(out,"        .type_info = {\n");
      memb_data_info_to_init(&info[i]->members[j], out);
      fprintf(out,"\n        }\n");
      fprintf(out,"      }");
      if (j != (info[i]->member_count - 1)) fprintf(out,",");
      fprintf(out,"\n");
    }
    fprintf(out,"    },\n");
    if (info[i]->type == ST_STRUCT) {
      if (info[i]->is_typedef) {
        fprintf(out,"    .sz = sizeof(%s),\n",info[i]->name);
      } else {
        fprintf(out,"    .sz = sizeof(struct %s),\n",info[i]->name);
      }
    } else if (info[i]->type == ST_ENUM) {
      if (info[i]->is_typedef) {
        fprintf(out,"    .sz = sizeof(%s),\n",info[i]->name);
      } else {
        fprintf(out,"    .sz = sizeof(enum %s),\n",info[i]->name);
      }
    } else if (info[i]->type == ST_UNION) {
      if (info[i]->is_typedef) {
        fprintf(out,"    .sz = sizeof(%s),\n",info[i]->name);
      } else {
        fprintf(out,"    .sz = sizeof(union %s),\n",info[i]->name);
      }
    } else if (info[i]->type == ST_ASTRUCT || info[i]->type == ST_AUNION) {
      fprintf(out,"    .root_idx = %d,\n",info[i]->root_idx + 1);
    }
    fprintf(out,"    .is_typedef = %d\n",info[i]->is_typedef);
    fprintf(out,"  },");
  }
  return 0;
}

void print_usage(char *cmd) {
  printf("syntax:\n%s path metainfo.h metainfo.c\n",cmd);
}

int main(int argc, char *argv[]) {
  FILE *outh;
  FILE *outc;
  header_data *hdata;
  struct_info *info[512];
  char *subpath;
  int n;
  int i;
  int j;
  int total;
  if (argc != 4) {
    print_usage(argv[0]);
    return 1;
  }
  outh = fopen(argv[2], "w+");
  outc = fopen(argv[3], "w+");
  if (outh == NULL || outc == NULL) return 1;
  fprintf(outh, "#if !defined(_gen_meta_info_H)\n# define _gen_meta_info_H (1)\n\n");
  fprintf(outh,"#include <stddef.h>\n");
  fprintf(outh,"#include <strings.h>\n");
  fprintf(outh,"#include \"struct_info.h\"\n");
  fprintf(outc,"#include \"metainfo.h\"\n\n");
  hdata = hdata_init(MAX_HEADERS);
  n = gather_struct_definitions(hdata,"types",argv[1]);
  for (i = total = 0; i < n; i++) {
    if (hdata[i].target_count > 0) {
      for (j = 0; j < hdata[i].def_count; j++) {
        info[total++] = &hdata[i].defs[j].info;
      }
      subpath = strstr(hdata[i].path, "/lib/");
      if (subpath != NULL) {
        fprintf(outh, "#include <krad/%s>\n", subpath + 5);
      }
    }
  }
  init_members(info, total);
  enum_utils(info, total, outc);
  enum_gen(info, total, outh);
  fprintf(outh,"extern const struct_info meta_info[];\n\n");
  fprintf(outc,"const struct_info meta_info[] = {");
  init_code_gen(info, total, outc);
  fprintf(outc,"\n};\n\n");
  fprintf(outh,"char *enum_to_str(int idx, int value);\n");
  fprintf(outh,"int str_to_enum(int idx, char *str);\n");
  fprintf(outh,"int enum_to_index(int idx, int value);\n\n");
  fprintf(outh, "#endif\n\n");
  hdata_free(hdata, MAX_HEADERS);
  fclose(outh);
  fclose(outc);
  return 0;
}
