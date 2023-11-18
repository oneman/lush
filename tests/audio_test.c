#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <krad/app/debug.h>
#include <krad/audio/audio.h>
#include <krad/mem/mem.h>
#include <krad/io/file.h>
#include "util/tone.h"
#include <krad/audio/noise.h>
#include <krad/codec/audio/flac/encoder.h>

static int audio_test() {
  float db;
  float db2;
  float val;
  for (db = 0.0f; db > -144.0f; db -= 1.0f) {
    val = kr_dbtolin(db);
    db2 = kr_lintodb(val);
    printk("%gdb -> %f linear -> %gdb", db, val, db2);
  }
  for (val = -1.0f; val <= 1.0f; val += 0.1) {
    db = kr_lintodb(val);
    printk("%f linear -> %gdb", val, db);
  }
  /*for (val = 100.0f; val >= 0.0f; val -= 1.0f) {
    db = kr_percent_to_db(val);
    printk("%g percent -> %gdb", val, db);
  }*/
  return 0;
}

static void print_usage() {
  printk("");
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc != 1) {
    print_usage();
    return 1;
  }
  ret = audio_test();
  if (ret) {
    printk("Failure!");
  }
  return ret;
}
