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
#include <krad/container/mkv/unpack.h>
#include <krad/io/file2.h>

static int fs_event(kr_file_event *e) {
  return 0;
}

static void print_block(uint64_t num, kr_mkv_block *b) {
  printk("Block %zu: Track: %u Size: %zu Timecode: %zu Keyframe: %0x Flags: %0x",
   num, b->track, b->sz, b->tc, b->keyframe, b->flags);
}

int demux(kr_file2 *f, kr_mkv_demuxer *d) {
  int ret;
  int nblocks;
  uint8_t *data;
  size_t pos;
  ssize_t res;
  kr_file2_info info;
  kr_mkv_block b;
  kr_mkv_header h;
  pos = 0;
  nblocks = 0;
  data = kr_file2_get_data(f);
  ret = kr_file2_get_info(f, &info);
  if (ret) {
    printke("getting file info");
    return -1;
  }
  res = kr_mkv_start(d, &h, data, info.sz);
  if (res < 1) return -1;
  pos += res;
  printk("\n!!!! Got header, %d tracks\n", h.ntracks);
  while (pos < info.sz) {
    res = kr_mkv_demux(d, &b, data + pos, info.sz - pos);
    if (res < 0) {
      printke("mkv demux ret: %d", res);
      return -1;
    }
    if (res == 0) break;
    //printk("Got Block %d at pos %zu!", nblocks++, pos);
    print_block(++nblocks, &b);
    if (res > 0) pos += res;
  }
  return 0;
}

int run_test(char const *fpath) {
  int ret;
  uint8_t *mem;
  kr_mkv_demuxer_setup dsetup;
  kr_mkv_demuxer *d;
  kr_file2 *f;
  kr_file_set *fs;
  kr_fs_setup fsetup;
  mem = NULL;
  mem = kr_allocz(1, kr_mkv_demuxer_sizeof());
  dsetup.mem = mem; 
  d = kr_mkv_demuxer_init(&dsetup);
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
    fprintf(stderr, "Give me a mkv file..\n");
    return -1;
  }
  ret = run_test(argv[1]);
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
