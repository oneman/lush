#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <krad/json/parser.h>

static int pretty_json_rec(kr_jps *jps);
static int handle_array(kr_jps *jps);

static void indent(int n) {
  int i;
  for (i = 0; i < n; i++) {
    printf("  ");
  }
}

static int jps_next_type(kr_jps *jps) {
  int type;
  jps_next(jps);
  type = jps_curr_type(jps);
  jps_prev(jps);
  return type;
}

static int handle_array(kr_jps *jps) {
  int res;
  int i;
  int sz;
  int pos;
  int pos2;
  sz = jps_curr_size(jps);
  pos = jps_curr(jps);
  for (i = 0; i < sz; i++) {
    jps_array_at(jps, i);
    indent(jps_curr_depth(jps));
    if (jps_curr_type(jps) == JS_STRING) {
      printf("\"%s\"", jps_curr_start(jps));
    } else if (jps_curr_type(jps) == JS_NUMBER) {
      printf("%g", atof(jps_curr_start(jps)));
    } else if (jps_curr_type(jps) == JS_ARRAY) {
      printf("[\n");
      pos2 = jps_curr(jps);
      res = handle_array(jps);
      if (res < 0) return -1;
      jps_at(jps, pos2);
      indent(jps_curr_depth(jps));
      printf("]");
    } else if (jps_curr_type(jps) == JS_OBJECT) {
      printf("{\n");
      pos2 = jps_curr(jps);
      res = pretty_json_rec(jps);
      if (res < 0) return -1;
      jps_at(jps, pos2);
      indent(jps_curr_depth(jps));
      printf("}");
    } else return -1;
    if (i != (sz - 1)) printf(",");
    printf("\n");
    jps_at(jps, pos);
  }
  return 0;
}

static int pretty_json_rec(kr_jps *jps) {
  int res;
  int pos;
  int obj_start;
  obj_start = jps_curr(jps);
  if (jps_next(jps) != 0) return -1;
  if (jps_curr_type(jps) != JS_STRING) return -1;
  for (;;) {
    indent(jps_curr_depth(jps));
    printf("\"%s\": ", jps_curr_start(jps));
    if (jps_next_type(jps) == JS_STRING) {
      jps_next(jps);
      printf("\"%s\"", jps_curr_start(jps));
      jps_prev(jps);
    } else if (jps_next_type(jps) == JS_NUMBER) {
      jps_next(jps);
      printf("%s", jps_curr_start(jps));
      jps_prev(jps);
    } else if (jps_next_type(jps) == JS_ARRAY) {
      pos = jps_curr(jps);
      printf("[\n");
      jps_next(jps);
      res = handle_array(jps);
      if (res < 0) return -1;
      jps_at(jps, pos);
      indent(jps_curr_depth(jps));
      printf("]");
    } else if (jps_next_type(jps) == JS_OBJECT) {
      pos = jps_curr(jps);
      jps_next(jps);
      printf("{\n");
      res = pretty_json_rec(jps);
      if (res < 0) return -1;
      jps_at(jps, pos);
      indent(jps_curr_depth(jps));
      printf("}");
    } else return -1;
    pos = jps_curr(jps);
    if (jps_next_key_in_curr_obj(jps) != 0) {
      printf("\n");
      break;
    }
    printf(",\n");
  }
  jps_at(jps, obj_start);
  return 0;
}

int pretty_json(char *json) {
  int res;
  kr_jps *jps;
  if (json == NULL) return -1;
  kr_jps_alloca(jps);
  jps_init(jps);
  res = jps_parse(jps, json);
  if (res != 0) return -1;
  if (jps_curr_type(jps) != JS_OBJECT) return -1;
  printf("{\n");
  res = pretty_json_rec(jps);
  printf("}\n");
  return res;
}
