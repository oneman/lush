#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <ctype.h>
#include <math.h>
#include <errno.h>

typedef struct {
  char *val;
  size_t len;
} str_val;

typedef struct {
  str_val label;
  str_val val;
} limit;

static void enum_val_gen(limit *lim, FILE *out, size_t idx) {
  fprintf(out, "  KR_%.*s_LIMIT = %zu,\n", (int)lim->label.len,
   lim->label.val, idx);
}

static void gen_limit(limit *lim, FILE *out) {
  fprintf(out, "    [KR_%.*s_LIMIT] = {\n", (int)lim->label.len, lim->label.val);
  fprintf(out, "      .label = \"%.*s\",\n", (int)lim->label.len, lim->label.val);
  fprintf(out, "      .value = %.*s\n", (int)lim->val.len, lim->val.val);
  fprintf(out, "    },\n");
}

int limits_gen(char **argv) {
  size_t sz;
  size_t n;
  char *line;
  char *del;
  FILE *file;
  FILE *out;
  limit lim;
  size_t k;
  file = fopen(argv[0], "r");
  if (file == NULL) return -1;
  out = fopen(argv[1], "w+");
  if (out == NULL) return -1;
  line = NULL;
  sz = 0;
  n = 0;
  k = 0;
  fprintf(out, "#if !defined(_gen_limits_H)\n# define _gen_limits_H (1)\n\n");
  fprintf(out, "#include <krad/radio/limits.h>\n#include <krad/radio/limit_types.h>\n\n");
  fprintf(out, "typedef enum {\n");
  while ( (sz = getline(&line, &n, file)) != -1) {
    if (sz > 2 && !memcmp(line, "//", 2)) continue;
    if (sz > 11 && !memcmp(line, "#define KR_", 11)) {
      lim.label.val = line + 11;
      del = strchr(lim.label.val, ' ');
      if (del == NULL) continue;
      lim.label.len = del - lim.label.val;
      lim.val.val = del + 1;
      lim.val.len = line + sz - lim.val.val - 1;
      //printf("LABEL: %.*s\n", (int)lim.label.len, lim.label.val);
      //printf("VALUE: %.*s\n", (int)lim.val.len, lim.val.val);
      enum_val_gen(&lim, out, k);
      k++;
    }
  }
  fprintf(out, "  KR_LIMIT_LAST = %zu\n", k);
  fprintf(out, "} kr_limit_type;\n\n");
  sz = 0;
  n = 0;
  rewind(file);
  fprintf(out, "static const kr_limits_info kr_limits = {\n");
  fprintf(out, "  .len = %zu,\n", k);
  fprintf(out, "  .limits = {\n");
  while ( (sz = getline(&line, &n, file)) != -1) {
    if (sz > 11 && !memcmp(line, "#define KR_", 11)) {
      lim.label.val = line + 11;
      del = strchr(lim.label.val, ' ');
      if (del == NULL) continue;
      lim.label.len = del - lim.label.val;
      lim.val.val = del + 1;
      lim.val.len = line + sz - lim.val.val - 1;
      //printf("LABEL: %.*s\n", (int)lim.label.len, lim.label.val);
      //printf("VALUE: %.*s\n", (int)lim.val.len, lim.val.val);
      gen_limit(&lim, out);
    }
  }
  fprintf(out, "  }\n");
  fprintf(out, "};\n\n");
  fprintf(out, "#endif");
  fclose(file);
  fclose(out);
  return 0;
}


int main(int argc, char *argv[]) {
  int ret;
  if (argc != 3) return 1;
  ret = limits_gen(&argv[1]);
  if (ret != 0) fprintf(stderr, "Failure\n");
  return ret;
}
