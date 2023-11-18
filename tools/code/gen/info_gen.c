#include "codegen_bootstrap_utils.h"
#include "bootstrapped.h"

typedef enum {
  TYPEDEFS = 1,
  INFO_TYPES,
  KR_INFO,
  FUNC_SPEC,
  INFO_FUNCS
} gformat;

enum {
  GFORMAT_LAST = 5
};

typedef struct {
  cgen_target_type types[7];
  int count;
} util_targets;

typedef struct {
  struct_info **infos;
  int *meta_idxs;
  util_targets *targets;
  int info_count;
  int idxs_count;
  FILE *out_funcs;
  FILE *out_types;
  FILE *out_typesc;
} util_state;

void print_usage(char *cmd) {
  printf("syntax:\n%s path info_functions.h info_types.h info_types.c\n",cmd);
}

int check_for_target(util_targets *utg, cgen_target_type type) {
  int i;
  for (i = 0; i < utg->count; i++) {
    if (utg->types[i] == type) {
      return 1;
    }
  }
  return 0;
}

void info_types_gen(util_state *ustate) {
  int i;
  char upp[NAME_MAX];
  fprintf(ustate->out_types, "\ntypedef enum {\n");
  fprintf(ustate->out_types, "  KR_NO_INFO = 0,\n");
  for (i = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    fprintf(ustate->out_types, "  %s",upp);
    if (i != (ustate->info_count - 1))
      fprintf(ustate->out_types, ",");
    fprintf(ustate->out_types, "\n");
  }
  fprintf(ustate->out_types, "} kr_info_type;\n\n");
}

void str_to_info_type(util_state *ustate) {
  int i;
  char upp[NAME_MAX];
  char *psfix;
  char *name;
  fprintf(ustate->out_typesc, "int str_to_kr_info_type(char *str) {\n");
  for (i = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    psfix = strstr(ustate->infos[i]->name, "kr_");
    if (psfix) {
      name = &ustate->infos[i]->name[3];
    } else {
      psfix = strstr(ustate->infos[i]->name, "_info");
      if (psfix) {
        psfix = NULL;
        name = ustate->infos[i]->name;
      } else exit(-1);
    }
    fprintf(ustate->out_typesc, "  if (!strncmp(str, \"%s\", %u)) return %s;\n",
      name,(uint)strlen(name),upp);
  }
  fprintf(ustate->out_typesc, "  return 0;\n}\n\n");
}

void info_type_to_str(util_state *ustate) {
  int i;
  char upp[NAME_MAX];
  char *psfix;
  char *name;
  fprintf(ustate->out_typesc, "char *kr_info_type_to_str(int type) {\n");
  fprintf(ustate->out_typesc, "  switch(type) {\n");
  for (i = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    psfix = strstr(ustate->infos[i]->name, "kr_");
    name = ustate->infos[i]->name;
    if (psfix) {
      name = &ustate->infos[i]->name[3];
    }
    fprintf(ustate->out_typesc, "    case %s: return \"%s\";\n"
      ,upp,name);
  }
  fprintf(ustate->out_typesc, "    default: return \"\";\n  }\n  return 0;\n}\n");
}

void info_type_to_metainfo_type(util_state *ustate) {
  int i;
  int j;
  char upp[NAME_MAX];
  char *psfix;
  //char *name;
  fprintf(ustate->out_typesc, "int kr_info_type_to_metainfo_type(int type) {\n");
  fprintf(ustate->out_typesc, "  switch(type) {\n");
  for (i = j = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    psfix = strstr(ustate->infos[i]->name, "kr_");
    //name = ustate->infos[i]->name;
    if (psfix) {
      //name = &ustate->infos[i]->name[3];
    }
    fprintf(ustate->out_typesc, "    case %s: return KR_%s_META_INFO;\n"
      ,upp,upp);
  }
  fprintf(ustate->out_typesc, "    default: break;\n  }\n  return 0;\n}\n");
}

void metainfo_type_to_info_type(util_state *ustate) {
  int i;
  int j;
  char upp[NAME_MAX];
  char *psfix;
  //char *name;
  fprintf(ustate->out_typesc, "int kr_metainfo_type_to_info_type(int type) {\n");
  fprintf(ustate->out_typesc, "  switch(type) {\n");
  for (i = j = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    psfix = strstr(ustate->infos[i]->name, "kr_");
    //name = ustate->infos[i]->name;
    if (psfix) {
      //name = &ustate->infos[i]->name[3];
    }
    fprintf(ustate->out_typesc, "    case KR_%s_META_INFO: return %s;\n"
      ,upp,upp);
  }
  fprintf(ustate->out_typesc, "    default: break;\n  }\n  return 0;\n}\n");
}

