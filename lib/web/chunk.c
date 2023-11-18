#include "chunk.h"
#include "math.h"

static int find_trailer(uint8_t *buf, size_t sz) {
  int i;
  if (sz < KR_CHUNK_TRAILER_LEN) return -1;
  for (i = 0; i < sz - 1; i++) {
    if (buf[i] == '\r' && buf[i + 1] == '\n') {
      return i;
    }
  }
  return -1;
}

static int parse_header(kr_chunked_tracker *ct, kr_iob *in) {
  int trailer_pos;
  //printk("%.*s", KR_CHUNK_TRAILER_LEN, in->rd_buf);
  printkd("Parsing chunked header");
  trailer_pos = find_trailer(in->rd_buf, in->len);
  if (trailer_pos > 0) {
    //printk("trailer_pos %d", trailer_pos);
    if (trailer_pos > KR_CHUNK_HDR_MAX_LEN) return -1;
    ct->len = strtol((char *)in->rd_buf, NULL, 16);
    printkd("great, chunk of size %d", ct->len);
    kr_io2_pulled(in, trailer_pos + KR_CHUNK_TRAILER_LEN);
    if (ct->len == 0) {
      if (in->len == KR_CHUNK_TRAILER_LEN &&
       !find_trailer(in->rd_buf, KR_CHUNK_TRAILER_LEN)) {
        ct->state = KR_CHUNKED_DONE;
        kr_io2_pulled(in, KR_CHUNK_TRAILER_LEN);
        printkd("done dechunking!");
        return -1;
      } else {
        printke("end of chunked message not found");
        return -1;
      }
    }
  } else {
    //printk("in->len %d", in->len);
    if (in->len > KR_CHUNK_HDR_MAX_LEN) return -1;
    printkd("partial header, not bothering");
    ct->state = KR_CHUNKED_INIT;
    return 1;
  }
  return 0;
}

static int dechunker(kr_chunked_tracker *ct, kr_iob *in,
 void *buffer, size_t len) {
  int ret;
  int tot;
  tot = 0;
  printkd("dechunking...");
  if (in->len == 0) return 0;
  //if (len < in->len) return -1;
  while (tot < len) {
    if (ct->state == KR_CHUNKED_INIT) {
      ret = parse_header(ct, in);
      if (ct->state == KR_CHUNKED_DONE) return tot;
      if (ret == 0) ct->state = KR_CHUNKED_CHUNK;
      if (ret > 0) break;
      if (ret < 0) return -1;
      if (ct->len < len) len = ct->len;
      ret = kr_io2_unpack(in, buffer + tot, len);
      //printk("unpack ret %d", ret);
      if (ret < 0) return -1;
      tot += ret;
      ct->len -= ret;
      if (ct->len == 0) {
        if (in->len < KR_CHUNK_TRAILER_LEN) break;
        ret = find_trailer(in->rd_buf, in->len);
        if (ret == 0) {
          printkd("end of chunk");
          ct->state = KR_CHUNKED_INIT;
          kr_io2_pulled(in, KR_CHUNK_TRAILER_LEN);
        } else {
          printke("no trailer after chunk ended!");
          return -1;
        }
      }
      if (in->len == 0) break;
    } else if (ct->state == KR_CHUNKED_CHUNK) {
      printkd("in partial chunk %d", ct->len);
      if (ct->len < len) len = ct->len;
      ret = kr_io2_unpack(in, buffer + tot, len);
      //printk("unpack ret %d", ret);
      if (ret < 0) return -1;
      tot += ret;
      ct->len -= ret;
      if (ct->len == 0) {
        if (in->len < KR_CHUNK_TRAILER_LEN) break;
        ret = find_trailer(in->rd_buf, in->len);
        if (ret == 0) {
          printkd("end of chunk");
          ct->state = KR_CHUNKED_INIT;
          kr_io2_pulled(in, KR_CHUNK_TRAILER_LEN);
        } else {
          printke("no trailer after chunk ended!");
          return -1;
        }
      }
      if (in->len == 0) break;
    } else {
      printke("dechunking done already :/");
      break;
    }
  }
  return tot;
}

static ssize_t chunker(kr_chunked_tracker *ct, kr_iob *out,
 void *buffer, size_t len) {
  ssize_t ret;
  size_t tot;
  char header[24];
  ct->len = len;
  ret = snprintf(header, sizeof(header), "%x%s",
   (uint32_t)ct->len, KR_CHUNK_TRAILER);
  if (ret >= sizeof(header)) return -1;
  ct->header_len = ret;
  tot = ct->len + ct->header_len + KR_CHUNK_TRAILER_LEN;
  if (out->space < (tot + KR_CHUNKING_TRAILER_LEN) ) {
    ct->len = 0;
    ct->header_len = 0;
    return -1;
  }
  ret = kr_io2_pack(out, header, ct->header_len);
  if (ret < 0) return -1;
  ret = kr_io2_pack(out, buffer, ct->len);
  if (ret < 0) return -1;
  ret = kr_io2_pack(out, KR_CHUNK_TRAILER, KR_CHUNK_TRAILER_LEN);
  if (ret < 0) return -1;
  return tot;
}
