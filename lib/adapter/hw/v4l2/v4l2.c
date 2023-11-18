#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <math.h>
#include <poll.h>
#include <dirent.h>
#include <linux/videodev2.h>
#include <linux/uvcvideo.h>
#include <krad/app/debug.h>
#include <krad/image/pool.h>
#include <krad/image/convert.h>
#include <krad/image/debug.h>
#include "v4l2.h"

#define KR_V4L2_NFRAMES 12

typedef struct kr_v4l2 kr_v4l2;

typedef struct {
  struct v4l2_buffer buf;
  kr_frame frame;
  kr_v4l2 *kv;
} kv_frame;

struct kr_v4l2 {
  int fd;
  kr_v4l2_info *info;
  kr_v4l2_path_info *path_info;
  kr_adapter *adapter;
  kr_adapter_path *adapter_path;
  uint32_t nframes;
  kv_frame frames[KR_V4L2_NFRAMES];
  uint64_t total_frames;
};

static int read_frame(kr_v4l2 *kv, kr_frame **frame);
static int xioctl(int fd, int request, void *arg);
static int user_event(kr_event *event);
static int device_event(kr_event *event);
static int frame_release(void *ptr);
static int capture_off(kr_v4l2 *kv);
static int capture_on(kr_v4l2 *kv);
static void unmap_frames(kr_v4l2 *kv);
static int map_frames(kr_v4l2 *kv);
static int set_params(kr_v4l2 *kv);
static void teardown(kr_v4l2 *kv);
static int kv_init(kr_v4l2 *kv);

static int xioctl(int fd, int request, void *arg) {
  int r;
  do r = ioctl(fd, request, arg);
  while (-1 == r && EINTR == errno);
  return r;
}

static int frame_release(void *ptr) {
  kv_frame *kvframe;
  if (ptr == NULL) return -1;
  kvframe = (kv_frame *)ptr;
  if (-1 == xioctl(kvframe->kv->fd, VIDIOC_QBUF, &kvframe->buf)) {
    printke("V4L2: VIDIOC_QBUF");
    return -1;
  }
  return 0;
}

static int read_frame(kr_v4l2 *kv, kr_frame **frame) {
  struct v4l2_buffer buf;
  if (kv == NULL) return -1;
  if (frame == NULL) return -1;
  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (-1 == xioctl(kv->fd, VIDIOC_DQBUF, &buf)) {
    switch (errno) {
      case EAGAIN:
        return 0;
      default:
        printke("Krad V4L2: VIDIOC_DQBUF");
        return -1;
    }
  }
  kv->frames[buf.index].frame.info.ts = buf.timestamp;
  *frame = &kv->frames[buf.index].frame;
  /*kr_frame_debug_time(*frame, "V4L2", 4);*/
  return 1;
}

static int user_event(kr_event *event) {
  if (event == NULL) return -1;
  kr_v4l2 *kv;
  int ret;
  kr_frame frame;
  kv = (kr_v4l2 *)event->user;
  if (event->events & EPOLLIN) {
    //frame = NULL;
    ret = kv->adapter_path->read(kv->adapter_path->user, (kr_av *)&frame);
    /*printk("V4L2: user event read ret %d", ret);*/
    if (ret != 1) {
      printke("V4L2: problem reading frame back");
      return -1;
    }
    return ret;
  } else {
    printke("V4L2: Got an unreadable user event");
  }
  return 0;
}

static int device_event(kr_event *event) {
  if (event == NULL) return -1;
  kr_v4l2 *kv;
  int ret;
  kr_frame *frame;
  char dbgstr[64];
  kv = (kr_v4l2 *)event->user;
  if (event->events & EPOLLIN) {
    ret = read_frame(kv, &frame);
    if (ret != 1) {
      printke("V4L2: problem reading frame");
      return -1;
    }
    kv->total_frames++;
    snprintf(dbgstr, sizeof(dbgstr), "V4L2 CAP ;%010"PRIu64"",
     kv->total_frames);
    kr_image_debug_render(&frame->image, 1, dbgstr);
    ret = kv->adapter_path->write(kv->adapter_path->user, (kr_av *)frame);
    /*printk("V4L2: device event write ret %d", ret);*/
    if (ret != 1) {
      frame->image.release(frame->image.owner);
    }
    return ret;
  } else {
    printke("V4L2: Got an unreadable event");
  }
  return 0;
}

static int capture_off(kr_v4l2 *kv) {
  enum v4l2_buf_type type;
  if (kv == NULL) return -1;
  if (kv->fd == -1) return -1;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (kv->info->state != KR_V4L2_CAPTURE) {
    return 0;
  }
  if (-1 == xioctl(kv->fd, VIDIOC_STREAMOFF, &type)) {
    printke("V4L2: VIDIOC_STREAMOFF");
    kv->info->state = KR_V4L2_VOID;
    return -1;
  }
  kv->info->state = KR_V4L2_VOID;
  return 0;
}

