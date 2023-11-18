#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/io/file2.h>

/* mp4 */

typedef struct kr_mp4_demuxer_setup kr_mp4_demuxer_setup;
typedef struct kr_mp4_demuxer kr_mp4_demuxer;
typedef struct kr_mp4_header kr_mp4_header;
typedef struct kr_mp4_box kr_mp4_box;

struct kr_mp4_demuxer_setup {
  uint8_t *mem;
};

struct kr_mp4_box {
  char type[4];
  uint32_t sz;
};

struct kr_mp4_header {
  int ntracks;
};

struct kr_mp4_demuxer {
  kr_mp4_header header;
  int boxes;
};

size_t kr_mp4_demuxer_sizeof(void);
kr_mp4_demuxer *kr_mp4_demuxer_init(kr_mp4_demuxer_setup *setup);
int kr_mp4_start(kr_mp4_demuxer *d, kr_mp4_header *h, uint8_t *buf, size_t sz);
int kr_mp4_demux(kr_mp4_demuxer *d, kr_mp4_box *box, uint8_t *buf, size_t sz);

static void rmemcpy(void *dst, void *src, int len);

static void rmemcpy(void *dst, void *src, int len) {
  uint8_t *a_dst;
  uint8_t *a_src;
  int count;
  count = 0;
  len -= 1;
  a_dst = dst;
  a_src = src;
  while (len > -1) {
    a_dst[len--] = a_src[count++];
  }
}

size_t kr_mp4_demuxer_sizeof(void) {
  return sizeof(kr_mp4_demuxer);
}

kr_mp4_demuxer *kr_mp4_demuxer_init(kr_mp4_demuxer_setup *setup) {
  kr_mp4_demuxer *d;
  d = (kr_mp4_demuxer *)setup->mem;
  memset(d, 0, kr_mp4_demuxer_sizeof());
  d->boxes = 0;
  return d;
}

static size_t unbox(kr_mp4_box *box, uint8_t *buf) {
  memset(box, 0, sizeof(*box));
  rmemcpy(&box->sz, buf, 4);
  memcpy(box->type, buf + 4, 4);
  return box->sz;
}

static int boxeq(kr_mp4_box *box, const char *str) {
  return !memcmp(box->type, str, 4);
}

static void unpack_ftype(kr_mp4_box *b, uint8_t *buf) {
  char brand[4];
  int ver;
  int pos;
  pos = 0;
  memcpy(brand, buf, 4);
  rmemcpy(&ver, buf + 4, 4);
  printk("ftype: %.4s %d", brand, ver);
  for (pos = 8; pos < b->sz - 8; pos += 4) {
    printk(" compat: %.4s", buf + pos);
  }
}

static void unpack_boxinfo(uint8_t *ver, uint8_t *flags, uint8_t *buf) {
  *ver = 0;
  *flags = 0;
  *ver = buf[0];
  memcpy(flags, buf + 1, 3);
  printk("  version: %u", *ver);
}

static void unpack_mvhd(kr_mp4_box *b, uint8_t *buf) {
  //int ret;
  int pos;
  uint8_t ver;
  uint8_t flags[3];
  
  uint64_t ctime;
  uint64_t mtime;
  uint32_t tscale;
  uint64_t duration;
  ctime = 0;
  mtime = 0;
  tscale = 0;
  duration = 0;
  //kr_mp4_box box;
  pos = 0;
  printk(" Header: [%zu]", b->sz);
  unpack_boxinfo(&ver, flags, buf);
  pos += 4;
  if (ver == 0) {
    rmemcpy(&ctime, buf + pos, 4);
    pos += 4;
    rmemcpy(&mtime, buf + pos, 4);
    pos += 4;
    rmemcpy(&tscale, buf + pos, 4);
    pos += 4;
    rmemcpy(&duration, buf + pos, 4);
    pos += 4;
  }
  printk(" creation: %zu Last Mod: %zu Timescale: %u", ctime,
   mtime, tscale, duration);
  uint64_t mins;
  uint64_t secs;
  mins = duration / tscale / 60;
  secs = (duration / tscale) % 60;
  printk(" Duration: %zu:%zu", mins, secs);
}

static void unpack_trak(kr_mp4_box *b, uint8_t *buf) {
  return;
}

