#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/ebml/ebml.h>
#include <krad/util/util.h>
#include <krad/gen/metainfo.h>
#define MAX_JS_LEN 1024 * 1024

static char upchar(char c) {
  if (c > 96 && c < 123) c -= 32;
  return c;
}

static void capize(char *capname, size_t capsz, const char *name, size_t sz) {
  if (sz < 2) return;
  memset(capname, 0, capsz);
  capname[0] = upchar(name[0]);
  memcpy(capname + 1, name + 1, sz - 1);
}

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

static size_t handle_primitive(char *js, size_t max,
 const member_info *memb) {
  size_t pos;
  pos = 0;
  int64_t i_init;
  uint64_t u_init;
  double d_init;
  i_init = 0;
  u_init = 0;
  d_init = 0.00f;
  switch(memb->type) {
    case T_CHAR:
    case T_INT8:
      i_init = memb->type_info.int8_info.init;
      break;
    case T_INT16:
      i_init = memb->type_info.int16_info.init;
      break;
    case T_INT32:
      i_init = memb->type_info.int32_info.init;
      break;
    case T_INT64:
      i_init = memb->type_info.int64_info.init;
      break;
    case T_UINT8:
      u_init = memb->type_info.int8_info.init;
      break;
    case T_UINT16:
      u_init = memb->type_info.int16_info.init;
      break;
    case T_UINT32:
      u_init = memb->type_info.int32_info.init;
      break;
    case T_UINT64:
      u_init = memb->type_info.int64_info.init;
      break;
    case T_FLOAT:
      d_init = memb->type_info.float_info.init;
      break;
    case T_DOUBLE:
      d_init = memb->type_info.double_info.init;
      break;
    default: break;
  }
  switch(memb->type) {
    case T_CHAR: {
      if (memb->arr) {
        pos += snprintf(&js[pos], max - pos, "  this.%s = \"\";\n", memb->name);
        if (pos >= max) return -1;
      } else {
        pos += snprintf(&js[pos], max - pos, "  this.%s = %"PRId64";\n",
         memb->name, i_init);
        if (pos >= max) return -1;
      }
      break;
    }
    case T_INT8:
    case T_INT16:
    case T_INT32:
    case T_INT64:
      if (memb->arr) {
        pos += snprintf(&js[pos], max - pos, "  this.%s = new Array(%d);\n",
         memb->name, memb->arr);
        if (pos >= max) return -1;
      } else {
        pos += snprintf(&js[pos], max - pos, "  this.%s = %"PRId64";\n",
         memb->name, i_init);
        if (pos >= max) return -1;
      }
      break;
    case T_UINT8:
    case T_UINT16:
    case T_UINT32:
    case T_UINT64:
      if (memb->arr) {
        pos += snprintf(&js[pos], max - pos, "  this.%s = new Array(%d);\n",
         memb->name, memb->arr);
        if (pos >= max) return -1;
      } else {
        pos += snprintf(&js[pos], max - pos, "  this.%s = %"PRIu64";\n",
         memb->name, u_init);
        if (pos >= max) return -1;
      }
      break;
    case T_FLOAT:
    case T_DOUBLE:
      if (memb->arr) {
        pos += snprintf(&js[pos], max - pos, "  this.%s = new Array(%d);\n",
         memb->name, memb->arr);
        if (pos >= max) return -1;
      } else {
        pos += snprintf(&js[pos], max - pos, "  this.%s = %0.2f;\n",
         memb->name, d_init);
        if (pos >= max) return -1;
      }
      break;
    default: break;
  }
  return pos;
}

static size_t js_proto_gen(char *js, size_t max, const struct_info *info) {
  int i;
  int j;
  int len;
  char *str;
  size_t pos;
  const member_info *memb;
  char capname[128];
  struct_info dummy;
  pos = 0;
  for (i = 0; i < info->member_count; i++) {
    memb = &info->members[i];
    if (is_member_primitive(memb)) {
      pos += handle_primitive(&js[pos], max - pos, memb);
      if (pos >= max) return max;
      continue;
    }
    if (memb->type == T_ENUM) {
      pos += snprintf(&js[pos], max - pos, "  this.%s = \"%s\";\n",
        memb->name, enum_to_str(memb->type_info.subenum_info.idx, 1));
      if (pos >= max) return -1;
      continue;
    }
    if (memb->type == T_ASTRUCT) {
      pos += js_proto_gen(&js[pos], max - pos,
       &meta_info[memb->type_info.anon_info.idx]);
      if (pos >= max) return max;
      continue;
    }
    if (memb->type == T_STRUCT) {
      len = strlen(memb->type_info.substruct_info.type_name);
      str = strstr(memb->type_info.substruct_info.type_name, "_info");
      if (str) len -= 5;
      if (memb->arr) {
        pos += snprintf(&js[pos], max - pos, "  this.%s = [",
          memb->name);
        if (pos >= max) return -1;
        for (j = 0; j < memb->arr; j++) {
          pos += snprintf(&js[pos], max - pos, "%.*s_init()",
           len, memb->type_info.substruct_info.type_name);
          if (pos >= max) return -1;
          if (j != (memb->arr - 1)) {
            pos += snprintf(&js[pos], max - pos, ", ");
            if (pos >= max) return -1;
          }
        }
        pos += snprintf(&js[pos], max - pos, "];\n");
        if (pos >= max) return -1;
      } else {
        capize(capname, sizeof(capname), memb->type_info.substruct_info.type_name, len);
        pos += snprintf(&js[pos], max - pos, "  this.%s = new %.*s();\n",
         memb->name, len, capname); 
        if (pos >= max) return -1;
      }
      continue;
    }
    if (memb->type == T_AUNION) {
      if (i == 0 || info->members[i-1].type != T_ENUM) continue;
      memset(&dummy, 0, sizeof(dummy));
      dummy.member_count = meta_info[memb->type_info.anon_info.idx].member_count;
      for (j = 0; j < dummy.member_count; j++) {
        dummy.members[j] = meta_info[memb->type_info.anon_info.idx].members[j];
        strcpy(dummy.members[j].name, enum_to_str(
          info->members[i-1].type_info.subenum_info.idx, j + 1));
      }
      pos += js_proto_gen(&js[pos], max - pos,
       &dummy);
      if (pos >= max) return max;
      continue;
    }
  }
  return pos;
}

static int kr_js_proto() {
  int i;
  char *js;
  char *str;
  size_t pos;
  size_t max;
  int len;
  char capname[128];
  max = MAX_JS_LEN - 1;
  pos = 0;
  js = calloc(1, MAX_JS_LEN);
  for (i = 0; i < KR_META_INFO_LAST; i++) {
    if (meta_info[i].type == ST_STRUCT) {
      len = strlen(meta_info[i].name);
      str = strstr(meta_info[i].name, "_info");
      if (str) len -= 5;
      capize(capname, sizeof(capname), meta_info[i].name, len);
      pos += snprintf(&js[pos], max - pos, "function %.*s() {\n",
       len, capname);
      if (pos >= max) return -1;
      pos += js_proto_gen(&js[pos], max - pos, &meta_info[i]);
      if (pos >= max) return -1;
      pos += snprintf(&js[pos], max - pos, "}\n\n");
      if (pos >= max) return -1;
    }
  }
  printf("%s", js);
  free(js);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = kr_js_proto();
  if (res < 0) fprintf(stderr, "Failure!\n");
  return res;
}
