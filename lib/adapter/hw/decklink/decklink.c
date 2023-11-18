#include <krad/app/debug.h>
#include "decklink.h"
#include "capture.h"

typedef struct {
  kr_decklink_capture *capture;
  kr_adapter *adapter;
  kr_adapter_path *adapter_path;
  kr_decklink_info *info;
  kr_decklink_path_info *path_info;
} kr_decklink;

static int free_frame(void *user);
static kr_frame *dup_frame(kr_frame *frame);
static int user_event(kr_event *event);
static int audio_cb(void *user, void *audio, int nframes);
static int frame_cb(void *user, kr_image *image);

static int audio_cb(void *user, void *audio, int nframes) {
  return 0;
}

static int free_frame(void *user) {
  kr_frame *f;
  if (!user) return -1;
  f = (kr_frame *)user;
  free(f->image.px[0]);
  free(f);
  return 0;
}

static kr_frame *dup_frame(kr_frame *frame) {
  kr_frame *df;
  size_t sz;
  df = calloc(1, sizeof(*frame));
  *df = *frame;
  sz = df->image.info.h * df->image.ps[0];
  df->image.px[0] = malloc(sz);
  memcpy(df->image.px[0], frame->image.px[0], sz);
  df->image.owner = df;
  df->image.release = free_frame;
  return df;
}

static int user_event(kr_event *event) {
  if (event == NULL) return -1;
  kr_decklink *dl;
  int ret;
  kr_frame frame;
  dl = (kr_decklink *)event->user;
  if (event->events & EPOLLIN) {
    //frame = NULL;
    ret = dl->adapter_path->read(dl->adapter_path->user, (kr_av *)&frame);
    /*printk("Decklink: user event read ret %d", ret);*/
    if (ret != 1) {
      printke("Decklink: problem reading frame back");
      return -1;
    }
    return ret;
  } else {
    printke("Decklink: Got an unreadable user event");
  }
  return 0;
}

static int frame_cb(void *user, kr_image *image) {
  int ret;
  kr_decklink *dl;
  kr_frame frame;
  kr_frame *df;
  dl = (kr_decklink *)user;
  frame.type = KR_IMAGE_RAW;
  frame.image = *image;
  df = dup_frame(&frame);
  ret = dl->adapter_path->write(dl->adapter_path->user, (kr_av *)df);
  return ret;
}

int kr_decklink_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Decklink adapter path controlled");
  return 0;
}

int kr_decklink_unlink(kr_adapter_path *path) {
  int ret;
  kr_decklink *dl;
  if (path == NULL) return -1;
  dl = (kr_decklink *)path->handle;
  if (!dl) return -1;
  if (!dl->adapter_path) {
    printke("Decklink: unlinking null adapter path");
    return -1;
  }
  ret = kr_decklink_capture_close(dl->capture);
  ret = kr_loop_del(dl->adapter->loop, path->fd);
  if (ret != 0) {
    printke("Decklink: error removing loop fd");
  }
  dl->capture = NULL;
  dl->adapter_path = NULL;
  printk("Decklink adapter path removed");
  return ret;
}

int kr_decklink_link(kr_adapter *adp, kr_adapter_path *path) {
  kr_decklink *dl;
  kr_decklink_setup setup;
  kr_event harness;
  if (adp == NULL) return -1;
  if (path == NULL) return -2;
  dl = (kr_decklink *)adp->handle;
  if (dl->adapter_path) {
    printke("Decklink: adapter already had link!");
    kr_decklink_unlink(path);
  }
  dl->adapter_path = path;
  dl->path_info = &path->info->dlv_in;
  setup.frame_cb = frame_cb;
  setup.audio_cb = audio_cb;
  setup.info = dl->info;
  setup.path_info = dl->path_info;
  setup.user = dl;
  dl->capture = kr_decklink_capture_open(&setup);
  if (!dl->capture) {
    printke("Decklink adapter path failed!");
  }
  harness.user = dl;
  harness.fd = path->fd;
  harness.handler = user_event;
  harness.events = EPOLLIN;
  kr_loop_add(adp->loop, &harness);
  printk("Decklink adapter path created");
  return 0;
}

int kr_decklink_ctl(kr_adapter *adp, kr_patchset *patchset) {
  if (adp == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("Decklink adapter controlled");
  return 0;
}

int kr_decklink_close(kr_adapter *adapter) {
  int ret;
  kr_decklink *dl;
  if (adapter == NULL) return -1;
  dl = (kr_decklink *)adapter->handle;
  if (dl == NULL) return -1;
  printk("Decklink: Adapter closing");
  if (dl->adapter_path) kr_decklink_unlink(dl->adapter_path);
  ret = kr_loop_destroy(adapter->loop);
  if (ret != 0) {
    printke("V4L2: trouble unlooping");
  }
  free(dl);
  adapter->handle = NULL;
  printk("Decklink: adapter closed");
  return 0;
}

int kr_decklink_open(kr_adapter *adapter) {
  int ret;
  kr_decklink *dl;
  kr_loop_setup loop_setup;
  ret = 0;
  if (adapter == NULL) return -1;
  printk("Decklink: adapter open");
  adapter->handle = kr_allocz(1, sizeof(*dl));
  dl = (kr_decklink *)adapter->handle;
  dl->adapter = adapter;
  dl->info = &adapter->info->dl;
  ret = kr_decklink_get_name(dl->info);
  if (ret > 0) {
    kr_loop_setup_init(&loop_setup);
    snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_dl_%d",
     dl->info->num);
    adapter->loop = kr_loop_create(&loop_setup);
    printk("Decklink: adapter opened: %d - %s", dl->info->num, dl->info->name);
    return 0;
  }
  return ret;
}
