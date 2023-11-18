#include <stdio.h>
#include <sys/eventfd.h>
#include <krad/app/debug.h>
#include <krad/image/image.h>
#include <krad/image/gif.h>
#include <krad/image/convert.h>
#include <krad/image/coder.h>
#include <krad/codec/video/vpx/vpx.h>
#include <krad/time/timer.h>
#include <krad/image/png.h>
#include <krad/mem/mem.h>
#include <krad/loop/loop.h>
#include <krad/io/file.h>
#include <krad/container/mkv/mkv.h>

static uint8_t *buffer = NULL;
static kr_data data;
static int ngifs;
static int curgif;

static char *allgifs[] = {
  "tumblr_lad3eouzez1qzt4vjo1_500.gif",
  "tumblr_li0bz2iwt51qzt4vjo1_500.gif",
  "tumblr_ljm3m8dfun1qzt4vjo1_500.gif",
  "dvdp-untitled-2.gif",
  "amVfE2n.gif",
  "198fMwT.gif",
  "6EFq3E3.gif",
  "2kkR6g6.gif",
  "5BRkPki.gif",
  "bklSj2l.gif",
  "Dvdp3.gif",
  "eTcZYDG.gif",
  "hOunFhS.gif",
  "K22mPQv.gif",
  "LX4NrAy.gif",
  "q6wm0Ah.gif",
  "wQAaOEX.gif",
  "zbf0A9f.gif",
  "w890jU3.gif",
};

char *get_a_gif_filename() {
  return allgifs[curgif];
}

kr_gif *get_a_gif() {
  int ret;
  int sz;
  kr_file *file;
  char filename[512];
  kr_gif *gif;
  ngifs = sizeof(allgifs) / sizeof(allgifs[0]);
  snprintf(filename, sizeof(filename), "%s/images/gif/%s",
   getenv("KRAD_TEST_DATA"), get_a_gif_filename());
  file = kr_file_open(filename);
  if (file == NULL) return NULL;
  if (buffer != NULL) {
    free(buffer);
  }
  sz = kr_file_size(file);
  buffer = kr_allocz(1, sz);
  //printk("Opened %s %zu bytes", filename, sz);
  ret = kr_file_read(file, buffer, sz);
  if (ret != sz) {
    printke("oh damn");
    return NULL;
  }
  //printk("Read %d bytes", ret);
  data.info.sz = sz;
  data.buf = buffer;
  kr_file_close(&file);
  gif = kr_gif_open(&data);
  return gif;
}

