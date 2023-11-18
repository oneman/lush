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
#include <krad/mem/mem.h>
#include <krad/container/ogg/unpack.h>
#include <krad/container/ogg/pack.h>

static int unpack_pack(kr_ogg_unpacker *u, uint8_t *buf, size_t sz,
 kr_codec_packet *pkt, int out) {
  int ret;
  int eos;
  kr_ogg_pkt_info info;
  kr_ogg_muxer *ogg;
  kr_ogg_muxer_setup ogg_setup;
  kr_ogg_muxer_data mdata;
  uint8_t codec_hdr0[512];
  size_t codec_hdr0_sz;
  uint8_t *container;
  ogg_setup.mem = kr_alloc(kr_ogg_muxer_sizeof());
  codec_hdr0_sz = 0;
  ogg_setup.tracks = 1;
  container = kr_alloc(512);
  printf("UnpackPack starting\n");
  kr_ogg_unpacker_resync(u);
  eos = 0;
  ogg = NULL;
  for (;;) {
    ret = kr_ogg_unpack(u, &info, buf, sz);
    if (ret < 0) return -1;
    if (ret == 1) eos = 1;
    printf("  pkt num %"PRIu64", sz %zu, track %u, hdr %u\n",
     info.num, info.sz, info.track, info.hdr);
    if (info.truncated > 0) {
      printf("  pkt num %zu of track %u appears to be truncated, %zu bytes not processed\n",
       info.num, info.track, info.truncated);
      break;
    }
    if (info.num == 0) {
      memcpy(codec_hdr0, pkt->buf, pkt->len);
      codec_hdr0_sz = pkt->len;
    }
    if (info.num == 1) {
      ogg_setup.header[0].len = 2;
      ogg_setup.header[0].data[0].buf = codec_hdr0;
      ogg_setup.header[0].data[0].sz = codec_hdr0_sz;
      ogg_setup.header[0].data[1].buf = pkt->buf;
      ogg_setup.header[0].data[1].sz = pkt->len;
      ogg = kr_ogg_muxer_init(&ogg_setup);
      if (!ogg) return -45;
      ret = kr_ogg_muxer_read_header(ogg, container, 512);
      if (ret <= 0) return -1;
      ret = write(out, container, ret);
    }
    if (info.num > 1) {
      memset(&mdata, 0, sizeof(mdata));
      mdata.framing_buf = container;
      mdata.sz = 512;
      mdata.track = 0;
      mdata.in[0].granule_pos = pkt->tc;
      mdata.in[0].buf = pkt->buf;
      mdata.in[0].sz = pkt->len;
      mdata.in_len = 1;
      ret = kr_ogg_mux(ogg, &mdata);
      if (ret < 1) {
        printf("Mux: Ogg muxer %d\n", ret);
        return -1;
      }
      printf("Nice muxed %zu cnt and %zu codec!!\n", mdata.out[0].sz, mdata.out[1].sz);
      ret = write(out, mdata.out[0].buf, mdata.out[0].sz);
      ret = write(out, mdata.out[1].buf, mdata.out[1].sz);
    }
    if (eos) break;
  }
  close(out);
  free(container);
  free(ogg_setup.mem);
  return 0;
}

int ogg_test(char const *in_path, char const *out_path) {
  int in_fd;
  int out_fd;
  int status;
  size_t size;
  struct stat s;
  uint8_t *mapped;
  ssize_t ret;
  kr_ogg_unpacker_setup setup;
  kr_ogg_unpacker *u;
  kr_codec_packet pkt;
  out_fd = open(out_path, O_CREAT | O_EXCL | O_RDWR, 0640);
  if (out_fd < 0) return -1;
  in_fd = open(in_path, O_RDONLY);
  if (in_fd < 0) return -1;
  status = fstat(in_fd, &s);
  if (status < 0) return -1;
  size = s.st_size;
  mapped = mmap(NULL, size, PROT_READ, MAP_PRIVATE | MAP_NONBLOCK, in_fd, 0);
  if (mapped == MAP_FAILED) {
    fprintf(stderr, "File: mmap %s\n", strerror(errno));
    close(out_fd);
    close(in_fd);
    return -1;
  }
  pkt.sz = 8192 * 8;
  pkt.buf = kr_alloc(pkt.sz);
  pkt.len = 0;
  setup.mem = kr_alloc(kr_ogg_unpacker_sizeof());
  u = kr_ogg_unpacker_init(&setup);
  if (!u) return -1;
  kr_ogg_unpacker_output_set(u, &pkt, 1);
  ret = unpack_pack(u, mapped, size, &pkt, out_fd);
  if (ret != 0) return ret;
  munmap(mapped, size);
  close(out_fd);
  close(in_fd);
  free(pkt.buf);
  free(setup.mem);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  if (argc != 3) {
    fprintf(stderr, "Usage:\n %s in.ogg out.ogg\n", argv[0]);
    return -1;
  }
  ret = ogg_test(argv[1], argv[2]);
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
