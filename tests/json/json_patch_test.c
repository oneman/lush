#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#include "../lib/gen/jsmn/jsmn.h"
#include "../lib/gen/jsmn/jsmn.c"

#include "../lib/app/path.h"
#include "../lib/app/path.c"

#define KR_PATCH_DEPTH_MAX 8
#define KR_PATCHSET_PATCHES_MAX 128

#define KR_JSON_TOKR_MAX 512
typedef jsmntok_t kr_jtok;

typedef struct {
  jsmntok_t tokens[KR_JSON_TOKR_MAX];
  int cur;
  int ret;
  int sz;
  char *json;
  int json_len;
} kr_jtokr;

typedef struct kr_patch kr_patch;

struct kr_patch {
  int coconut;
};

typedef struct kr_patchset kr_patchset;

struct kr_patchset {
  kr_patch patches[KR_PATCHSET_PATCHES_MAX];
  int used;
};


int kr_jtokr_init(kr_jtokr *tokr) {
  if (tokr == NULL) return -1;
  memset(tokr, 0, sizeof(*tokr));
  tokr->sz = KR_JSON_TOKR_MAX;
  return tokr->sz;
}

kr_jtok *kr_jtokr_iter(kr_jtokr *tokr) {
  if (tokr == NULL) return NULL;
  if (tokr->cur < tokr->ret) {
    return &tokr->tokens[tokr->cur++];
  }
  return NULL;
}

kr_jtok *kr_jtokr_iter_limit(kr_jtokr *tokr, int limit) {
  if (tokr == NULL) return NULL;
  if (tokr->cur < tokr->ret) {
    if (tokr->tokens[tokr->cur].end <= limit) {
      return &tokr->tokens[tokr->cur++];
    }
  }
  return NULL;
}

int kr_jtokr_tseek(kr_jtokr *tokr, int pos) {
  if (tokr == NULL) return -1;
  if (pos < 0) return -1;
  if (pos < tokr->ret) {
    tokr->cur = pos;
    return 0;
  }
  return -1;
}

int kr_jtokr_reltseek(kr_jtokr *tokr, int pos) {
  if (tokr == NULL) return -1;
  pos += tokr->cur;
  return kr_jtokr_tseek(tokr, pos);
}

kr_jtok *kr_jtokr_overseek(kr_jtokr *tokr, int pos) {
  kr_jtok *tok;
  if (tokr == NULL) return NULL;
  if (pos < 0) return NULL;
  while ((tok = kr_jtokr_iter(tokr))) {
    if (tok->start > pos ) {
      /*kr_jtokr_reltseek(tokr, -1);*/
      return tok;
    }
  }
  return NULL;
}

int kr_jtokr_rewind(kr_jtokr *tokr) {
  if (tokr == NULL) return -1;
  kr_jtokr_tseek(tokr, 0);
  return 0;
}

kr_jtokr *kr_json_parse(kr_jtokr *tokr, char *json, int len) {
  jsmn_parser parser;
  jsmnerr_t err;
  if ((tokr == NULL) || (json == NULL) || (len < 1)) return NULL;
  jsmn_init(&parser);
  kr_jtokr_init(tokr);
  err = jsmn_parse(&parser, json, tokr->tokens, tokr->sz);
  tokr->ret = parser.toknext;
  tokr->json = json;
  tokr->json_len = len;
  if (err != JSMN_SUCCESS || tokr->ret < 3) {
    return NULL;
  }
  return tokr;
}

int kr_jtokstrcmp(kr_jtokr *tokr, kr_jtok *tok, char *str, int len) {
 int strsz;
 strsz = tok->end - tok->start;
 //printf("we got len %d and %d\n", strsz, len);
 if (len != strsz) return -1;
 return memcmp(str, &tokr->json[tok->start], len);
}

static void path_print(kr_path *path) {
  char *name;
  int len;
  int ret;
  name = NULL;
  if (path == NULL) {
    fprintf(stderr, "path_print called with null kr_path\n");
    return;
  }
  printf("%d step path :: ", kr_path_steps(path));
  for (;;) {
    len = kr_path_cur_name(path, &name);
    /*printf("/ (%d) %.*s ", len, len, name);*/
    printf("/ %.*s ", len, name);
    ret = kr_path_step(path);
    if (ret != 0) break;
  }
  printf("\n");
  /*len = kr_path_last(path, &name);
  printf("Last is (%d chars) %s\n", len, name);*/
}

