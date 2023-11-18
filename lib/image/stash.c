#include <stdlib.h>
#include <stdint.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/mem/pool.h>
#include <krad/image/coder.h>
#include <krad/image/convert.h>
#include <krad/hash/hashids.h>
#include "stash.h"

struct kr_image_stash_item {
  kr_hash_id hid;
  kr_image_stash_item_info info;
  uint8_t alloc;
  kr_stash_image image;
  kr_image_stash *stash;
};

struct kr_image_stash {
  kr_hashids *hids;
  kr_image_stash_item *items;
  size_t size;
  kr_convert conv;
  void *user;
  kr_image_stash_event_cb *event_cb;
};

static void run_event(kr_image_stash *stash, kr_image_stash_item *item,
 kr_image_stash_event_type type) {
  kr_image_stash_event event;
  event.user = stash->user;
  event.type = type;
  event.stash = stash;
  event.item = item;
  event.info = item->info;
  stash->event_cb(&event);
}

static kr_image_stash_item *stash_new_item(kr_image_stash *stash) {
  int i;
  for (i = 0; i < stash->size; i++) {
    if (stash->items[i].hid.id == 0) {
      stash->items[i].stash = stash;
      return &stash->items[i];
    }
  }
  return NULL;
}

static void stash_del_item(kr_image_stash_item *item) {
  if (item->hid.id != 0) {
    if (item->alloc) {
      if (item->image.type == KR_IMAGE_ENC) {
        if (item->image.enc.data) free(item->image.enc.data);
      }
      if (item->image.type == KR_IMAGE_RAW) {
        if (item->image.raw.px[0]) free(item->image.raw.px[0]);
      }
    }
    memset(item, 0, sizeof(*item));
  }
}

static kr_image_stash_item *stash_get_item(kr_image_stash *stash,
 char *uid, size_t len) {
  int i;
  if (!len) return NULL;
  for (i = 0; i < stash->size; i++) {
    if (len == stash->items[i].hid.len &&
      !memcmp(stash->items[i].hid.uid, uid, len)) {
      return &stash->items[i];
    }
  }
  return NULL;
}

int kr_image_stash_item_info_get(kr_image_stash_item *item,
 kr_image_stash_item_info *info) {
  if (!item || !info) return -1;
  *info = item->info;
  return 0;
}

size_t kr_image_stash_sizeof() {
  return sizeof(kr_image_stash);
}

size_t kr_image_stash_item_uid(kr_image_stash_item *item,
 char *buf, size_t sz) {
  if (!buf || !sz) return 0;
  if (sz < item->hid.len) return 0;
  memcpy(buf, item->hid.uid, item->hid.len);
  return item->hid.len;
}

kr_stash_image *kr_image_stash_item_image(kr_image_stash_item *item) {
  return &item->image;
}

kr_image_stash_item *kr_image_stash_find_by_id(kr_image_stash *stash,
 char *uid, size_t len) {
  if (!stash || !uid) return NULL;
  return stash_get_item(stash, uid, len);
}

int kr_image_stash_del(kr_image_stash *stash, kr_image_stash_item *item) {
  stash_del_item(item);
  run_event(stash, item, KR_IMAGE_STASH_REMOVE);
  return 0;
}

kr_image_stash_item *kr_stash_decode_put(kr_image_stash *stash,
 kr_stash_image *image) {
  int ret;
  kr_stash_image out;
  kr_image_stash_item *item;
  if (stash == NULL || image == NULL) return NULL;
  if (image->type != KR_IMAGE_ENC) return NULL;
  item = stash_new_item(stash);
  if (item == NULL) return NULL;
  memset(&out, 0, sizeof(out));
  out.raw.info = image->enc.info.image;
  //out.raw.px[0] = kr_alloc(out.raw.info.w * out.raw.info.h * 4);
  //out.raw.ps[0] = out.raw.info.w * 4;
  ret = kr_image_decode(&out.raw, &image->enc);
  if (ret != 0) {
    if (out.raw.px[0]) free(out.raw.px[0]);
    printke("kr_stash_decode_put: decoding failed %d", ret);
    return NULL;
  }
  item->image.type = KR_IMAGE_RAW;
  item->image.raw = out.raw;
  ret = kr_hashids_gen(item->stash->hids, &item->hid);
  if (ret != 0) return NULL;
  item->info.created_on = item->hid.id;
  item->alloc = 1;
  item->info.type = KR_IMAGE_RAW;
  item->info.width = out.raw.info.w;
  item->info.height = out.raw.info.h;
  item->info.raw_fmt = out.raw.info.fmt;
  item->info.sz = kr_image_sizeof(&out.raw.info);
  run_event(stash, item, KR_IMAGE_STASH_ADD);
  //run_event(stash, item, KR_IMAGE_STASH_DECODE);
  return item;
}

kr_image_stash_item *kr_image_stash_put(kr_image_stash *stash,
 kr_stash_image *image) {
  int ret;
  kr_image_stash_item *item;
  if (stash == NULL || image == NULL) return NULL;
  item = stash_new_item(stash);
  if (item == NULL) return NULL;
  item->image = *image;
  ret = kr_hashids_gen(item->stash->hids, &item->hid);
  if (ret != 0) return NULL;
  item->info.type = image->type;
  if (image->type == KR_IMAGE_RAW) {
    item->info.width = image->raw.info.w;
    item->info.height = image->raw.info.h;
    item->info.raw_fmt = image->raw.info.fmt;
    item->info.sz = kr_image_sizeof(&image->raw.info);
  }
  if (image->type == KR_IMAGE_ENC) {
    item->info.width = image->enc.info.image.w;
    item->info.height = image->enc.info.image.h;
    item->info.enc_fmt = image->enc.info.fmt;
    item->info.sz = image->enc.info.sz;
  }
  run_event(stash, item, KR_IMAGE_STASH_ADD);
  return item;
}

