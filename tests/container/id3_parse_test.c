#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/container/mpeg/mpga/id3.h>

int id3_parse_test() {
  int ret;
  uint8_t mem[64];
  uint8_t buf[10];
  kr_id3_parser *p;
  kr_id3_info info;
  kr_id3_parser_setup setup;
  if (sizeof(mem) < kr_id3_parser_sizeof()) return -1;
  setup.mem = mem;
  p = kr_id3_parser_init(&setup);
  if (!p) return -2;
  ret = read(0, buf, sizeof(buf));
  if (ret != sizeof(buf)) return -3;
  ret = kr_id3_parse(p, buf, sizeof(buf));
  if (ret == -3) {
    printk("  No ID3 found");
    return 0;
  }
  if (ret != 0) {
    printke("kr_id3_parse failed %d", ret);
    return -4;
  }
  kr_id3_parser_get_info(p, &info);
  printk("\n  ID3 sz: %zu B\n", info.sz);
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
  ret = id3_parse_test();
  if (ret != 0) {
    fprintf(stderr, "Failure %d\n", ret);
  }
  return 0;
}
