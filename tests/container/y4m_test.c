#include <errno.h>
#include <krad/app/debug.h>
#include <krad/mem/mem.h>
#include <krad/image/image.h>
#include <krad/container/y4m/unpack.h>

#define BUFSIZE 3840 * 2160 * 4

static int y4m_test(char *filename) {
  int err;
  size_t len;
  size_t image_sz;
  ssize_t ret;
  uint8_t mem[64];
  uint8_t *buf;
  kr_image_info image_info;
  ret = 0;
  len = 0; 
  kr_y4m_demuxer_setup setup;
  kr_y4m_demuxer *y;
  if (!filename) {
    printk("Y4M Test: Reading STDIN");
  } else {
    printk("Y4M Test: Reading %s", filename);
  }
  printk("kr_y4m_demuxer_sizeof() is %zu", kr_y4m_demuxer_sizeof());
  memset(mem, 0, sizeof(mem));
  //memset(buf, 0, sizeof(buf));
  buf = kr_allocz(1, BUFSIZE);
  memset(&setup, 0, sizeof(setup));
  setup.mem = mem;
  y = kr_y4m_demuxer_init(&setup);
  if (!y) {
    printke("Failed to init y4m demuxer");
    return -1;
  }
  for (;;) {
    if ((KR_Y4M_HEADER_SIZE_MAX - len) == 0) {
      printke("Failed to get Y4M Header");
      return -1;
    }
    ret = read(STDIN_FILENO, buf + len, KR_Y4M_HEADER_SIZE_MAX - len);
    if (ret < 0) {
      err = errno;
      printke("read() failed %s", strerror(err));
      return -1;
    }
    if (ret == 0) {
      usleep(1000);
      continue;
      //printk("read 0 bytes, EOF");
      return 0;
    }
    printk("Read %d", ret);
    len += ret;
    ret = kr_y4m_demuxer_read_header(y, buf, len);
    printk("kr_y4m_demuxer_read_header() ret %d", ret);
    if (ret > 0) {
      memmove(buf, buf + ret, len);
      len -= ret;
      break;
    }
  }
  /** END READ HEADER **/
  ret = kr_y4m_demuxer_get_image_info(y, &image_info);
  image_sz = kr_image_sizeof_pixels(&image_info);
  printk("Image size: %zu", image_sz);
  for (;;) {
    if ((KR_Y4M_HEADER_SIZE_MAX - len) == 0) {
      printke("Failed to get Y4M FRAME Header");
      return -1;
    }
    ret = read(STDIN_FILENO, buf + len, KR_Y4M_HEADER_SIZE_MAX - len);
    //ret = read(STDIN_FILENO, buf + len, BUFSIZE - len);
    if (ret < 0) {
      err = errno;
      printke("read() failed %s", strerror(err));
      return -1;
    }
    if (ret == 0) {
      usleep(1000);
      continue;
      //printk("read 0 bytes, EOF");
      return 0;
    }
    printk("Read %d", ret);
    len += ret;
    ret = kr_y4m_demuxer_read_frame_header(y, buf, len);
    printk("kr_y4m_demuxer_read_frame_header() ret %d", ret);
    if (ret > 0) {
      memmove(buf, buf + ret, len);
      len -= ret;
      break;
    }
  }
  /** END READ FRAME HEADER **/
  for (;;) {
    ret = read(STDIN_FILENO, buf + len, image_sz - len);
    if (ret < 0) {
      err = errno;
      printke("read() failed %s", strerror(err));
      return -1;
    }
    if (ret == 0) {
      usleep(1000);
      continue;
      //printk("read 0 bytes, EOF");
      return 0;
    }
    printk("Read %d", ret);
    len += ret;
    if (len == image_sz) break;
  }
  /** END READ FRAME **/
  printk("Got a frame");
  

  free(buf);
  return 0;
}

int main(int argc, char const *argv[]) {
  int ret;
  ret = 0;
  if (argc > 2) {
    fprintf(stderr, "Filename as argument or we read stdin\n");
    ret = 1;
  } else if (argc == 2) {
    ret = y4m_test((char *)argv[1]);
  } else if (argc == 1) {
    ret =  y4m_test(NULL);
  }
  if (ret) fprintf(stderr, "Failure.\n");
  return ret;
}
