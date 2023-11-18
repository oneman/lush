#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/playlist/m3u.h>
#include "m3u.h"

#define MAX_M3U_LINES 64

typedef enum {
  URI = 1,
  TAG
} m3u_line_type;

typedef struct {
  char *start;
  size_t sz;
} m3u_token;

typedef struct {
  char *buffer;
  size_t sz;
  size_t pos;
  kr_m3u_info *info;
} m3u_parser;

static const m3u_token tag_names[] = {
  [EXTM3U] = {.start = "EXTM3U", .sz = 6},
  [EXTINF] = {.start = "EXTINF", .sz = 6},
  [EXT_X_BYTERANGE] = {.start = "EXT-X-BYTERANGE", .sz = 15},
  [EXT_X_TARGETDURATION] = {.start = "EXT-X-TARGETDURATION", .sz = 20},
  [EXT_X_MEDIA_SEQUENCE] = {.start = "EXT-X-MEDIA-SEQUENCE", .sz = 20},
  [EXT_X_KEY] = {.start = "EXT-X-KEY", .sz = 9},
  [EXT_X_PROGRAM_DATE_TIME] = {.start = "EXT-X-PROGRAM-DATE-TIME", .sz = 23},
  [EXT_X_ALLOW_CACHE] = {.start = "EXT-X-ALLOW-CACHE", .sz = 17},
  [EXT_X_PLAYLIST_TYPE] = {.start = "EXT-X-PLAYLIST-TYPE", .sz = 19},
  [EXT_X_STREAM_INF] = {.start = "EXT-X-STREAM-INF", .sz = 16},
  [EXT_X_I_FRAME_STREAM_INF] = {.start = "EXT-X-I-FRAME-STREAM-INF", .sz = 24},
  [EXT_X_I_FRAMES_ONLY] = {.start = "EXT-X-I-FRAMES-ONLY", .sz = 19},
  [EXT_X_MAP] = {.start = "EXT-X-MAP", .sz = 9},
  [EXT_X_MEDIA] = {.start = "EXT-X-MEDIA", .sz = 11},
  [EXT_X_ENDLIST] = {.start = "EXT-X-ENDLIST", .sz = 13},
  [EXT_X_DISCONTINUITY] = {.start = "EXT-X-DISCONTINUITY", .sz = 19},
  [EXT_X_DISCONTINUITY_SEQUENCE] = {.start = "EXT-X-DISCONTINUITY-SEQUENCE",
   .sz = 28},
  [EXT_X_INDEPENDENT_SEGMENTS] = {.start = "EXT-X-INDEPENDENT-SEGMENTS",
   .sz = 26},
  [EXT_X_START] = {.start = "EXT-X-START", .sz = 11},
  [EXT_X_VERSION] = {.start = "EXT-X-VERSION", .sz = 13}
};

static const m3u_token attr_names[] = {
  [M3U_ATTR_METHOD] = {.start = "METHOD", .sz = 6},
  [M3U_ATTR_URI] = {.start = "URI", .sz = 3},
  [M3U_ATTR_IV] = {.start = "IV", .sz = 2},
  [M3U_ATTR_KEYFORMAT] = {.start = "KEYFORMAT", .sz = 9},
  [M3U_ATTR_KEYFORMATVERSIONS] = {.start = "KEYFORMATVERSIONS", .sz = 17},
  [M3U_ATTR_TYPE] = {.start = "TYPE", .sz = 4},
  [M3U_ATTR_GROUP_ID] = {.start = "GROUP-ID", .sz = 8},
  [M3U_ATTR_PROGRAM_ID] = {.start = "PROGRAM-ID", .sz = 10},
  [M3U_ATTR_LANGUAGE] = {.start = "LANGUAGE", .sz = 8},
  [M3U_ATTR_ASSOC_LANGUAGE] = {.start = "ASSOC-LANGUAGE", .sz = 14},
  [M3U_ATTR_NAME] = {.start = "NAME", .sz = 4},
  [M3U_ATTR_DEFAULT] = {.start = "DEFAULT", .sz = 7},
  [M3U_ATTR_AUTOSELECT] = {.start = "AUTOSELECT", .sz = 10},
  [M3U_ATTR_FORCED] = {.start = "FORCED", .sz = 6},
  [M3U_ATTR_INSTREAM_ID] = {.start = "INSTREAM-ID", .sz = 11},
  [M3U_ATTR_CHARACTERISTICS] = {.start = "CHARACTERISTICS", .sz = 15},
  [M3U_ATTR_BANDWIDTH] = {.start = "BANDWIDTH", .sz = 9},
  [M3U_ATTR_CODECS] = {.start = "CODECS", .sz = 6},
  [M3U_ATTR_RESOLUTION] = {.start = "RESOLUTION", .sz = 10},
  [M3U_ATTR_AUDIO] = {.start = "AUDIO", .sz = 5},
  [M3U_ATTR_VIDEO] = {.start = "VIDEO", .sz = 5},
  [M3U_ATTR_SUBTITLES] = {.start = "SUBTITLES", .sz = 9},
  [M3U_ATTR_CLOSED_CAPTIONS] = {.start = "CLOSED-CAPTIONS", .sz = 15},
  [M3U_ATTR_BYTERANGE] = {.start = "BYTERANGE", .sz = 9},
  [M3U_ATTR_TIME_OFFEST] = {.start = "TIME-OFFSET", .sz = 11},
  [M3U_ATTR_PRECISE] = {.start = "PRECISE", .sz = 7}
};

