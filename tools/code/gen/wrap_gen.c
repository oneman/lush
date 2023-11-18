#include "codegen_bootstrap_utils.h"
#include "bootstrapped.h"

void print_usage(char *cmd) {
  printf("syntax:\n%s path\n",cmd);
}

void to_text_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_to_text(char *text, void *info, int max);\n", info->name);
  }
}

void pack_json_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_pack_json(char *json, void *info, int max);\n", info->name);
  }
}

void unpack_json_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_unpack_json(char *json, void *info);\n", info->name);
  }
}

void pack_ebml_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_pack_ebml(kr_ebml *ebml, void *info);\n", info->name);
  }
}

void unpack_ebml_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_unpack_ebml(kr_ebml *ebml, void *info);\n", info->name);
  }
}

void helpers_wrap_gen_header(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type == ST_ENUM) {
      fprintf(target, "int str_to_%s(char *str);\n", info->name);
      fprintf(target, "char *%s_to_str(int val);\n", info->name);
    }
    if (info->type != ST_STRUCT) continue;
    fprintf(target, "int %s_init(void *info);\n", info->name);
    fprintf(target, "int %s_valid(void *info);\n", info->name);
    fprintf(target, "int %s_random(void *info);\n", info->name);
    fprintf(target, "int %s_patch_path(kr_patch *patch, kr_path *path);\n", info->name);
    fprintf(target, "int %s_diff(kr_diffset *diffset, void *info1, void *info2);\n", info->name);
  }
}

void to_text_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_to_text(char *text, void *info, int max) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_to_text(text, info, max, %s);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
  }
}

void pack_json_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_pack_json(char *json, void *info, int max) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_pack_json(json, info, max, %s);\n  return res;\n",
     enum_val);
    fprintf(target, "}\n\n");
  }
}

void unpack_json_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_unpack_json(char *json, void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_unpack_json(json, info, %s);\n  return res;\n",
     enum_val);
    fprintf(target, "}\n\n");
  }
}

void pack_ebml_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_pack_ebml(kr_ebml *ebml, void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_pack_unpack_ebml(ebml, info, %s, 0);\n  return res;\n",
     enum_val);
    fprintf(target, "}\n\n");
  }
}

void unpack_ebml_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_unpack_ebml(kr_ebml *ebml, void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_pack_unpack_ebml(ebml, info, %s, 1);\n  return res;\n",
     enum_val);
    fprintf(target, "}\n\n");
  }
}

