#if !defined(_image_stash_H)
# define _image_stash_H (1)

#include <stdlib.h>
#include <uuid/uuid.h>
#include <krad/mem/pool.h>
#include <krad/image/image.h>
#include <krad/image/stash_types.h>

typedef struct kr_image_stash kr_image_stash;
typedef struct kr_image_stash_item kr_image_stash_item;
typedef struct kr_image_stash_event kr_image_stash_event;
typedef int (kr_image_stash_event_cb)(kr_image_stash_event *);

typedef enum {
  KR_IMAGE_STASH_ADD = 1,
  KR_IMAGE_STASH_REMOVE,
  KR_IMAGE_STASH_ENCODE,
  KR_IMAGE_STASH_DECODE
} kr_image_stash_event_type;

typedef struct {
  kr_image_type type;
  union {
    kr_image raw;
    kr_coded_image enc;
  };
} kr_stash_image;

struct kr_image_stash_event {
  void *user;
  kr_image_stash *stash;
  kr_image_stash_item *item;
  kr_image_stash_event_type type;
  kr_image_stash_item_info info;
};

typedef struct {
  void *mem;
  size_t stash_size;
  kr_image_stash_event_cb *event_cb;
  void *user;
} kr_image_stash_setup;

size_t kr_image_stash_sizeof();
kr_image_stash *kr_image_stash_init(kr_image_stash_setup *setup);
int kr_image_stash_destroy(kr_image_stash *);
kr_image_stash_item *kr_image_stash_put(kr_image_stash *, kr_stash_image *);
int kr_image_stash_del(kr_image_stash *, kr_image_stash_item *);
kr_image_stash_item *kr_image_stash_iter(kr_image_stash *, uint32_t *);
kr_image_stash_item *kr_image_stash_dec(kr_image_stash *,
 kr_image_stash_item *);
kr_image_stash_item *kr_stash_decode_put(kr_image_stash *, kr_stash_image *);
kr_image_stash_item *kr_image_stash_enc(kr_image_stash *,
 kr_coded_image_type , kr_image_stash_item *);
kr_image_stash_item *kr_image_stash_find_by_id(kr_image_stash *stash,
 char *uid, size_t len);
int kr_image_stash_del(kr_image_stash *stash, kr_image_stash_item *item);
size_t kr_image_stash_item_uid(kr_image_stash_item *item,
 char *buf, size_t sz);
kr_stash_image *kr_image_stash_item_image(kr_image_stash_item *item);
int kr_image_stash_item_info_get(kr_image_stash_item *item,
 kr_image_stash_item_info *info);

#endif