static void get_to_nl(m3u_parser *psr) {
  for (; psr->pos < psr->sz; psr->pos++) {
    if (psr->buffer[psr->pos] == '\n') {
      return;
    }
  }
}

static int m3u_tag_name_parse(m3u_el_info *info, m3u_token *tag) {
  int i;
  char *name;
  size_t sz;
  name = tag->start + 4;
  sz = tag->sz - 4;
  for (i = 1; i <= EXT_X_VERSION; i++) {
    if ( (sz == (tag_names[i].sz - 3) ) &&
     !memcmp(name, tag_names[i].start + 3, sz)) {
      info->tag.name = i;
    }
  }
  return 0;
}

static int m3u_tag_attr_name_parse(m3u_tag_info *info, m3u_token *name) {
  int i;
  for (i = 1; i <= M3U_ATTR_PRECISE; i++) {
    if ( (name->sz == attr_names[i].sz) &&
     !memcmp(name->start, attr_names[i].start, name->sz)) {
      info->alist[info->count].name = i;
      return 0;
    }
  }
  return -1;
}

static int m3u_tag_val_alist_parse(m3u_parser *psr) {
  int k;
  uint8_t reading_val;
  uint8_t string_val;
  m3u_token tok;
  m3u_tag_info *info;
  info = &psr->info->elements[psr->info->el_count].tag;
  psr->pos++;
  tok.start = psr->buffer + psr->pos;
  reading_val = 0;
  string_val = 0;
  for (k = 0; psr->pos < psr->sz; psr->pos++) {
    switch(psr->buffer[psr->pos]) {
      case '\r':
      case '\n': {
        if (psr->buffer[psr->pos - 1] == '\\') {
          tok.start = psr->buffer + psr->pos + 1;
          k = 0;
          break;
        }
        if (reading_val) {
          info->count++;
        }
        return 0;
      }
      case '\t':
      case '\v':
      case ' ': {
        tok.start = psr->buffer + psr->pos + 1;
        k = 0;
        break;
      }
      case '=': {
        if (k == 0) return -1;
        tok.sz = k;
        m3u_tag_attr_name_parse(info, &tok);
        k = 0;
        reading_val = 1;
        break;
      }
      case ',': {
        if (k == 0) return -1;
        if (reading_val) {
          if (string_val) {
            info->alist[info->count].value[k] = psr->buffer[psr->pos];
            k++;
            break;
          }
          reading_val = 0;
          tok.start = psr->buffer + psr->pos + 1;
          info->count++;
        }
        k = 0;
        break;
      }
      case '"': {
        if (!string_val && reading_val) {
          string_val = 1;
        } else if (string_val && reading_val) {
          string_val = 0;
        }
        break;
      }
      default: {
        if (reading_val) {
          info->alist[info->count].value[k] = psr->buffer[psr->pos];
        }
        k++;
        break;
      }
    }
  }
  return -1;
}