void helpers_wrap_gen(struct_data *data, int count, FILE *target) {
  int i;
  struct_info *info;
  char enum_val[256];
  for (i = 0; i < count; i++) {
    info = &data[i].info;
    if (info->type == ST_ENUM) {
      sprintf(enum_val, "KR_%s_META_INFO", info->name);
      uppercase(enum_val, enum_val);
      fprintf(target, "int str_to_%s(char *str) {\n", info->name);
      fprintf(target, "  return str_to_enum(%s, str);\n", enum_val);
      fprintf(target, "}\n\n");
      fprintf(target, "char *%s_to_str(int val) {\n", info->name);
      fprintf(target, "  return enum_to_str(%s, val);\n", enum_val);
      fprintf(target, "}\n\n");
    }
    if (info->type != ST_STRUCT) continue;
    sprintf(enum_val, "KR_%s_META_INFO", info->name);
    uppercase(enum_val, enum_val);
    fprintf(target, "int %s_init(void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_helpers(info, %s, 1);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
    fprintf(target, "int %s_valid(void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_helpers(info, %s, 2);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
    fprintf(target, "int %s_random(void *info) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_helpers(info, %s, 3);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
    fprintf(target, "int %s_patch_path(kr_patch *patch, kr_path *path) {\n  int res;\n", info->name);
    fprintf(target, "  res = patch_path(patch, path, %s);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
    fprintf(target, "int %s_diff(kr_diffset *diffset, void *info1, void *info2) {\n  int res;\n", info->name);
    fprintf(target, "  res = info_diff(diffset, info1, info2, %s);\n  return res;\n", enum_val);
    fprintf(target, "}\n\n");
  }
}

int wrap_gen(header_data *hdata, FILE *wrappers) {
  int i;
  int res;
  FILE *targeth;
  FILE *targetc;
  char fpath[PATH_MAX];
  char define[PATH_MAX];
  char *substr;
  char *substr2;
  printf("reading %s\n",hdata->path);
  for (i = 0; i < hdata->target_count; i++) {
    res = snprintf(fpath, PATH_MAX, "%s", hdata->targets[i].path);
    if (res >= PATH_MAX) return -1;
    fpath[res - 1] = 'c';
    printf("  found target %s\n",hdata->targets[i].path);
    targeth = fopen(hdata->targets[i].path, "w+");
    targetc = fopen(fpath, "w+");
    if (targeth == NULL || targetc == NULL) return -1;
    substr = strstr(hdata->targets[i].path, "/lib/");
    if (substr != NULL) {
      substr2 = strstr(&substr[5], ".h");
      if (substr2 == NULL) return -1;
      substr2[1] = 'c';
      fprintf(wrappers, "#include \"../%s\"\n", &substr[5]);
      substr2[1] = 'h';
      slashdotscore(define, &substr[5]);
      uppercase(define, define);
      fprintf(targeth, "#if !defined(_%s)\n#define _%s (1)\n\n", define, define);
    }
    fprintf(targeth, "#include <stdio.h>\n");
    fprintf(targeth, "#include <stdint.h>\n");
    fprintf(targeth, "#include \"../%s\"\n", basename(hdata->path));
    fprintf(targetc, "#include \"%s\"\n\n",basename(hdata->targets[i].path));
    fprintf(targetc, "#include <krad/util/util.h>\n");
    switch(hdata->targets[i].type) {
      case TO_TEXT: {
        to_text_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        to_text_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      case TO_JSON: {
        pack_json_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        pack_json_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      case FR_JSON: {
        unpack_json_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        unpack_json_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      case TO_EBML: {
        fprintf(targeth, "#include <krad/ebml/ebml.h>\n");
        pack_ebml_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        pack_ebml_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      case FR_EBML: {
        fprintf(targeth, "#include <krad/ebml/ebml.h>\n");
        unpack_ebml_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        unpack_ebml_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      case HELPERS: {
        fprintf(targeth, "#include <krad/app/path.h>\n");
        fprintf(targeth, "#include <krad/app/patch.h>\n");
        fprintf(targeth, "#include <krad/app/diff.h>\n");
        helpers_wrap_gen_header(hdata->defs, hdata->def_count, targeth);
        helpers_wrap_gen(hdata->defs, hdata->def_count, targetc);
        break;
      }
      default: break;
    }
    if (substr != NULL) {
      fprintf(targeth, "#endif\n\n");
    }
    fclose(targeth);
    fclose(targetc);
  }
  return 0;
}

int main(int argc, char *argv[]) {
  header_data *hdata;
  FILE *wrappers;
  char wrappers_path[PATH_MAX];
  int res;
  int n;
  int i;
  if (argc != 2) {
    print_usage(argv[0]);
    return 1;
  }
  hdata = hdata_init(MAX_HEADERS);
  n = gather_struct_definitions(hdata,"types",argv[1]);
  snprintf(wrappers_path, PATH_MAX, "%s/gen/wrappers.c", argv[1]);
  wrappers = fopen(wrappers_path, "w+");
  if (wrappers == NULL) return -1;
  for (i = 0; i < n; i++) {
    if (hdata[i].target_count > 0) {
      res = wrap_gen(&hdata[i], wrappers);
      if (res < 0) return 1;
    }
  }
  fclose(wrappers);
  hdata_free(hdata, MAX_HEADERS);
  return 0;
}
