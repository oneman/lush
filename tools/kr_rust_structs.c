#include <stdlib.h>
#include <krad/util/util.h>
#include <krad/app/debug.h>

static char *struct_type_to_str(int type) {
  switch (type) {
  case ST_ENUM:
    return "enum";
  case ST_STRUCT:
    return "enum";
  case ST_UNION:
    return "enum";
  case ST_ASTRUCT:
    return "enum";
  case ST_AUNION:
    return "enum";
  }
  return "fuckup";
}

static char *memb_type_to_str(int type) {
  switch (type) {
    case T_CHAR:
      return "str";
    case T_INT8:
      return "i8";
    case T_INT16:
      return "i16";
    case T_INT32:
      return "i32";
    case T_INT64:
      return "i64";
    case T_UINT8:
      return "u8";
    case T_UINT16:
      return "u16";
    case T_UINT32:
      return "u32";
    case T_UINT64:
      return "u64";
    case T_FLOAT:
      return "f32";
    case T_DOUBLE:
      return "f64";
    case T_STRUCT:
      return "struct";
    case T_UNION:
      return "union";
    case T_ENUM:
      return "lolskate";
      //return "enum";
    case T_AUNION:
      return "anon union";
    case T_ASTRUCT:
      return "anon struct";
    default: break;
  }
  return "unknown";
}


/*
static int cur_member = 0;
static int nmembers = 0;
static int nstructs = 0;

//static int skip = 0;
//static int sidx = 0;
static int in_enum = 0;
static int in_enum_mode = 0;

static int handle_member(const member_info *m, int depth) {
  //if (in_enum_mode && !in_enum) return 0;
  //printf("type is %d\n", m->type);
  if ((m->type != T_ENUM && depth > 2) && in_enum_mode) {
    return 0;
  }
  if (in_enum_mode && (m->type != 13 && m->type != 3)) return 0;
    
  if (in_enum_mode && m->type == 13) {
    printf("pub enum %s {\n", m->type_info.subenum_info.type_name);
    return 0;
    }
  //  printk("num mem %d\n", m->type_info.subenum_info.idx);//}
  if (!in_enum_mode && depth > 0) return 0;
  if (in_enum_mode && depth > 2) return 0;
  if (!in_enum_mode) cur_member++;
  printf("%*c", depth + 1, ' ');
  int skipcomma;
  skipcomma = 0;
  //printf("%s: %s    :::%d-%d", m->name, memb_type_to_str(m->type), cur_member, nmembers);
  if (m->type == T_ENUM  || m->type == T_AUNION) {
      if (!in_enum_mode) { 
        //printf("%s: %s", m->name, m->type_info.subenum_info.type_name);
        skipcomma = 1;
      } else {
        //printf("----%s", m->name);
      }
    if (in_enum && in_enum_mode) {
      //printf("-33---%s", m->name);
    } else {
      //if (!in_enum_mode) printf("%s: %s", m->name, m->type_info.subenum_info.type_name);
      if (in_enum_mode) {

      }
    }
  } else {
    if (!m->arr) {
      if (in_enum_mode) {
        if ((memcmp(m->name, "KR", 2) == 0) && depth == 1) { //printf("rrrrrr %d", m->type);
          printf("%s", m->name);
          cur_member++;
        }
      } else {
        printf("%s: %s", m->name, memb_type_to_str(m->type));
      }
    } else {
      if (!in_enum_mode) printf("%s: [%s; 64]", m->name, memb_type_to_str(m->type));
    }
  }
  if (cur_member == nmembers || (skipcomma && cur_member + 1 == nmembers)) {
    cur_member = 0;
    nmembers = 0;
    printf("\n");
    printf("}\n");
  } else {
    if (!in_enum_mode && !skipcomma) printf(",\n");
  }
  return 0;
}

static int handle_struct(const struct_info *s) {
  if (s->type == ST_ENUM) {
    in_enum = 1;
  } else {
    in_enum = 0;
  }
  //if ((s->type != ST_ENUM && s->type != ST_UNION) && in_enum_mode) {
  //if (s->type == ST_ENUM && !in_enum_mode) return 0;
  if (in_enum_mode && s->type == ST_STRUCT) nmembers = s->member_count;
  if (s->type != ST_ENUM && in_enum_mode) return 0;
    //printf("TYPE IS %d\n", s->type);
 //    return 0;
  //}
  //  printf(" X TYPE IS %d\n", s->type);
  if (nmembers == 0 && nstructs > 0) {
    printf("\n");
  }
  //skip = 0;
  cur_member = 0;
  if (!in_enum_mode) nmembers = s->member_count;
 //  printf("mems is %d\n", nmembers);
  if (s->type == ST_ENUM) { 
    //in_enum = 1;
  } else {
    //in_enum = 0;
  }
  nstructs++;
  *//*if (!in_enum_mode && nstructs > 9) { 
    printf("\n");
    printf("fn main() {}");
    exit(0);
  }*//*
  printf("#[allow(dead_code)]\n");
  printf("#[allow(non_camel_case_types)]\n");
  //if (in_enum_mode && in_enum) {
  if (in_enum_mode) {
    printf("pub enum %s {\n", s->name);
  } else {
    printf("pub struct %s {\n", s->name);
  }
  //printk("members %d", s->member_count);
  return 0;
}
*/