int handle_json_partial(kr_patchset *patchset, kr_path *path, kr_jtokr *tokr, int limit) {
  kr_jtok *tok;
  int ret;
  int parents[10];
  int parent;
  int newparent;
  int p;
  int c;
  union {
    int integer;
    float real;
  } value;
  newparent = 0;
  parent = 0;
  memset(&parents, 0, sizeof(parents));
  ret = 0;
  while ((tok = kr_jtokr_iter_limit(tokr, limit))) {
    if (patchset->used == KR_PATCHSET_PATCHES_MAX) {
      /* damn this would be attacker to fail */
      printf("Too many patches\n");
      return -1;
    }
    if (newparent) {
      if (parents[parent]) {
        parents[parent]--;
      }
      if (parents[parent]) {
        parent++;
      }
      parents[parent] = tok->size;
      newparent = 0;
    } else {
      if (parent > 0) {
      if (parents[parent] == 0) {
        kr_path_clear_last(path);
        parent--;
      if (parents[parent]) {
        parents[parent]--;
      }
      }
      }
    }
    switch (tok->type) {
      case JSMN_OBJECT:
        //printf("Token %d an object\n", k + 1);
        break;
      case JSMN_ARRAY:
        //printf("Token %d an array\n", k + 1);
        break;
      case JSMN_STRING:
        //printf("Token %d a string\n", k + 1);
        if (kr_path_steps_free(path) == 0) {
          /* no sploiting, disconnect this jerk */
          printf("Too many paths\n");
          return -1;
        }
        kr_path_push(path, &tokr->json[tok->start], tok->end - tok->start);
        newparent++;
        if (0) {
          path_print(path);
          kr_path_rewind(path);
        }
        break;
      case JSMN_PRIMITIVE:
        //printf("Token %d we got us a primitive!\n", k + 1);
        if ((tok->end - tok->start) > 6) {
          printf("Numbers to dang long, we ain't dealing with that\n");
          return -1;
        }
        p = 0;
        for (c = tok->start; c < tok->end; c++) {
          if ((!isdigit(tokr->json[c])) && (tokr->json[c] != '.')) {
            /* rage force disconnect the fool who sends a non number */
            printf("Got a primitive that isn't a number, ragequiting\n");
            return -1;
          }
          if (tokr->json[c] == '.') {
           if (p == 1) {
             printf("Too many points in your floating bullshit\n");
             return -1;
           } else {
             p = 1;
           }
          }
        }
        if (1) {
          path_print(path);
          kr_path_rewind(path);
        }
        if (p == 1) {
          value.real = atof(&tokr->json[tok->start]);
          printf("We got a %f floater wa doo!\n", value.real);
        }
        if (p == 0) {
          value.integer = atoi(&tokr->json[tok->start]);
          printf("We got a %d integer wee hoo!\n", value.integer);
        }
        //ret = kr_strarr_to_patch(&patchset->patches[patchset->used], value, path, path_len);
        if (ret != 0) {
          /* damn this patch is invalid as hell */
          return -1;
        }
        patchset->used++;
       // kr_path_clear_last(path);
        break;
      default:
        printf("O dear major jsmn messup!\n");
        return -1;
        break;
    }
  }
  printf("Done parsing json patchset, we got %d patches\n\n", patchset->used);
  return patchset->used;
}