static void unpack_moov(kr_mp4_demuxer *d, kr_mp4_box *b, uint8_t *buf) {
  int ret;
  int pos;
  kr_mp4_box box;
  pos = 0;
  printk("Movie [%zu]", b->sz);
  while (pos < b->sz - 8) {
    ret = unbox(&box, buf + pos);
    if (ret < 1) return;
    if (boxeq(&box, "mvhd")) {
      unpack_mvhd(&box, buf + pos + 8);
    } else if (boxeq(&box, "trak")) {
      unpack_trak(&box, buf + pos + 8);
      d->header.ntracks++;
    } else {
      printk(" moov: %.4s [%zu]", box.type, box.sz);
    }
    pos += ret;
  }
}

int kr_mp4_start(kr_mp4_demuxer *d, kr_mp4_header *h, uint8_t *buf, size_t sz) {
  int ret;
  size_t pos;
  kr_mp4_box box;
  if (!d || !h || !buf) return -1;
  if (sz < 8) return 0;
  pos = 0;
  memset(h, 0, sizeof(*h));
  while (pos != sz) {
    ret = unbox(&box, buf + pos);
    if (ret < 1) break;
    if (boxeq(&box, "ftype")) {
      unpack_ftype(&box, buf + pos + 8);
    } else if (boxeq(&box, "moov")) {
      unpack_moov(d, &box, buf + pos + 8);
    } else {
      printk("Box: %.4s [%zu]", box.type, box.sz);
    }
    pos += box.sz;
  }
  *h = d->header;
  return pos;
}

int kr_mp4_demux(kr_mp4_demuxer *d, kr_mp4_box *box, uint8_t *buf, size_t sz) {
  if (!d || !box || !buf) return -1;
  if (sz < 8) return 0;
  unbox(box, buf);
  return box->sz;
}

/* test */

static int fs_event(kr_file_event *e) {
  return 0;
}

static void print_box(uint64_t num, kr_mp4_box *b) {
  printk("Box Type: %.4s Size: %zu", b->type, b->sz);
}

int demux(kr_file2 *f, kr_mp4_demuxer *d) {
  int ret;
  int nboxes;
  uint8_t *data;
  size_t pos;
  ssize_t res;
  kr_file2_info info;
  kr_mp4_box b;
  kr_mp4_header h;
  pos = 0;
  nboxes = 0;
  data = kr_file2_get_data(f);
  ret = kr_file2_get_info(f, &info);
  if (ret) {
    printke("getting file info");
    return -1;
  }
  res = kr_mp4_start(d, &h, data, info.sz);
  if (res < 1) return -1;
  pos += res;
  printk("Got header, %d tracks", h.ntracks);
  printk("Pos: %zu File Size: %zu", pos, info.sz);
  while (pos < info.sz) {
    res = kr_mp4_demux(d, &b, data + pos, info.sz - pos);
    if (res < 0) {
      printke("mp4 demux ret: %d", res);
      return -1;
    }
    if (res == 0) break;
    //printk("Got Block %d at pos %zu!", nboxess++, pos);
    print_box(++nboxes, &b);
    if (res > 0) pos += res;
  }
  return 0;
}

int run_test(char const *fpath) {
  int ret;
  uint8_t *mem;
  kr_mp4_demuxer_setup dsetup;
  kr_mp4_demuxer *d;
  kr_file2 *f;
  kr_file_set *fs;
  kr_fs_setup fsetup;
  mem = NULL;
  mem = kr_allocz(1, kr_mp4_demuxer_sizeof());
  dsetup.mem = mem; 
  d = kr_mp4_demuxer_init(&dsetup);
  fsetup.user = d;
  fsetup.event_cb = fs_event;
  fsetup.nfiles = 1;
  fs = kr_file_set_create(&fsetup);
  if (!fs) return -1;
  f = kr_file2_open(fs, (char *)fpath, strlen(fpath));
  if (!f) {
    printke("error kr_file_open");
    return -2;
  }
  ret = demux(f, d);
  if (f) {
    ret = kr_file2_close(f);
    if (ret) {
      printke("error kr_file_close");
      return -3;
    }
  }
  if (fs) {
    ret = kr_file_set_destroy(fs);
    if (ret) {
      printke("error kr_file_set_destroy");
      return -4;
    }
  }
  if (mem) free(mem);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 2) {
    fprintf(stderr, "Give me a mp4 file..\n");
    return -1;
  }
  ret = run_test(argv[1]);
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