static int capture_on(kr_v4l2 *kv) {
  uint32_t i;
  struct v4l2_buffer buf;
  enum v4l2_buf_type type;
  if (kv == NULL) return -1;
  if (kv->fd == -1) return -1;
  if (kv->nframes == 0) return -1;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  for (i = 0; i < kv->nframes; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = type;
    buf.index = i;
    buf.memory = V4L2_MEMORY_MMAP;
    if (-1 == xioctl(kv->fd, VIDIOC_QBUF, &buf)) {
      printke("V4L2: VIDIOC_QBUF");
      return -1;
    }
  }
  if (-1 == xioctl(kv->fd, VIDIOC_STREAMON, &type)) {
    printke("V4L2: VIDIOC_STREAMON");
    return -1;
  }
  kv->info->state = KR_V4L2_CAPTURE;
  return 0;
}

static void unmap_frames(kr_v4l2 *kv) {
  int i;
  kv_frame *kvframe;
  if (kv->nframes > 0) {
    capture_off(kv);
    for (i = 0; i < kv->nframes; i++) {
      kvframe = &kv->frames[i];
      if (-1 == munmap(kvframe->frame.image.px[0], kvframe->buf.length)) {
        printke("V4L2: munmap %d", i);
      }
    }
    kv->nframes = 0;
  }
}

static int map_frames(kr_v4l2 *kv) {
  int i;
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers req;
  kr_frame *frame;
  kr_image *image;
  memset(&req, 0, sizeof(req));
  req.count = KR_V4L2_NFRAMES;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;
  if (-1 == xioctl(kv->fd, VIDIOC_REQBUFS, &req)) {
    if (EINVAL == errno) {
      printke("V4L2: device does not support memory mapping");
    } else {
      printke("V4L2: VIDIOC_REQBUFS");
    }
    return -1;
  }
  if (req.count < 2) {
    printke("V4L2: Insufficient buffer memory");
    return -1;
  }
  kv->nframes = req.count;
  printk("V4L2: %d buffer frames", kv->nframes);
  for (i = 0; i < kv->nframes; i++) {
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (-1 == xioctl(kv->fd, VIDIOC_QUERYBUF, &buf)) {
      printke("V4L2: VIDIOC_QUERYBUF");
      kv->nframes = 0;
      return -1;
    }
    frame = &kv->frames[i].frame;
    image = &frame->image;
    kv->frames[i].kv = kv;
    kv->frames[i].buf = buf;
    image->px[0] = mmap(NULL, buf.length,
     PROT_READ | PROT_WRITE, MAP_SHARED, kv->fd, buf.m.offset);
    if (MAP_FAILED == image->px[0]) {
      printke("V4L2: mmap failure");
      kv->nframes = 0;
      return -1;
    }
    image->px[1] = NULL;
    image->px[2] = NULL;
    image->px[3] = NULL;
    image->ps[0] = kv->path_info->width * 2;
    image->ps[1] = 0;  /* kv->path_info->width/2; */
    image->ps[2] = 0;  /* kv->path_info->width/2; */
    image->ps[3] = 0;
    image->info.w = kv->path_info->width;
    image->info.h = kv->path_info->height;
    image->info.fmt = KR_PIXELS_YUV422P;
    image->release = frame_release;
    image->owner = &kv->frames[buf.index];
  }
  return 0;
}

static int set_params(kr_v4l2 *kv) {
  kr_v4l2_path_info *info;
  info = kv->path_info;
  struct v4l2_format format;
  struct v4l2_streamparm sps;
  memset(&sps, 0, sizeof(sps));
  memset(&format, 0, sizeof(format));
  format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  format.fmt.pix.width = info->width;
  format.fmt.pix.height = info->height;
  format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
  format.fmt.pix.field = V4L2_FIELD_ANY;
  if (-1 == xioctl(kv->fd, VIDIOC_S_FMT, &format)) {
    printke("V4L2: Unable to set desired video format");
    return -1;
  } else {
    printk("V4L2: Set %dx%d", info->width, info->height);
  }
  sps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (-1 == xioctl(kv->fd, VIDIOC_G_PARM, &sps)) {
    printke("V4L2: Unable to get capture parameters");
    return -1;
  }
  sps.parm.capture.timeperframe.numerator = info->den;
  sps.parm.capture.timeperframe.denominator = info->num;
  if (-1 == xioctl(kv->fd, VIDIOC_S_PARM, &sps)) {
    printke("V4L2: Unable to set desired capture parameters");
    printke("V4L2: error %d, %s", errno, strerror(errno));
    return -1;
  }
  return 0;
}