void func_spec_gen(const char **formats, int n, FILE *out) {
  int i;
  fprintf(out, "typedef struct {\n  size_t size;\n");
  for (i = 0; i < n; i++) {
    fprintf(out, "  kr_info_type_%s *%s;\n",formats[i],formats[i]);
  }
  fprintf(out, "} kr_info_functions_spec;\n\n");
}

void typedefs_gen(const char **formats, int n, FILE *out) {

  fprintf(out, "typedef int (kr_info_type_%s)(char *text, void *st, int max);\n",formats[0]);
  fprintf(out, "typedef int (kr_info_type_%s)(char *json, void *st, int max);\n",formats[1]);
  fprintf(out, "typedef int (kr_info_type_%s)(char *json, void *st);\n",formats[2]);
  fprintf(out, "typedef int (kr_info_type_%s)(kr_ebml *ebml, void *st);\n",formats[3]);
  fprintf(out, "typedef int (kr_info_type_%s)(kr_ebml *ebml, void *st);\n",formats[4]);
  fprintf(out, "typedef int (kr_info_type_%s)(void *st);\n",formats[5]);
  fprintf(out, "typedef int (kr_info_type_%s)(void *st);\n",formats[6]);
  fprintf(out, "typedef int (kr_info_type_%s)(void *st);\n",formats[7]);
  fprintf(out, "typedef int (kr_info_type_%s)(kr_patch *patch, kr_path *path);\n",formats[8]);
  fprintf(out, "typedef int (kr_info_type_%s)(kr_diffset *diffset, void *info1, void *info2);\n",formats[9]);
}

void info_funcs_gen(util_state *ustate, const char **formats, int fcount) {
  int i;
  int j;
  char upp[NAME_MAX];
  fprintf(ustate->out_funcs, "static const kr_info_functions_spec info_functions[] = {\n");
  for (i = 0; i < ustate->info_count; i++) {
    uppercase(ustate->infos[i]->name, upp);
    fprintf(ustate->out_funcs, "  [%s] = {\n",upp);
    if (ustate->infos[i]->is_typedef) {
      fprintf(ustate->out_funcs, "    .size = sizeof(%s),\n",ustate->infos[i]->name);
    } else {
      fprintf(ustate->out_funcs, "    .size = sizeof(struct %s),\n",ustate->infos[i]->name);
    }
    for (j = 0; j < fcount - 5; j++) {
      if (check_for_target(&ustate->targets[i], j+1)) {
        fprintf(ustate->out_funcs, "    .%s = %s_%s,\n",formats[j],ustate->infos[i]->name,formats[j]);
      }
    }
    if (check_for_target(&ustate->targets[i], HELPERS)) {
      fprintf(ustate->out_funcs, "    .%s = %s_%s,\n",
        formats[5],ustate->infos[i]->name,formats[5]);
      fprintf(ustate->out_funcs, "    .%s = %s_%s,\n",
        formats[6],ustate->infos[i]->name,formats[6]);
      fprintf(ustate->out_funcs, "    .%s = %s_%s,\n",
        formats[7],ustate->infos[i]->name,formats[7]);
      fprintf(ustate->out_funcs, "    .%s = %s_%s,\n",
        formats[8],ustate->infos[i]->name,formats[8]);
      fprintf(ustate->out_funcs, "    .%s = %s_%s\n",
        formats[9],ustate->infos[i]->name,formats[9]);
    }
    fprintf(ustate->out_funcs, "  }");
    if (i != (ustate->info_count - 1))
      fprintf(ustate->out_funcs, ",");
    fprintf(ustate->out_funcs, "\n");
  }
  fprintf(ustate->out_funcs, "};\n\n");
}

void kr_info_gen(util_state *ustate) {
  int i;
  char *psfix;
  char *name;
  fprintf(ustate->out_funcs,"union kr_info {\n  uint8_t buf[8];\n");
  for (i = 0; i < ustate->info_count; i++) {
    psfix = strstr(ustate->infos[i]->name, "kr_");
    if (psfix)
      name = &ustate->infos[i]->name[3];
    else {
      psfix = strstr(ustate->infos[i]->name, "_info");
      if (psfix) {
        psfix = NULL;
        name = ustate->infos[i]->name;
      } else exit(-1);
    }
    if (ustate->infos[i]->is_typedef) {
      fprintf(ustate->out_funcs, "  %s %s;\n",ustate->infos[i]->name,name);
    } else {
      fprintf(ustate->out_funcs, "  struct %s %s;\n",ustate->infos[i]->name,name);
    }
  }
  fprintf(ustate->out_funcs,"};\n\n");
}

