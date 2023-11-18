#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/container/mpeg/mpga/id3.h>
#include <krad/container/mpeg/mpga/parse.h>

#define MAX_SKIPS 32

static void mpga_info_print(kr_mpga_info *info, int br) {
  if (info->version == 3) {
    printk("\n  MPEG-2.5");
  } else {
    printk("\n  MPEG-%d", info->version);
  }
  printk("    Layer %d | Channels %d | Sample Rate %dHz | Bit Rate (Avg) %dKb/s",
   info->layer, info->channels, info->sample_rate, br);
}

static ssize_t mp3_get_id3_sz(uint8_t *buf, size_t sz) {
  int ret;
  uint8_t mem[64];
  kr_id3_parser *p;
  kr_id3_info info;
  kr_id3_parser_setup setup;
  if (sizeof(mem) < kr_id3_parser_sizeof()) return -1;
  setup.mem = mem;
  p = kr_id3_parser_init(&setup);
  if (!p) return -2;
  ret = kr_id3_parse(p, buf, sz);
  if (ret == -3) return 0;
  if (ret != 0) return -1;
  kr_id3_parser_get_info(p, &info);
  return info.sz;
}

int mp3_parse_test() {
  size_t id3_sz;
  size_t len;
  ssize_t ret;
  uint8_t buf[2048];
  size_t frames;
  size_t frames_sz;
  size_t bytes;
  size_t rd;
  size_t skipped;
  size_t at;
  int br;
  int sr;
  int first;
  int skips;
  kr_mpga_info info;
  frames = 0;
  frames_sz = 0;
  len = 0;
  bytes = 0;
  sr = 0;
  skipped = 0;
  at = 0;
  first = 1;
  skips = 0;
  memset(&info, 0, sizeof(info));
  /* parse ID3 headers if present */
  for (;;) {
    ret = read(0, buf + len, sizeof(buf) - len);
    if (ret <= 0) return -1;
    len += ret;
    bytes += ret;
    ret = mp3_get_id3_sz(buf, len);
    if (ret <= 0) break;
    printk("\n  Read %zu Bytes of ID3 data at %zu.", ret, bytes - len);
    id3_sz = ret;
    if (id3_sz < len) {
      len -= id3_sz;
      memmove(buf, buf + id3_sz, len);
    } else {
      id3_sz -= len;
      while (id3_sz > 0) {
        len = sizeof(buf);
        if (id3_sz < sizeof(buf)) len = id3_sz;
        ret = read(0, buf, len);
        if (ret <= 0) return -2;
        bytes += ret;
        id3_sz -= ret;
      }
      len = 0;
    }
  }
  printk("\n  No more ID3 headers found.");
  /* move on to parse actual mp3 data */
  for (;;) {
    if (!first) {
      if (skips >= MAX_SKIPS) {
        printke("   Too much garbage, probably not valid/corrupted MP3 data.");
        return 1;
      }
      ret = read(0, buf + len, sizeof(buf) - len);
      if (ret < 0) return -3;
      if (ret == 0) {
        if (skipped) printk("Skipped %zu Bytes of junk at %zu", skipped, bytes);
        printk("  Done reading input at %zu", bytes);
        break;
      }
      bytes += ret;
      len += ret;
    } else first = 0;
    at = bytes - len;
    ret = kr_mpga_scan(buf, len, &rd);
    if (ret < 0) {
      len -= rd;
      skipped += rd;
      memmove(buf, buf + rd, len);
      continue;
    } else {
      len -= ret;
      if (ret) memmove(buf, buf + ret, len);
      skipped = ret;
    }
    if (skipped) {
      printk("  Skipped %zu Bytes of junk at %zu", skipped, at);
      skipped = 0;
      ++skips;
    }
    ret = kr_mpga_parse(&info, buf, len);
    if (ret == 0 || info.frames) {
      if (info.layer != 3 || info.version != 1) {
        if (len < 2) {
          printke("Done reading input at %zu.", bytes);
          break;
        }
        len -= 2;
        memmove(buf, buf + 2, len);
        continue;
      }
      if (info.frames) sr = info.sample_rate;
      frames += info.frames;
      frames_sz += info.frames_sz;
      if (len < info.frames_sz) {
        printke("This should not have happened.");
        exit(-1);
      }
      len -= info.frames_sz;
      memmove(buf, buf + info.frames_sz, len);
    } else {
      if (len < 2) {
        printke("Done reading input at %zu.", bytes);
        break;
      }
      len -= 2;
      memmove(buf, buf + 2, len);
    }
  }
  br = 0;
  if (sr && frames) {
    br = (frames_sz / (double)((double)(frames * 1152)
     / (double)sr)) / (double)125;
  }
  if (frames) mpga_info_print(&info, br);
  printk("  Correctly parsed %zu MP3 frames.", frames);
  printk("  Total sz of parsed frames is %zu Bytes.\n", frames_sz);
  return 0;
}

void print_usage(char *cmd) {
  printk("Usage\n  cat in.mp3 | %s", cmd);
  printk("  find ~/Music/ -type f -name \"*.mp3\" -print0 "
   "-exec bash -c \"cat \\\"{}\\\" | %s\" \\;", cmd);
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 1) {
    print_usage(argv[0]);
    return 1;
  }
  ret = mp3_parse_test();
  if (ret != 0) {
    fprintf(stderr, "Failure %d\n", ret);
  }
  return 0;
}
