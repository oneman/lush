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
#include <krad/container/ogg/pack.h>
 
#define BUFSIZE 8192 * 1024

static int ogg_mux_test() {
  ssize_t ret;
  uint8_t *framing_buf;
  uint8_t *packet_buf;
  kr_ogg_muxer *ogg;
  kr_ogg_muxer_setup ogg_setup;
  kr_ogg_muxer_data od;
  framing_buf = kr_alloc(BUFSIZE);
  packet_buf = kr_allocz(1, BUFSIZE);
  ogg_setup.mem = kr_alloc(kr_ogg_muxer_sizeof());
  ogg_setup.tracks = 1;
  ogg_setup.header[0].data[0].buf = (uint8_t *)"ROAR";
  ogg_setup.header[0].data[0].sz = 4;
  ogg_setup.header[0].len = 1;
  ogg = kr_ogg_muxer_init(&ogg_setup);
  if (!ogg) return -45;
  memset(&od, 0, sizeof(od));
  od.framing_buf = framing_buf;
  od.sz = BUFSIZE;
  od.track = 0;

  od.in_len = 0;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 1;
  od.in[0].buf = packet_buf;
  od.in_len = 1;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 255;
  od.in[0].buf = packet_buf;
  od.in_len = 1;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 256;
  od.in[0].buf = packet_buf;
  od.in_len = 1;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 1;
  od.in[0].buf = packet_buf;
  od.in[1].sz = 1;
  od.in[1].buf = packet_buf;
  od.in_len = 2;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");
  
  od.in[0].sz = 1;
  od.in[0].buf = packet_buf;
  od.in[1].sz = 1;
  od.in[1].buf = packet_buf;
  od.in[2].sz = 1;
  od.in[2].buf = packet_buf;
  od.in_len = 3;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 111111;
  od.in[0].buf = packet_buf;
  od.in_len = 1;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");

  od.in[0].sz = 1111111;
  od.in[0].buf = packet_buf;
  od.in[1].sz = 1;
  od.in[1].buf = packet_buf;
  od.in_len = 2;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");
  
  od.in[0].sz = 1;
  od.in[0].buf = packet_buf;
  od.in[1].sz = 111111;
  od.in[1].buf = packet_buf;
  od.in[2].sz = 1;
  od.in[2].buf = packet_buf;
  od.in_len = 3;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");
  
  od.in[0].sz = 1;
  od.in[0].buf = packet_buf;
  od.in[1].sz = 65025;
  od.in[1].buf = packet_buf;
  od.in[2].sz = 1;
  od.in[2].buf = packet_buf;
  od.in_len = 3;
  ret = kr_ogg_mux(ogg, &od);
  printk(" mux ret %d with %d output vec len", ret, od.out_len);
  printk("\n");
  
  free(framing_buf);
  free(packet_buf);
  free(ogg_setup.mem);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = ogg_mux_test();
  if (ret) printke("ogg_mux_test() ret %d", ret);
  return ret;
}
