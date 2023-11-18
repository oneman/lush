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
  char *ext;
  size_t len;
} mime_ext;

typedef struct {
  char *mime;
  size_t len;
  mime_ext exts[8];
  uint8_t nexts;
} mime_info;

static int mime_parse(mime_info *info, char *line) {
  uint8_t space;
  int8_t mime;
  int8_t ext;
  space = 0;
  mime = 0;
  ext = 0;
  while (*line != '\0') {
    if (isspace(*line)) {
      space = 1;
      if (mime > 0) {
        info->len = mime;
        mime = -1;
      }
      if (ext > 0) {
        info->exts[info->nexts].len = ext;
        ext = 0;
        info->nexts++;
      }
    } else {
      if (space) space = 0;
      if (!mime) {
        info->mime = line;
        mime++;
      } else if (mime > 0) {
        mime++;
      } else {
        if (!ext) {
          info->exts[info->nexts].ext = line;
        }
        ext++;
      }
    }
    line++;
  }
  if (ext > 0) {
    info->exts[info->nexts].len = ext;
    info->nexts++;
  }
  if (mime != -1) return -1;
  return 0;
}

static void mime_to_ext_gen(FILE *c, mime_info *info) {
  if (info->nexts == 0) return;
  fprintf(c, "  if (!strncmp(mime, \"%.*s\", %d)) {\n"
    "    return \"%.*s\";\n  }\n",
    (int)info->len, info->mime, (int)info->len,
     (int)info->exts[0].len, info->exts[0].ext);
}

static void ext_to_mime_gen(FILE *c, mime_info *info) {
  int i;
  if (info->nexts == 0) return;
  for (i = 0; i < info->nexts; i++) {
    fprintf(c, "  if (!strncmp(ext, \"%.*s\", %d)) {\n"
    "    return \"%.*s\";\n  }\n", (int)info->exts[i].len, info->exts[i].ext,
     (int)info->exts[i].len, (int)info->len, info->mime);
  }
}

static int mime_gen(char **argv) {
  mime_info info;
  char *line;
  size_t n;
  size_t sz;
  FILE *file;
  FILE *c;
  FILE *h;
  memset(&info, 0, sizeof(info));
  file = fopen(argv[0], "r");
  if (file == NULL) return -1;
  c = fopen(argv[1], "w+");
  if (c == NULL) return -1;
  h = fopen(argv[2], "w+");
  if (h == NULL) return -1;
  line = NULL;
  n = 0;
  fprintf(c, "#include <stdio.h>\n#include <string.h>\n\n"
    "char *mime_to_ext(char *mime) {\n  if (!mime) return NULL;\n");
  while ( (sz = getline(&line, &n, file)) != -1) {
    if (line[sz - 2] == ';') {
      line[sz - 2] = '\0';
      if (mime_parse(&info, line) == 0) mime_to_ext_gen(c, &info);
      memset(&info, 0, sizeof(info));
    }
  }
  fprintf(c, "  return NULL;\n}\n");
  rewind(file);
  fprintf(c, "\nchar *ext_to_mime(char *ext) {\n  if (!ext) return NULL;\n");
  while ( (sz = getline(&line, &n, file)) != -1) {
    if (line[sz - 2] == ';') {
      line[sz - 2] = '\0';
      if (mime_parse(&info, line) == 0) ext_to_mime_gen(c, &info);
      memset(&info, 0, sizeof(info));
    }
  }
  fprintf(c, "  return NULL;\n}\n");
  if (line) free(line);
  fprintf(h, "#if !defined(_gen_mime_H)\n# define _gen_mime_H (1)\n"
    "char *mime_to_ext(char *mime);\nchar *ext_to_mime(char *ext);\n#endif\n");
  fclose(file);
  fclose(c);
  fclose(h);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 4) return 1;
  ret = mime_gen(&argv[1]);
  if (ret != 0) fprintf(stderr, "Failure\n");
  return ret;
}
