#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <inttypes.h>
#include <alloca.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/playlist/m3u.h>
#include <krad/tuner/hls_tuner.h>
#include <krad/io/file.h>

int m3u_test(char *filename) {
  char text[8192];
  kr_m3u_info *info;
  kr_hls_playlist plst;
  kr_file *file;
  int64_t fsz;
  char *buffer;
  ssize_t read_sz;
  int ret;
  if (!file_exists(filename)) {
    fprintf(stderr, "File does not exists.\n");
    return -1;
  }
  fsz = file_size(filename);
  if (fsz <= 0) return -1;
  file = kr_file_open(filename);
  if (file == NULL) return -1;
  buffer = kr_alloc(fsz);
  info = kr_allocz(1, sizeof(kr_m3u_info));
  read_sz = kr_file_read(file, buffer, fsz);
  if (read_sz != fsz) return -1;
  ret = kr_m3u_parse(info, buffer, fsz);
  if (ret != 0) {
    fprintf(stderr, "m3u parsing failed.\n");
    return -1;
  }
  memset(&plst, 0, sizeof(plst));

/*  if (kr_m3u_info_to_text(text, info, sizeof(text)) != sizeof(text)) {
    printk("%s", text);
  }
*/
  if (m3u_to_hls(&plst, info) != 0) return -1;
  if (kr_hls_playlist_to_text(text, &plst, sizeof(text)) != sizeof(text)) {
    printk("%s", text);
  }
  kr_file_close(&file);
  free(buffer);
  return 0;
}

void print_usage() {
  printk("Usage\n  ./m3u_test playlist.m3u");
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    print_usage();
    return 1;
  }
  if (m3u_test(argv[1]) < 0) {
    fprintf(stderr, "test failed.\n");
  }
  return 0;
}