static int depth = 0;
static int nseen = 0;
static int members[10];
static int in_enum = 0;
static int in_aunion = 0;

char *fix_name(const char *name, char *str, size_t sz, struct_type type) {
  int len;
  int sub;
  char *addon;
  addon = "";
  sub = 0;
  len = strlen(name);
  if (len < 5) {
    sub = 0;
  } else if (memcmp(name + (len - 4), "sub", 3) == 0) {
    sub = 5;
  }
  //printk("sub is %d", sub);
  memset(str, 0, sz);
  if (type == ST_ENUM) {
    //addon = "_oldy";
    //sub += 5;
  }
  //if (type == ST_AUNION || type == ST_STRUCT || ST_ASTRUCT) {
  if (type == ST_AUNION) {
    addon = "_info";
    sub += 5;
  }
  snprintf(str, sz, "%.*s%s", len - sub, name, addon);
  return str;
}

static int handle_struct(const struct_info *s, int start) {
  const char *name;
  char rename[64];
  if (start) {
    if (nseen > 0) printf("\n");
    nseen++;
    depth++;
    if (s->type == ST_AUNION) in_aunion = 1;
    if (s->type == ST_ENUM) in_enum = 1;
    name = fix_name(s->name, rename, sizeof(rename), s->type);
    printf("pub %s %s {\n", struct_type_to_str(s->type), name);
  } else {
    printf("\n}\n"); 
    members[depth] = 0;
    depth--;
    if (s->type == ST_AUNION) in_aunion = 0;
    if (s->type == ST_ENUM) in_enum = 0;
  }
  return 0;
}

const char *filter_name(const char *name) {
  if (strcmp(name, "type") == 0) return "kr_type";
  if (strcmp(name, "in") == 0) return "kr_in";
  return name;
}

static int handle_member(const member_info *m) {
  const char *name;
  int i;
  i = 0;
  if (m->type == T_AUNION) return 0;
  if (members[depth]) printf(",\n");
  members[depth]++;
  while (i++ != depth) printf("  ");
  name = filter_name(m->name);
  if (m->type == T_STRUCT) {
    //printk("type is %d", m->type);
    if (in_aunion) {
      printf("%s (%s)", name, m->type_info.subenum_info.type_name);
    } else {
      printf("%s (%s)", name, m->type_info.substruct_info.type_name);
    } 
  } else {
    if (m->arr) {
      if (in_aunion) {
        printf("%s ([%s; 64])", name, memb_type_to_str(m->type));
      } else {
        printf("%s ([%s; 64])", name, memb_type_to_str(m->type));
      }
    } else if (in_enum) {
      printf("%s", name);
    } else if (in_aunion || m->type == T_UNION || m->type == T_AUNION) {
      printf("%s (%s)", name, m->type_info.subenum_info.type_name);
    } else if (m->type == T_ENUM) {
      printf("%s (%s)", name, m->type_info.subenum_info.type_name);
    } else {
      printf("%s (%s)", name, memb_type_to_str(m->type));
    }
  }
  return 0;
}

static int gen_header() {
  int ret;
  /*printf("pub enum lolskate {\n");
  printf("  Bonehead,\n");
  printf("  Frogleg\n");
  printf("}\n\n");*/
  ret = meta_loop(handle_struct, handle_member);
  printf("\n");
  printf("fn main() {}");
  exit(0);
  return ret;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = gen_header();
  if (ret != 0) printke("Failure!");
  return ret;
}
