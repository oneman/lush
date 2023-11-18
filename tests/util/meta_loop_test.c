#include <krad/util/util.h>
#include <krad/app/debug.h>
/*
static char *memb_type_to_str(int type) {
  switch (type) {
    case T_CHAR:
      return "char";
    case T_INT8:
      return "int8_t";
    case T_INT16:
      return "int16_t";
    case T_INT32:
      return "int32_t";
    case T_INT64:
      return "int64_t";
    case T_UINT8:
      return "uint8_t";
    case T_UINT16:
      return "uint16_t";
    case T_UINT32:
      return "uint32_t";
    case T_UINT64:
      return "uint64_t";
    case T_FLOAT:
      return "float";
    case T_DOUBLE:
      return "float";
    case T_STRUCT:
      return "struct";
    case T_UNION:
      return "union";
    case T_ENUM:
      return "enum";
    case T_AUNION:
      return "anon union";
    case T_ASTRUCT:
      return "anon struct";
    default: break;
  }
  return "unknown";
}

static int handle_member(const member_info *m, int depth) {
  printf("%*c", depth + 1, ' ');
  printf("%s %s", memb_type_to_str(m->type), m->name);
  if (m->arr) {
    printf("[%d]", m->arr);
  }
  printk("");
  return 0;
}

static int handle_struct(const struct_info *s) {
  printk("\n\nhandling %s", s->name);
  return 0;
}
*/
static int meta_loop_test() {
  int ret;
  ret = 0;
  //ret = meta_loop(handle_struct, handle_member);
  return ret;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = meta_loop_test();
  if (ret != 0) printke("Failure!");
  return ret;
}