static int run_test() {
  char outfilename[1024];
  int ret;
  kr_frame gif_frame;
  kr_frame conv_frame;
  kr_frame enc_frame;
  kr_gif *gif;
  //kr_data data;
  kr_gif_info info;
  kr_vpx_enc *ve;
  kr_vpx_enc_setup setup;
  kr_convert conv;
  kr_mkv_t *webm;
  int16_t live;
  krad_transmitter_t *txmr;
  kr_timer *timer;
  //uint8_t *buffer;
  uint64_t clip_timecode;
  uint64_t running_timecode;
  uint64_t elapsed_time;
  int deadline;
  int track;
  clip_timecode = 0;
  running_timecode = 0;
  ret = 0;
  txmr = NULL;
  live = 9090;
  memset(&gif_frame, 0, sizeof(gif_frame));
  memset(&conv_frame, 0, sizeof(conv_frame));
  memset(&enc_frame, 0, sizeof(enc_frame));
  if (live != 0) {
    txmr = krad_transmitter_create();
    ret = krad_transmitter_listen_on(txmr, live);
    printk("txmr listen on %u ret %d", ret, live);
  }
  gif = get_a_gif();
  if (gif == NULL) {
    printke("reading gif");
    return -1;
  }
  ret = kr_gif_get_info(gif, &info);
  printk("%d x %d gif", info.w, info.h);
  gif_frame.image.info.w = info.w;
  gif_frame.image.info.h = info.h;
  gif_frame.image.ps[0] = info.w * 4;
  conv_frame.image.info.w = info.w;
  conv_frame.image.info.h = info.h;
  conv_frame.image.ps[0] = info.w + (info.w % 2);
  conv_frame.image.ps[1] = conv_frame.image.ps[0] / 2;
  conv_frame.image.ps[2] = conv_frame.image.ps[0] / 2;
  conv_frame.image.ps[3] = 0;
  gif_frame.image.info.fmt = KR_PIXELS_ARGB;
  conv_frame.image.info.fmt = KR_PIXELS_YUV420;
  //enc_frame = conv_frame;
  kr_image_pixels_alloc(&gif_frame.image);
  kr_image_pixels_alloc(&conv_frame.image);
  conv_frame.image.px[1] = conv_frame.image.px[0] + (conv_frame.image.ps[0] * ((conv_frame.image.info.h) + (conv_frame.image.info.h % 2)));
  conv_frame.image.px[2] = conv_frame.image.px[1] + (conv_frame.image.ps[1] * ((conv_frame.image.info.h / 2) + (conv_frame.image.info.h % 2)));
  kr_image_convert_init(&conv);
  if (txmr != NULL) {
    webm = kr_mkv_create_transmission(txmr, "giftv.webm", "video/webm");
  } else {
    snprintf(outfilename, sizeof(outfilename), "%s/%s.webm", getenv("HOME"), "sometestfile");
    webm = kr_mkv_create_file(outfilename);
  }
  setup.info.width = info.w;
  setup.info.height = info.h;
  setup.info.bitrate = 66 * 8;
  ve = kr_vpx_enc_create(&setup);
  track = kr_mkv_video_track(webm, VP8, info.w, info.h);
  timer = kr_timer_create();
  kr_timer_start(timer);
  for (;;) {
    ret = kr_gif_get_frame(gif, &gif_frame);
    if (ret != 1) {
      kr_gif_close(gif);
      running_timecode += clip_timecode;
      clip_timecode = 0;
      gif = get_a_gif();
      if (gif != NULL) continue;
      break;
    }
    ret = kr_image_convert(&conv, &conv_frame.image, &gif_frame.image);
    if (ret != 0) {
      break;
    }
    kr_frame_copytime(&conv_frame, &gif_frame);
    kr_frame_addtime(&conv_frame, running_timecode);
    elapsed_time = kr_timer_current_ms(timer);
    deadline = 0;
    if (conv_frame.tc > elapsed_time) {
      deadline = conv_frame.tc - elapsed_time;
    }
    deadline += conv_frame.td;
    if (deadline > 1000) {
      printk("napping");
      usleep(500 * 1000);
    }
    ret = kr_vpx_enc_ctl(ve, KR_VPX_DEADLINE, deadline);
    if (ret != 0) {
      printke("VPX enc ctl deadline %d ret: %d", deadline, ret);
      break;
    }
    ret = kr_vpx_enc_frame(ve, &enc_frame, &conv_frame);
    if (ret != 1) {
      printke("VPX enc ret: %d", ret);
      break;
    }
    kr_mkv_video_frame(webm, track, &enc_frame);
    clip_timecode = gif_frame.tc + gif_frame.td;
    if (elapsed_time % 15000 > 10000) {
      curgif++;
      curgif = curgif % ngifs;
    }
  }
  ret = kr_vpx_enc_frame(ve, &enc_frame, NULL);
  kr_gif_close(gif);
  free(buffer);
  kr_image_pixels_free(&gif_frame.image);
  kr_image_pixels_free(&conv_frame.image);
  kr_image_convert_clear(&conv);
  kr_vpx_enc_destroy(ve);
  kr_mkv_destroy(&webm);
  if (txmr != NULL) {
    krad_transmitter_destroy(txmr);
  }
  kr_timer_destroy(timer);
  return ret;
}

static int winning(kr_event *event) {
  uint64_t num;
  int efd;
  efd = *(int *)event->user;
  num = 666;
  write(efd, &num, sizeof(num));
  printk("Roar!");
  return 0;
}

static int loop_test() {
  uint64_t num;
  int efd;
  kr_loop *loop;
  kr_event event;
  kr_loop_setup loop_setup;
  kr_loop_setup_init(&loop_setup);
  snprintf(loop_setup.name, sizeof(loop_setup.name), "Someloop");
  loop = kr_loop_create(&loop_setup);
  if (loop) {
    efd = eventfd(0, EFD_CLOEXEC);
    event.fd = 0; /* stdin */
    event.user = &efd;
    event.events = EPOLLIN | EPOLLET;
    event.handler = winning;
    kr_loop_add(loop, &event);
    printk("Press enter to win.");
    read(efd, &num, sizeof(num));
    kr_loop_destroy(loop);
    printk("Winner! Number %"PRIu64"", num);
    return 0;
  }
  return -1;
}

int main (int argc, char *argv[]) {
  int ret;
  if ((argc == 2) && (atoi(argv[1]) == 666)) {
    ret = loop_test();
  } else {
    ret = run_test();
  }
  return ret;
}