kr_image_stash_item *kr_image_stash_iter(kr_image_stash *stash,
 uint32_t *count) {
  int i;
  int j;
  if (stash == NULL || *count >= stash->size) return NULL;
  for (i = j = 0; i < stash->size; i++) {
    if (stash->items[i].hid.id != 0) {
      ++j;
      if (*count < j) {
        (*count)++;
        return &stash->items[i];
      }
    }
  }
  return NULL;
}

kr_image_stash_item *kr_image_stash_dec(kr_image_stash *stash,
 kr_image_stash_item *item) {
  int res;
  kr_image_stash_item *dec_item;
  if (stash == NULL || item == NULL) return NULL;
  if (item->image.type != KR_IMAGE_ENC) return NULL;
  dec_item = stash_new_item(stash);
  if (dec_item == NULL) return NULL;
  dec_item->image.raw.info = item->image.enc.info.image;
  //dec_item->image.raw.px[0] = kr_alloc(dec_item->image.raw.info.w
  //   * dec_item->image.raw.info.h * 4);
  //dec_item->image.raw.ps[0] = dec_item->image.raw.info.w * 4;
  //dec_item->image.raw.info.fmt = KR_PIXELS_ARGB;
  res = kr_image_decode(&dec_item->image.raw, &item->image.enc);
  dec_item->alloc = 1;
  if (res != 0) {
    stash_del_item(dec_item);
    return NULL;
  }
  res = kr_hashids_gen(dec_item->stash->hids, &dec_item->hid);
  if (res != 0) return NULL;
  dec_item->info.type = KR_IMAGE_RAW;
  dec_item->info.width = dec_item->image.raw.info.w;
  dec_item->info.height = dec_item->image.raw.info.h;
  dec_item->info.raw_fmt = dec_item->image.raw.info.fmt;
  dec_item->info.sz = kr_image_sizeof(&dec_item->image.raw.info);
  run_event(stash, dec_item, KR_IMAGE_STASH_DECODE);
  return dec_item;
}

kr_image_stash_item *kr_image_stash_enc(kr_image_stash *stash,
 kr_coded_image_type fmt, kr_image_stash_item *item) {
  int ret;
  kr_image raw_img;
  kr_image_stash_item *enc_item;
  if (stash == NULL || item == NULL) return NULL;
  if (item->image.type != KR_IMAGE_RAW) return NULL;
  enc_item = stash_new_item(stash);
  if (enc_item == NULL) return NULL;
  memset(&raw_img, 0, sizeof(raw_img));
  memset(&enc_item->image, 0, sizeof(enc_item->image));
  if (item->image.raw.info.fmt != KR_PIXELS_ARGB) {
    raw_img.info = item->image.raw.info;
    raw_img.px[0] = kr_alloc(item->image.raw.info.w
     * item->image.raw.info.h * 4);
    raw_img.ps[0] = item->image.raw.info.w * 4;
    raw_img.info.fmt = KR_PIXELS_ARGB;
    ret = kr_image_convert(&stash->conv, &raw_img, &item->image.raw);
    if (ret != 0) {
      free(raw_img.px[0]);
      printke("image_stash: img conv failed! %d", ret);
      return NULL;
    }
  } else {
    raw_img = item->image.raw;
  }
  enc_item->image.enc.data = kr_alloc(item->image.raw.info.w
     * item->image.raw.info.h * 4);
  enc_item->image.enc.info.sz = item->image.raw.info.w
     * item->image.raw.info.h * 4;
  enc_item->image.enc.info.image.fmt = item->image.raw.info.fmt;
  enc_item->alloc = 1;
  ret = kr_image_encode(&enc_item->image.enc, &raw_img);
  if (raw_img.px[0]) free(raw_img.px[0]);
  if (ret != 0) {
    stash_del_item(item);
    return NULL;
  }
  ret = kr_hashids_gen(enc_item->stash->hids, &enc_item->hid);
  if (ret != 0) return NULL;
  enc_item->info.type = KR_IMAGE_ENC;
  enc_item->info.width = enc_item->image.enc.info.image.w;
  enc_item->info.height = enc_item->image.enc.info.image.h;
  enc_item->info.enc_fmt = enc_item->image.enc.info.fmt;
  enc_item->info.sz = enc_item->image.enc.info.sz;
  run_event(stash, enc_item, KR_IMAGE_STASH_ENCODE);
  return enc_item;
}

int kr_image_stash_destroy(kr_image_stash *stash) {
  int i;
  if (stash == NULL) return -1;
  if (stash->items == NULL) return -1;
  for (i = 0; i < stash->size; i++) {
    stash_del_item(&stash->items[i]);
  }
  free(stash->items);
  free(stash->hids);
  kr_image_convert_clear(&stash->conv);
  return 0;
}

kr_image_stash *kr_image_stash_init(kr_image_stash_setup *setup) {
  kr_image_stash *stash;
  kr_hashids_setup hids_setup;
  if (!setup) return NULL;
  if (!setup->mem || !setup->stash_size) return NULL;
  stash = (kr_image_stash *)setup->mem;
  stash->size = setup->stash_size;
  stash->items = kr_allocz(stash->size, sizeof(kr_image_stash_item));
  stash->user = setup->user;
  stash->event_cb = setup->event_cb;
  kr_image_convert_init(&stash->conv);
  hids_setup.mem = kr_allocz(1, kr_hashids_sizeof());
  memcpy(hids_setup.salt, "kr_stash", 8);
  hids_setup.len = 8;
  stash->hids = kr_hashids_init(&hids_setup);
  return stash;
}
