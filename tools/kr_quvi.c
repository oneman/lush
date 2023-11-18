#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <locale.h>
#include <quvi.h>
#include <krad/gen/metainfo.h>
#include <krad/tasks/quvi/types.h>

static int check_quvi(quvi_t q) {
  if (!quvi_ok(q)) {
    fprintf(stderr, "quvi error: %s\n", quvi_errmsg(q));
    return 0;
  }
  return 1;
}

int kr_media_info_get(const char *url) {
  kr_quvi_info info;
  int mode;
  char *tmp;
  char json[32000];
  quvi_t q;
  quvi_media_t media;
  kr_media_version *ver;
  mode = QUVI_SUPPORTS_MODE_OFFLINE;
  q = quvi_new();
  if (!check_quvi(q)) {
    quvi_free(q);
    return -1;
  }
  memset(&info, 0, sizeof(info));
  //strncpy(info.url, url, sizeof(info.url));
  if (quvi_supports(q, url, mode, QUVI_SUPPORTS_TYPE_MEDIA)) {
    media = quvi_media_new(q, url);
    if (check_quvi(q)) {
      quvi_media_get(media, QUVI_MEDIA_PROPERTY_ID, &tmp);
      strncpy(info.status.media_info.id, tmp, sizeof(info.status.media_info.id));
      quvi_media_get(media, QUVI_MEDIA_PROPERTY_TITLE, &tmp);
      strncpy(info.status.media_info.title, tmp, sizeof(info.status.media_info.title));
      quvi_media_get(media, QUVI_MEDIA_PROPERTY_THUMBNAIL_URL, &tmp);
      strncpy(info.status.media_info.thumbnail_url, tmp, sizeof(info.status.media_info.thumbnail_url));
      quvi_media_get(media, QUVI_MEDIA_PROPERTY_START_TIME_MS, &info.status.media_info.start_time_ms);
      quvi_media_get(media, QUVI_MEDIA_PROPERTY_DURATION_MS, &info.status.media_info.duration_ms);
      while (quvi_media_stream_next(media)) {
        ver = &info.status.media_info.version[info.status.media_info.versions++];
          quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_ID, &tmp);
        strncpy(ver->id, tmp, sizeof(ver->id));
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_URL, &tmp);
        strncpy(ver->url, tmp, sizeof(ver->url));
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_CONTAINER, &tmp);
        strncpy(ver->container, tmp, sizeof(ver->container));
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_VIDEO_ENCODING, &tmp);
        strncpy(ver->video_encoding, tmp, sizeof(ver->video_encoding));
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_AUDIO_ENCODING, &tmp);
        strncpy(ver->audio_encoding, tmp, sizeof(ver->audio_encoding));
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_VIDEO_BITRATE_KBIT_S, &ver->video_bitrate);
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_AUDIO_BITRATE_KBIT_S, &ver->audio_bitrate);
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_VIDEO_HEIGHT, &ver->video_height);
        quvi_media_get(media, QUVI_MEDIA_STREAM_PROPERTY_VIDEO_WIDTH, &ver->video_width);
        if (info.status.media_info.versions >= 32) break;
      }
      kr_quvi_info_pack_json(json, &info, sizeof(json));
    }
    quvi_media_free(media);
  }
  quvi_free(q);
  printf("%.*s", (int)sizeof(json), json);
  /* libkdecore calls setlocale(LC_ALL, ""), which breaks basic C string
   * processing functions. libkdecore can be loaded by libproxy, which is
   * used by libquvi9. This is a rather dirty workaround to reset locales.
   */
  setlocale(LC_ALL, "C");
  return 0;
}

int main(int argc, char **argv) {
  int ret;
  if (argc != 2) {
    fprintf(stderr, "Usage: kr_quvi_test [url]\n");
    return 1;
  }
  ret = kr_media_info_get(argv[1]);
  if (ret != 0) {
   fprintf(stderr, "FAILED!\n");
   return 1;
  }
  return 0;
}
