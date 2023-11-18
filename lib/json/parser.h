#ifndef KRAD_JSON_PARSER_H
#define KRAD_JSON_PARSER_H

#include <inttypes.h>
#include <string.h>
#include <stddef.h>

#define MAX_JPS_TOKENS 2048

typedef struct kr_jps kr_jps;

typedef enum {
  JS_NUMBER,
  JS_OBJECT,
  JS_ARRAY,
  JS_STRING
} js_tk_type;

size_t kr_jps_sizeof();
#define kr_jps_alloca(parser) \
  parser = alloca(kr_jps_sizeof()); \
  memset(parser, 0, kr_jps_sizeof());
int jps_init(kr_jps *jpstate);
int jps_curr_size(kr_jps *jpstate);
int jps_curr_len(kr_jps *jpstate);
int jps_curr(kr_jps *jpstate);
char *jps_curr_start(kr_jps *jpstate);
char *jps_curr_end(kr_jps *jpstate);
int jps_curr_type(kr_jps *jpstate);
int jps_parse(kr_jps *jpstate, char *json);
int jps_next(kr_jps *jpstate);
int jps_prev(kr_jps *jpstate);
int jps_at(kr_jps *jpstate, int tk);
int jps_rewind(kr_jps *jpstate);
int jps_rewind_at(kr_jps *jpstate, int tk);
int jps_rewind_parent(kr_jps *jpstate);
int jps_curr_depth(kr_jps *jpstate);
int jps_curr_parent(kr_jps *jpstate);
int jps_token_count(kr_jps *jpstate);
int jps_array_at(kr_jps *jpstate, int index);
int jps_next_key_in_curr_obj(kr_jps *jpstate);
char *jps_type_to_str(js_tk_type type);
int jps_find_key(kr_jps *jps, char *name);

#endif