static int m3u_tag_val_prim_parse(m3u_parser *psr) {
  int k;
  m3u_tag_info *info;
  info = &psr->info->elements[psr->info->el_count].tag;
  psr->pos++;
  for (k = 0; psr->pos < psr->sz; psr->pos++) {
    if (isspace(psr->buffer[psr->pos])) {
      if (psr->buffer[psr->pos - 1] != ',') {
        if (k > 0) info->count++;
        k = 0;
      }
      return 0;
    } else if (psr->buffer[psr->pos] == ',') {
      if (k > 0) info->count++;
      k = 0;
    } else {
      if (k >= sizeof(info->prim[info->count].value)) {
        printke("m3u_parser: tag prim value too long! (>= %zu)",
          sizeof(info->prim[info->count].value));
        return -1;
      }
      info->prim[info->count].value[k++] = psr->buffer[psr->pos];
    }
  }
  return -1;
}

static int m3u_tag_val_parse(m3u_parser *psr) {
  m3u_tag_info *info;
  info = &psr->info->elements[psr->info->el_count].tag;
  switch(info->name) {
    case EXTM3U : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return 0;
    }
    case EXTINF : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_BYTERANGE : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_TARGETDURATION : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_MEDIA_SEQUENCE : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_KEY : {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_PROGRAM_DATE_TIME : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_ALLOW_CACHE : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_PLAYLIST_TYPE : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_STREAM_INF : {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_I_FRAME_STREAM_INF : {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_I_FRAMES_ONLY : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return 0;
    }
    case EXT_X_MAP: {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_MEDIA : {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_ENDLIST : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return 0;
    }
    case EXT_X_DISCONTINUITY : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return 0;
    }
    case EXT_X_DISCONTINUITY_SEQUENCE : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    case EXT_X_INDEPENDENT_SEGMENTS : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return 0;
    }
    case EXT_X_START : {
      info->vtype = M3U_TAG_VAL_ATTR_LIST;
      return m3u_tag_val_alist_parse(psr);
    }
    case EXT_X_VERSION : {
      info->vtype = M3U_TAG_VAL_PRIMITIVE;
      return m3u_tag_val_prim_parse(psr);
    }
    default: return -1;
  }
}

static int m3u_tag_parse(m3u_parser *psr) {
  m3u_token tok;
  size_t start_pos;
  start_pos = psr->pos;
  psr->info->elements[psr->info->el_count].type = M3U_TAG;
  tok.start = psr->buffer + psr->pos;
  for (; psr->pos < psr->sz; psr->pos++) {
    if (psr->buffer[psr->pos] == ':' || isspace(psr->buffer[psr->pos])
     || (psr->pos == (psr->sz - 1))) {
      tok.sz = psr->pos - start_pos;
      m3u_tag_name_parse(&psr->info->elements[psr->info->el_count], &tok);
      break;
    }
  }
  if (m3u_tag_val_parse(psr) == 0) {
    psr->info->el_count++;
  }
  return 0;
}

static int m3u_uri_parse(m3u_parser *psr) {
  int k;
  kr_m3u_info *info;
  info = psr->info;
  info->elements[psr->info->el_count].type = M3U_URI;
  for (k = 0; psr->pos < psr->sz; psr->pos++) {
    if (!isspace(psr->buffer[psr->pos])) {
      if (k >= sizeof(info->elements[psr->info->el_count].uri)) {
        printke("m3u_parser: uri too long (>= %zu)",
         sizeof(info->elements[psr->info->el_count].uri));
        return -1;
      }
      info->elements[psr->info->el_count].uri[k++] = psr->buffer[psr->pos];
    }
    if (isspace(psr->buffer[psr->pos]) || (psr->pos == (psr->sz - 1))) {
      if (k > 0) info->el_count++;
      break;
    }
  }
  return 0;
}

static int m3u_parse(m3u_parser *psr) {
  for (; psr->pos < psr->sz; psr->pos++) {
    if (isspace(psr->buffer[psr->pos])) {
      get_to_nl(psr);
    }
    switch (psr->buffer[psr->pos]) {
      case '#': {
        if (m3u_tag_parse(psr) != 0) return -1;
        break;
      }
      default: {
        if (m3u_uri_parse(psr) != 0) return -1;
        break;
      }
    }
    get_to_nl(psr);
  }
  return 0;
}

int kr_m3u_parse(kr_m3u_info *info, char *buffer, size_t sz) {
  m3u_parser psr;
  if (!info || !buffer) return -1;
  if (sz <= 0) return -1;
  psr.info = info;
  psr.buffer = buffer;
  psr.sz = sz;
  psr.pos = 0;
  return m3u_parse(&psr);
}
