#if !defined(_ops_media_media_H)
#define _ops_media_media_H (1)

#include <uuid/uuid.h>
#include <krad/io/file.h>
#include <krad/io/dir.h>
#include "types.h"

typedef struct kr_media kr_media;
typedef struct kr_media_event kr_media_event;

struct kr_media_event {
  kr_media *media;
  kr_patchset patchset;
  kr_media_event_type type;
};

typedef int (kr_media_event_cb)(kr_media_event *);

#include "pool.h"

size_t kr_media_sizeof();
uuid_t *kr_media_get_id(kr_media *media);
kr_media_info *kr_media_get_info(kr_media *media);
int kr_media_init(kr_media *media, kr_media_pool *pool, kr_media_setup *setup);
int kr_media_clear(kr_media *media);
void *kr_media_user_get(kr_media *media);
int kr_media_user_set(kr_media *media, void *user);
int kr_media_add_item(kr_media *media, kr_media_item_info *info);
int kr_media_file_open(kr_media *media, char *name);
int kr_media_file_close(kr_media *media);
int kr_media_files_iter(kr_media *media);
int kr_media_file_create(kr_media *media, char *name);

#endif