int handle_json_patch_obj(kr_patchset *patchset, kr_path *path, kr_jtokr *tokr) {
  int or;
  int ret;
  int oseek;
  char *patch_path;
  char *name;
  int ret2;
  int len2;
  char *value;
  int vstart;
  int vlimit;
  int vret;
  int patch_path_len;
  //int vlen;
  kr_jtok *tok;
  if (tokr == NULL) return -1;
  oseek = 0;
  or = 0;
  patch_path_len = 0;
  vstart = 0;
  vlimit = 0;
  vret = 0;
  //vlen = 0;
  patch_path = NULL;
  value = NULL;
  name = NULL;
  tok = kr_jtokr_iter(tokr);
  if (tok == NULL) return -3;
  if (tok->type != JSMN_OBJECT) return -1;
  printf("patch obj size %d\n", tok->size);
  if (tok->size != 6) return -2;
  for (;;) {
    if ((or == 1) && (patch_path != NULL) && (value != NULL)) {
      printf("winner!\n");
      vret = tokr->cur;
      kr_jtokr_tseek(tokr, vstart);
  kr_path *path2;
  kr_path_alloca(path2);
  kr_path_parse(path, patch_path, patch_path_len);
  for (;;) {
    len2 = kr_path_cur_name(path2, &name);
    /*printf("/ (%d) %.*s ", len, len, name);*/
//    printf("/ %.*s ", len, name);
    kr_path_push(path, name, len2);
    ret2 = kr_path_step(path2);
    if (ret2 != 0) break;
  }
  printf("\n");

      ret = handle_json_partial(patchset, path, tokr, vlimit);
while (kr_path_steps(path2)) {
  kr_path_clear_last(path);
  kr_path_clear_last(path2);
}

      printf("handle json partial ret %d\n", ret);
      kr_jtokr_tseek(tokr, vret);
      return 0;
    }
    if (oseek != 0) {
      tok = kr_jtokr_overseek(tokr, oseek);
      oseek = 0;
    } else {
      tok = kr_jtokr_iter(tokr);
    }
    switch (tok->type) {
      case JSMN_STRING:
        if ((or == 0) && (kr_jtokstrcmp(tokr, tok, "op", 2) == 0)) {
          tok = kr_jtokr_iter(tokr);
          if (kr_jtokstrcmp(tokr, tok, "replace", 7) == 0) {
            //printf("we got op replace!!\n");
            or = 1;
            continue;
          } else {
            printf("not a op replace\n");
            return -5;
          }
        }
        if ((patch_path == NULL) && (kr_jtokstrcmp(tokr, tok, "path", 4) == 0)) {
          tok = kr_jtokr_iter(tokr);
          //printf("we got path\n");
          patch_path = &tokr->json[tok->start];
          patch_path_len = tok->end - tok->start;
          continue;
        }
        if ((value == NULL) && (kr_jtokstrcmp(tokr, tok, "value", 5) == 0)) {
          tok = kr_jtokr_iter(tokr);
          //printf("we got value\n");
          value = &tokr->json[tok->start];
          printf("we have %d babies\n", tok->size);
          vstart = tokr->cur;
          vlimit = tok->end;
          if ((or != 1) || (patch_path == NULL)) {
            oseek = tok->end;
            //printf("will seek %d\n", oseek);
          }
          continue;
        }
        /* Falling thru if we catch nothing */
      default:
        printf("we went wrong.. bailing..\n");
        return -1;
    }
  }
  return -4;
}


int json_to_patchset(kr_path *path, char *json, int json_len, kr_patchset *patchset) {
  int ret;
  kr_jtokr tokr;
  kr_jtok *tok;
  if ((path == NULL) || (json == NULL) || (json_len < 1) || (patchset == NULL)) {
    return -1;
  }
  printf("Going to parse:\n%s\n", json);
  if (0) {
    path_print(path);
    kr_path_rewind(path);
  }
  ret = 0;
  patchset->used = 0;
  kr_json_parse(&tokr, json, json_len);
  tok = kr_jtokr_iter(&tokr);
  switch (tok->type) {
    case JSMN_OBJECT:
      printf("Parsing as JSON partial update\n");
      ret = handle_json_partial(patchset, path, &tokr, json_len);
      printf("handle json partial ret %d\n", ret);
      break;
    case JSMN_ARRAY:
      printf("Parsing as JSON PATCH Array\n");
      while (ret == 0) {
        ret = handle_json_patch_obj(patchset, path, &tokr);
        printf("handle json patch obj ret %d\n", ret);
      }
      break;
    default:
      printf("First we went wrong.. bailing..\n");
      return -1;
  }
  return patchset->used;
}

static int test(char *json_in) {
  int ret;
  kr_path *path;
  char *url;
  int url_len;
  char *json;
  int json_len;
  kr_patchset patchset;
  ret = -1;
  memset(&patchset, 0, sizeof(patchset));
  kr_path_alloca(path);
  url = "/mixer/music1";
  url_len = strlen(url);
  kr_path_parse(path, url, url_len);
  if (json_in != NULL) {
    json = json_in;
    json_len = strlen(json);
    ret = json_to_patchset(path, json, json_len, &patchset);
  } else {
    json = "[{\"bongo\":\"coconut\"}]";
    json_len = strlen(json);
    json_to_patchset(path, json, json_len, &patchset);
    json = "{\"analog\":{\"drive\":5}}";
    json_len = strlen(json);
    kr_path_parse(path, url, url_len);
    ret = json_to_patchset(path, json, json_len, &patchset);
  }
  if (ret > 0) return 0;
  return ret;
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc == 2) {
    ret = test(argv[1]);
  } else {
    ret = test(NULL);
  }
  if (ret) {
    fprintf(stderr, "Fail.\n");
  } else {
    printf("It worked!\n");
  }
  return ret;
}
