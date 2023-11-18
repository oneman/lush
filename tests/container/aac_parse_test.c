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
#include <krad/container/mpeg/adts/parse.h>

static char *adts_profile_to_str(int p) {
  switch (p) {
    case ACC_MAIN: return "AAC Main";
    case AAC_LC: return "AAC LC";
    case AAC_SSR: return "AAC SSR";
    case AAC_LTP: return "AAC LTP";
    case AAC_SBR: return "AAC SBR";
    default: break;
  }
  return NULL;
}

static void adts_info_print(kr_adts_info *info, int br, int d) {
  printk("\n  MPEG-%d", info->version);
  printk("    Profile %s Sample Rate %dHz | Bit Rate (Avg) %dKb/s | Duration: %ds",
   adts_profile_to_str(info->profile), info->sample_rate, br, d);
}

int aac_parse_test() {
  ssize_t ret;
  uint8_t buf[2048];
  int br;
  int sr;
  int d;
  size_t len;
  size_t bytes;
  size_t frames;
  size_t frames_sz;
  kr_adts_info info;
  frames = 0;
  frames_sz = 0;
  bytes = 0;
  len = 0;
  sr = 0;
  for (;;) {
    ret = read(0, buf + len, sizeof(buf) - len);
    if (ret == 0) {
      printk("  Done reading input");
      break;
    }
    if (ret < 0) return -1;
    len += ret;
    bytes += ret;
    ret = kr_adts_parse(&info, buf, len);
    if (ret != 0) {
      printke("Parsing failed %d at %zu", ret, bytes - len + info.frames_sz);
      break;
    }
    if (info.frames) {
      sr = info.sample_rate;
      frames += info.frames;
      frames_sz += info.frames_sz;
      len -= info.frames_sz;
      memmove(buf, buf + info.frames_sz, len);
    }
  }
  br = 0;
  d = 0;
  if (sr && frames) {
    br = (frames_sz / (double)((double)(frames * 1024)
     / (double)sr)) / (double)125;
    d = frames * 1024 / sr;
  }
  if (frames) adts_info_print(&info, br, d);
  printk("  Correctly parsed %zu AAC frames.", frames);
  printk("  Total sz of parsed frames is %zu Bytes.\n", frames_sz);
  return 0;
}

void print_usage(char *cmd) {
  printk("Usage\n  cat in.aac | %s", cmd);
  printk("  find ~/Music/ -type f -name \"*.aac\" -print0 "
   "-exec bash -c \"cat \\\"{}\\\" | %s\" \\;", cmd);
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 1) {
    print_usage(argv[0]);
    return 1;
  }
  ret = aac_parse_test();
  if (ret != 0) {
    fprintf(stderr, "Failure %d\n", ret);
  }
  return 0;
}
