#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <string.h>
#include <math.h>
#include <krad/app/debug.h>
#include <krad/playlist/m3u_types.h>
#include "hls_tuner.h"

static char *get_attr(m3u_tag_info *tag, kr_m3u_tag_attr_name name) {
  int i;
  if (tag->vtype != M3U_TAG_VAL_ATTR_LIST) return NULL;
  for (i = 0; i < tag->count; i++) {
    if (tag->alist[i].name == name) return tag->alist[i].value;
  }
  return NULL;
}

static int handle_tag(kr_hls_playlist *plst, m3u_tag_info *tag,
 m3u_el_info *elements, int i, int el_count) {
  kr_hls_media_segment *seg;
  kr_hls_rendition *rend;
  kr_hls_variant *var;
  char *value;
  switch(tag->name) {
    case EXTM3U : {
      // weird, it shouldn't be here, ignoring...
      break;
    }
    case EXTINF : {
      if (plst->type != KR_HLS_PLAYLIST_MEDIA) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      if (i == (el_count - 1) || elements[i + 1].type != M3U_URI) {
        printke("handle_tag: EXTINF requires a Media Segment URI.");
        return -2;
      }
      if (tag->count < 1) {
        printke("handle_tag: EXTINF requires a duration value.");
        return -3;
      }
      if (plst->media.nsegments == KR_HLS_MAX_SEGMENTS) {
        printke("handle_tag: KR_HLS_MAX_SEGMENTS exceeded.");
        return -4;
      }
      plst->media.type = HLS_LIVE;
      seg = &plst->media.segment[plst->media.nsegments++];
      seg->duration = atof(tag->prim[0].value);
      if (seg->duration == 0.00f || round(seg->duration) > plst->media.max_duration) {
        printke("handle_tag: invalid media segment duration.");
        return -5;
      }
      //strncpy(seg->url, elements[i + 1].uri, sizeof(seg->url));
      seg->sequence_num = plst->media.sequence_num + plst->media.nsegments - 1;
      break;
    }
    case EXT_X_BYTERANGE : {
      if (plst->type == KR_HLS_PLAYLIST_MASTER) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      break;
    }
    case EXT_X_TARGETDURATION : {
      if (plst->type == KR_HLS_PLAYLIST_MASTER) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      if (tag->count < 1) {
        printke("handle_tag: EXT_X_TARGETDURATION requires a duration value.");
        return -2;
      }
      plst->type = KR_HLS_PLAYLIST_MEDIA;
      plst->media.max_duration = atoi(tag->prim[0].value);
      break;
    }
    case EXT_X_MEDIA_SEQUENCE : {
      if (plst->type == KR_HLS_PLAYLIST_MASTER) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      if (tag->count < 1) {
        printke("handle_tag: EXT_X_MEDIA_SEQUENCE requires a numeric value.");
        return -2;
      }
      if (plst->media.nsegments != 0) {
        printke("handle_tag: EXT_X_MEDIA_SEQUENCE must be appear "
         "before the first media segment.");
        return -3;
      }
      plst->type = KR_HLS_PLAYLIST_MEDIA;
      plst->media.sequence_num = atoi(tag->prim[0].value);
      break;
    }
    case EXT_X_KEY : {
      break;
    }
    case EXT_X_PROGRAM_DATE_TIME : {
      break;
    }
    case EXT_X_ALLOW_CACHE : {
      break;
    }
    case EXT_X_PLAYLIST_TYPE : {
      if (plst->type == KR_HLS_PLAYLIST_MASTER) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      if (!strncmp(tag->prim[0].value, "VOD", 3)) {
        plst->media.type = HLS_VOD;
      } else if (!strncmp(tag->prim[0].value, "EVENT", 5)) {
        plst->media.type = HLS_LIVE;
      }
      plst->type = KR_HLS_PLAYLIST_MEDIA;
      break;
    }
    case EXT_X_STREAM_INF : {
      char *uri;
      if (plst->type == KR_HLS_PLAYLIST_MEDIA) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      if (i == (el_count - 1) || elements[i + 1].type != M3U_URI) {
        printke("handle_tag: EXT-X-STREAM-INF requires a Media Segment URI.");
        return -2;
      }
      value = get_attr(tag, M3U_ATTR_BANDWIDTH);
      if (value == NULL) {
        printke("handle_tag: EXT_X_STREAM_INF "
         "requires a BANDWIDTH attribute.");
        return -3;
      }
      plst->type = KR_HLS_PLAYLIST_MASTER;
      var = &plst->master.variant[plst->master.nvariants++];
      var->bitrate = atoi(value);
      if ( (value = get_attr(tag, M3U_ATTR_CODECS)) ) {
        strncpy(var->codecs, value, sizeof(var->codecs));
      }
      if ( (value = get_attr(tag, M3U_ATTR_RESOLUTION)) ) {
        char *end;
        if (strchr(value, 'x')) {
          var->width = strtod(value, &end);
          value = end + 1;
          var->height = strtod(value, &end);
        }
      }
      uri = get_attr(tag, M3U_ATTR_URI);
      if ( (value = get_attr(tag, M3U_ATTR_AUDIO)) ) {
        strncpy(var->name, value, sizeof(var->name));
        var->audio++;
      }
      if ( (value = get_attr(tag, M3U_ATTR_VIDEO)) ) {
        strncpy(var->name, value, sizeof(var->name));
        var->video++;
      }
      if ( (value = get_attr(tag, M3U_ATTR_SUBTITLES)) ) {
        if (!uri) {
          printke("handle_tag: EXT_X_STREAM_INF "
            "requires a URI attr when SUBTITLES attr is defined.");
          return -4;
        }
        strncpy(var->name, value, sizeof(var->name));
        var->subtitles++;
      }
      if (uri) {
        strncpy(var->url, uri, sizeof(var->url));
      } else {
        //strncpy(var->url, elements[i + 1].uri, sizeof(var->url) - 1);
      }
      break;
    }
    case EXT_X_I_FRAME_STREAM_INF : {
      break;
    }
    case EXT_X_I_FRAMES_ONLY : {
      break;
    }
    case EXT_X_MAP: {
      break;
    }
    case EXT_X_MEDIA : {
      if (plst->type == KR_HLS_PLAYLIST_MEDIA) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      plst->type = KR_HLS_PLAYLIST_MASTER;
      rend = &plst->master.rendition[plst->master.nrenditions++];;
      value = get_attr(tag, M3U_ATTR_TYPE);
      if (value == NULL) {
        printke("handle_tag: EXT-X-MEDIA "
         "requires a TYPE attribute.");
        return -2;
      }

      if (!strncmp(value, "AUDIO", 5)) {
        rend->type = HLS_AUDIO;
      } else if (!strncmp(value, "VIDEO", 5)) {
        rend->type = HLS_VIDEO;
      } else if (!strncmp(value, "SUBTITLES", 9)) {
        rend->type = HLS_SUBTITLES;
      } else if (!strncmp(value, "CLOSED-CAPTIONS", 15)) {
        rend->type = HLS_CLOSED_CAPTIONS;
      } else {
        printke("handle_tag: EXT-X-MEDIA, wrong type %s", value);
        return -3;
      }

      value = get_attr(tag, M3U_ATTR_GROUP_ID);
      if (value == NULL) {
        printke("handle_tag: EXT-X-MEDIA "
         "requires a GROUP-ID attribute.");
        return -4;
      }

      strncpy(rend->group, value, sizeof(rend->group));

      value = get_attr(tag, M3U_ATTR_NAME);

      if (value == NULL) {
        printke("handle_tag: EXT-X-MEDIA "
         "requires a NAME attribute.");
        return -4;
      }

      strncpy(rend->name, value, sizeof(rend->name));

      value = get_attr(tag, M3U_ATTR_URI);

      if (value) {
        strncpy(rend->url, value, sizeof(rend->url));
      }

      value = get_attr(tag, M3U_ATTR_DEFAULT);

      if (value && !strncmp(value, "YES", 3)) {
        rend->def++;
      }

      break;
    }
    case EXT_X_ENDLIST : {
      if (plst->type != KR_HLS_PLAYLIST_MEDIA) {
        printke("handle_tag: wrong playlist type.");
        return -1;
      }
      plst->media.type = HLS_VOD;
      break;
    }
    case EXT_X_DISCONTINUITY : {
      break;
    }
    case EXT_X_DISCONTINUITY_SEQUENCE : {
      break;
    }
    case EXT_X_INDEPENDENT_SEGMENTS : {
      break;
    }
    case EXT_X_START : {
      break;
    }
    case EXT_X_VERSION : {
      break;
    }
    default: return -1;
  }
  return 0;
}

int m3u_to_hls(kr_hls_playlist *plst, kr_m3u_info *info) {
  int i;
  int ret;
  m3u_el_info *el;
  if (!plst || !info) return -1;
  if (info->el_count < 1) {
    printke("m3u_to_hls: this is not a valid playlist file.");
    return -1;
  }
  el = &info->elements[0];
  if (el->type != M3U_TAG) {
    printke("m3u_to_hls: this is not a valid playlist file.");
    return -1;
  }
  if (el->type == M3U_TAG && el->tag.name != EXTM3U) {
    printke("m3u_to_hls: this is not a valid playlist file.");
    return -1;
  }
  for (i = 1; i < info->el_count; i++) {
    el = &info->elements[i];
    if (el->type == M3U_TAG) {
      ret = handle_tag(plst, &el->tag, info->elements, i, info->el_count);
      if (ret != 0) return -1;
    }
  }
  return 0;
}
