#include <krad/io/file.h>
#include <krad/io/dir.h>
#include <krad/time/stamp.h>
#include "media.h"

struct kr_media {
  uuid_t id;
  struct timespec create_time;
  kr_media_info info;
  kr_file *curr_file;
  kr_dir *dir;
  void *user;
  kr_media_pool *pool;
};

static int run_media_event(kr_media *media, kr_media_event_type event_type) {
  kr_media_event event;
  event.media = media;
  event.type = event_type;
  return kr_media_pool_run_event(media->pool, &event);
}

size_t kr_media_sizeof() {
  return sizeof(kr_media);
}

uuid_t *kr_media_get_id(kr_media *media) {
  if (!media) return NULL;
  return &media->id;
}

kr_media_info *kr_media_get_info(kr_media *media) {
  if (!media) return NULL;
  return &media->info;
}

int kr_media_init(kr_media *media, kr_media_pool *pool, kr_media_setup *setup) {
  int res;
  char *root_path;
  if (!media || !pool || !setup) return -1;
  memset(media, 0, sizeof(*media));
  uuid_generate(media->id);
  uuid_unparse_lower(media->id, media->info.id);
  kr_timestamp_now(&media->info.create_time, &media->create_time);
  root_path = getenv("KRAD_OPS_ROOT");
  if (!root_path) return -2;
  res = snprintf(media->info.media_dir, sizeof(media->info.media_dir),
   "%s/%s", root_path, media->info.id);
  if (res >= sizeof(media->info.media_dir)) return -3;
  res = kr_dir_create(media->info.media_dir);
  if (res != 0) return -4;
  media->user = setup->user;
  media->pool = pool;
  run_media_event(media, KR_MEDIA_CREATE);
  return 0;
}

int kr_media_clear(kr_media *media) {
  if (!media) return -1;
  if (media->curr_file)
    kr_media_file_close(media);
  /*if (media->dir)
    kr_dir_close(media->dir);*/
  run_media_event(media, KR_MEDIA_DESTROY);
  return 0;
}

void *kr_media_user_get(kr_media *media) {
  if (!media) return NULL;
  return media->user;
}

int kr_media_user_set(kr_media *media, void *user) {
  if (!media) return -1;
  media->user = user;
  return 0;
}

int kr_media_add_item(kr_media *media, kr_media_item_info *info) {
  size_t max_sz;
  kr_media_info *minfo;
  if (!media) return -1;
  if (!info) return -1;
  minfo = &media->info;
  max_sz = sizeof(minfo->item)/sizeof(minfo->item[0]);
  if (minfo->nitems >= max_sz) return -1;
  minfo->item[minfo->nitems++] = *info;
  run_media_event(media, KR_MEDIA_ITEM_ADD);
  return 0;
}

int kr_media_file_create(kr_media *media, char *name) {
  int res;
  kr_file *file;
  char *root_path;
  kr_media_item_info info;
  if (!media || !name) return -1;
  if (name[0] == '/') return -2;
  memset(&info, 0, sizeof(info));
  info.type = KR_MEDIA_FILE;
  kr_timestamp_now(&info.create_time, &media->create_time);
  root_path = getenv("KRAD_OPS_ROOT");
  if (!root_path) return -3;
  res = snprintf(info.file.name, sizeof(info.file.name),
   "%s/%s", root_path, name);
  if (res >= sizeof(info.file.name)) return -4;
  if (file_exists(name)) return -5;
  file = kr_file_create(name);
  if (!file) return -6;
  kr_file_close(&file);
  run_media_event(media, KR_MEDIA_FILE_CREATE);
  return 0;
}

int kr_media_file_open(kr_media *media, char *name) {
  if (!media || !name) return -1;
  media->curr_file = kr_file_open(name);
  if (media->curr_file == NULL) return -2;
  return 0;
}

int kr_media_file_close(kr_media *media) {
  if (!media) return -1;
  if (!media->curr_file) return -1;
  return kr_file_close(&media->curr_file);
}

int kr_media_files_iter(kr_media *media) {
  if (!media) return -1;
  if (!media->dir) return -2;
  return 0;
}