static void teardown(kr_v4l2 *kv) {
  if (kv->fd > -1) {
    capture_off(kv);
    unmap_frames(kv);
    close(kv->fd);
    kv->fd = -1;
    kv->info->state = KR_V4L2_VOID;
  }
}

static int kv_init(kr_v4l2 *kv) {
  struct stat st;
  char device[128];
  struct v4l2_capability cap;
  kv->total_frames = 0;
  snprintf(device, sizeof(device), "/dev/video%d", kv->info->dev);
  if (-1 == stat(device, &st)) {
    printke("V4L2: Can't reckon '%s': %d, %s", device, errno, strerror(errno));
    return -1;
  }
  if (!S_ISCHR(st.st_mode)) {
    printke("V4L2: %s is not a device", device);
    return -2;
  }
  kv->fd = open(device, O_RDWR | O_NONBLOCK, 0);
  if (-1 == kv->fd) {
    printke("V4L2: Access denied '%s': %d, %s", device, errno,
     strerror(errno));
    return -3;
  }
  if (-1 == xioctl(kv->fd, VIDIOC_QUERYCAP, &cap)) {
    printke("V4L2: VIDIOC_QUERYCAP");
    teardown(kv);
    return -4;
  } else {
    if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
      printke("V4L2: %s is not a video capture device", device);
      teardown(kv);
      return -5;
    }
    if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
      printke("V4L2: %s no have streamin support we need", device);
      teardown(kv);
      return -6;
    }
  }
  kv->info->state = KR_V4L2_OPEN;
  return 0;
}

int kr_v4l2_lctl(kr_adapter_path *path, kr_patchset *patchset) {
  if (path == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("V4L2: Adapter path control");
  printk("V4L2: Adapter path controlled");
  return 0;
}

int kr_v4l2_unlink(kr_adapter_path *path) {
  if (path == NULL) return -1;
  printk("V4L2: unlink (not implemented)");
  return 0;
}

int kr_v4l2_link(kr_adapter *adapter, kr_adapter_path *path) {
  int ret;
  kr_v4l2 *kv;
  kr_event harness;
  if (adapter == NULL) return -1;
  kv = (kr_v4l2 *)adapter->handle;
  if (path == NULL) return -2;
  printk("V4L2: Adapter path create");
  kv->adapter_path = path;
  kv->path_info = &path->info->v4l2_in;
  ret = set_params(kv);
  if (ret != 0) return ret;
  ret = map_frames(kv);
  if (ret != 0) return ret;
  ret = capture_on(kv);
  if (ret != 0) return ret;
  harness.user = kv;
  harness.fd = kv->fd;
  harness.handler = device_event;
  harness.events = EPOLLIN;
  kr_loop_add(adapter->loop, &harness);
  harness.fd = path->fd;
  harness.handler = user_event;
  harness.events = EPOLLIN;
  kr_loop_add(adapter->loop, &harness);
  printk("V4L2: Adapter path created");
  return ret;
}

int kr_v4l2_ctl(kr_adapter *adapter, kr_patchset *patchset) {
  if (adapter == NULL) return -1;
  if (patchset == NULL) return -2;
  printk("V4L2: Adapter control");
  printk("V4L2: Adapter controlled");
  return 0;
}

int kr_v4l2_close(kr_adapter *adapter) {
  int ret;
  kr_v4l2 *kv;
  if (adapter == NULL) return -1;
  kv = (kr_v4l2 *)adapter->handle;
  if (kv == NULL) return -1;
  printk("V4L2: Adapter closing");
  teardown(kv);
  ret = kr_loop_destroy(adapter->loop);
  if (ret != 0) {
    printke("V4L2: trouble unlooping");
  }
  free(kv);
  adapter->handle = NULL;
  printk("V4L2: adapter closed");
  return 0;
}

int kr_v4l2_open(kr_adapter *adapter) {
  int ret;
  kr_v4l2 *kv;
  kr_loop_setup loop_setup;
  if (adapter == NULL) return -1;
  printk("V4L2: Adapter opening");
  adapter->handle = kr_allocz(1, sizeof(kr_v4l2));
  kv = (kr_v4l2 *)adapter->handle;
  kv->adapter = adapter;
  kv->info = &adapter->info->v4l2;
  ret = kv_init(kv);
  if (ret != 0) return ret;
  adapter->fd = kv->fd;
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "kr_v4l2_%d", kv->info->dev);
  adapter->loop = kr_loop_create(&loop_setup);
  printk("V4L2: adapter opened");
  return 0;
}