void init_code_gen(util_state *ustate, gformat format) {
  const char *formats[] = {"to_text","pack_json","unpack_json","pack_ebml","unpack_ebml","init","valid","random","patch_path","diff"};
  switch (format) {
    case TYPEDEFS:
      typedefs_gen(formats, sizeof(formats)/sizeof(char *), ustate->out_funcs);
      break;
    case INFO_TYPES:
      info_types_gen(ustate);
      str_to_info_type(ustate);
      info_type_to_str(ustate);
      info_type_to_metainfo_type(ustate);
      metainfo_type_to_info_type(ustate);
      break;
    case KR_INFO:
      kr_info_gen(ustate);
      break;
    case FUNC_SPEC:
      func_spec_gen(formats, sizeof(formats)/sizeof(char *), ustate->out_funcs);
      break;
    case INFO_FUNCS:
      info_funcs_gen(ustate, formats, sizeof(formats)/sizeof(char *));
      break;
  }
}

int main(int argc, char *argv[]) {
  FILE *out_funcs;
  FILE *out_types;
  FILE *out_typesc;
  util_state ustate;
  header_data *hdata;
  int idxs[512];
  struct_info *infos[512];
  util_targets targets[512];
  char *subpath;
  int n;
  int i;
  int j;
  int l;
  int info_total;
  int idxs_total;
  if (argc != 5) {
    print_usage(argv[0]);
    return 1;
  }
  memset(&ustate, 0, sizeof(ustate));
  hdata = hdata_init(MAX_HEADERS);
  n = gather_struct_definitions(hdata,"types",argv[1]);
  out_funcs = fopen(argv[2], "w+");
  out_types = fopen(argv[3], "w+");
  out_typesc = fopen(argv[4], "w+");
  if (!out_funcs || !out_types || !out_typesc) return 1;
  info_total = 0;
  idxs_total = 0;
  for (i = 0; i < n; i++) {
    for (j = 0; j < hdata[i].def_count; j++){
      if (info_total == 512) return 1;
      if (idxs_total == 512) return 1;
      if (hdata[i].defs[j].info.type == ST_STRUCT) {
        for (l = 0; l < hdata[i].target_count; l++) {
          targets[info_total].types[l] = hdata[i].targets[l].type;
          targets[info_total].count++;
        }
        if (hdata[i].target_count > 0)
          infos[info_total++] = &hdata[i].defs[j].info;
          idxs[idxs_total++] = j;
      }
    }
  }
  ustate.infos = infos;
  ustate.meta_idxs = idxs;
  ustate.targets = targets;
  ustate.info_count = info_total;
  ustate.idxs_count = idxs_total;
  ustate.out_funcs = out_funcs;
  ustate.out_types = out_types;
  ustate.out_typesc = out_typesc;
  fprintf(out_types, "#if !defined(_gen_info_types_H)\n# define _gen_info_types_H (1)\n\n"
    "int str_to_kr_info_type(char *str);\nchar *kr_info_type_to_str(int type);"
    "\nint kr_info_type_to_metainfo_type(int type);\nint kr_metainfo_type_to_info_type(int type);\n");
  fprintf(out_funcs, "#if !defined(_gen_info_functions_H)\n# define _gen_info_functions_H (1)\n\n");
  if (info_total > 0) {
    fprintf(out_funcs,"typedef union kr_info kr_info;\n\n");
    for (i = 0; i < n; i++) {
      if (hdata[i].target_count > 0) {
        subpath = strstr(hdata[i].path, "/lib/");
        if (subpath != NULL)
          fprintf(out_funcs, "#include <krad/%s>\n",subpath + 5);
      }
    }
    fprintf(out_funcs, "#include \"info_types.h\"\n");
    fprintf(out_typesc, "#include \"info_types.h\"\n#include \"metainfo.h\"\n#include <string.h>\n");
    for (i = 1; i < GFORMAT_LAST + 1; i++) {
      init_code_gen(&ustate, i);
    }
  }
  fprintf(out_funcs, "#endif\n\n");
  fprintf(out_types, "#endif\n\n");
  fclose(out_funcs);
  fclose(out_types);
  fclose(out_typesc);
  hdata_free(hdata, MAX_HEADERS);
  return 0;
}
