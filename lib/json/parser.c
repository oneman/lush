#include <stdio.h>
#include <string.h>
#include "jsmn/jsmn.h"
#include "parser.h"

struct kr_jps {
  jsmn_parser parser;
  jsmntok_t tokens[MAX_JPS_TOKENS];
  int tk_count;
  char *json;
  char end[MAX_JPS_TOKENS];
  int curr;
};

static int jps_null_curr_end(kr_jps *jpstate);
static int jps_reset_curr_end(kr_jps *jpstate);

static int jps_null_curr_end(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  jpstate->end[jpstate->curr] = jpstate->json[jpstate->tokens[jpstate->curr].end];
  jpstate->json[jpstate->tokens[jpstate->curr].end] = '\0';
  return 0;
}

static int jps_reset_curr_end(kr_jps *jpstate) {
  char *end;
  if (jpstate == NULL) return -1;
  end = &jpstate->json[jpstate->tokens[jpstate->curr].end];
  if (*end == '\0') {
    *end = jpstate->end[jpstate->curr];
  }
  return 0;
}

size_t kr_jps_sizeof() {
  return sizeof(kr_jps);
}

int jps_init(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  memset(jpstate, 0, sizeof(kr_jps));
  jsmn_init(&jpstate->parser);
  return 0;
}

int jps_curr_size(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  return jpstate->tokens[jpstate->curr].size;
}

int jps_curr_len(kr_jps *jpstate) {
  int curr;
  if (jpstate == NULL) return -1;
  curr = jpstate->curr;
  return jpstate->tokens[curr].end - jpstate->tokens[curr].start;
}

char *jps_curr_start(kr_jps *jpstate) {
  if (jpstate == NULL) return NULL;
  return &jpstate->json[jpstate->tokens[jpstate->curr].start];
}

char *jps_curr_end(kr_jps *jpstate) {
  if (jpstate == NULL) return NULL;
  return &jpstate->json[jpstate->tokens[jpstate->curr].end];
}

int jps_curr_type(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  return jpstate->tokens[jpstate->curr].type;
}

int jps_parse(kr_jps *jpstate, char *json) {
  jsmnerr_t err;
  int n;
  if (jpstate == NULL || json == NULL) return -1;
  err = jsmn_parse(&jpstate->parser, json, jpstate->tokens, MAX_JPS_TOKENS);
  n = jpstate->parser.toknext;
  if (err != JSMN_SUCCESS) return err;
  jpstate->tk_count = n;
  jpstate->json = json;
  jps_null_curr_end(jpstate);
  return 0;
}

int jps_next(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  if (jpstate->curr == (jpstate->tk_count - 1) ) return -1;
  jpstate->curr++;
  jps_null_curr_end(jpstate);
  return 0;
}

int jps_prev(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  if (jpstate->curr == 0) return -1;
  jps_reset_curr_end(jpstate);
  jpstate->curr--;
  return 0;
}

int jps_rewind_at(kr_jps *jpstate, int tk) {
  int i;
  if (jpstate == NULL) return -1;
  for (i = jpstate->curr - 1; i >= tk; i--) {
    jps_prev(jpstate);
  }
  return 0;
}

int jps_at(kr_jps *jpstate, int tk) {
  int i;
  if (jpstate == NULL) return -1;
  if (jpstate->curr > tk) return jps_rewind_at(jpstate, tk);
  else {
    for (i = 0; i < (tk - jpstate->curr); i++) {
      jps_next(jpstate);
    }
  }
  return 0;
}

int jps_rewind(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  jps_rewind_at(jpstate, 0);
  return 0;
}

int jps_rewind_parent(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  jps_rewind_at(jpstate, jps_curr_parent(jpstate));
  jps_next(jpstate);
  return 0;
}

int jps_curr_depth(kr_jps *jpstate) {
  int curr;
  if (jpstate == NULL) return -1;
  curr = jpstate->curr;
  return jpstate->tokens[curr].depth;
}

int jps_curr_parent(kr_jps *jpstate) {
  int curr;
  if (jpstate == NULL) return -1;
  curr = jpstate->curr;
  return jpstate->tokens[curr].parent;
}

int jps_token_count(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  return jpstate->tk_count;
}

int jps_curr(kr_jps *jpstate) {
  if (jpstate == NULL) return -1;
  return jpstate->curr;
}

int jps_array_at(kr_jps *jpstate, int index) {
  kr_jps aux;
  int size;
  int pos;
  int i;
  int j;
  char *start;
  if (jpstate == NULL) return -1;
  if (jps_curr_type(jpstate) != JS_ARRAY) return -1;
  size = jps_curr_size(jpstate);
  if (index < 0) return -1;
  if (index >= size) return -1;
  jps_next(jpstate);
  if (index == 0) return 0;
  for (i = 1, pos = 0; i < size; i++) {
    jps_init(&aux);
    start = jps_curr_start(jpstate);
    if (jps_parse(&aux, start) != 0) return -1;
    pos = jps_token_count(&aux);
    for (j = 0; j < pos; j++) {
      jps_next(jpstate);
    }
    if (i == index) break;
  }
  return 0;
}

int jps_next_key_in_curr_obj(kr_jps *jpstate) {
  kr_jps aux;
  char *start;
  int i;
  int size;
  int tkc;
  int parent;
  if (jpstate == NULL) return -1;
  if (jps_curr_type(jpstate) != JS_STRING) return -1;
  parent = jps_curr_parent(jpstate);
  jps_next(jpstate);
  size = jps_curr_size(jpstate);
  if (size == 0) {
    if (jps_next(jpstate) == 0) {
      if (parent == jps_curr_parent(jpstate)) {
        return 0;
      }
      else {
        jps_prev(jpstate);
        return -1;
      }
    }
    else {
      return -1;
    }
  } else {
    jps_init(&aux);
    start = jps_curr_start(jpstate);
    if (jps_parse(&aux, start) != 0) return -1;
    tkc = jps_token_count(&aux);
    for (i = 0; i < tkc; i++) {
      if (jps_next(jpstate) < 0) return -1;
    }
    if (parent != jps_curr_parent(jpstate)) return -1;
    jps_rewind(&aux);
    return 0;
  }
  return -1;
}

char *jps_type_to_str(js_tk_type type) {
  switch (type) {
    case JS_NUMBER:
      return "number";
    case JS_OBJECT:
      return "object";
    case JS_ARRAY:
      return "array";
    case JS_STRING:
      return "string";
    default:
      return "Unknown";
  }
}

int jps_find_key(kr_jps *jps, char *name) {
  jps_rewind_parent(jps);
  do {
    if (jps_curr_type(jps) == JS_STRING) {
      if (!strcmp(jps_curr_start(jps), name)){
        if (jps_next(jps) < 0) return -1;
        return 0;
      }
    }
  } while (jps_next_key_in_curr_obj(jps) == 0);
  printf("JSON Error: Missing %s!\n", name);
  jps_rewind(jps);
  return -1;
}
