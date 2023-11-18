#include <string.h>
#include <stdlib.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>

#include "buffer.h"

static ssize_t kr_io2_write(kr_io2_t *io);

kr_io2_t *kr_io2_create_size(size_t size) {
  kr_io2_t *io;
  io = kr_alloc(sizeof(kr_io2_t));
  io->size = size;
  io->buffer = kr_alloc(io->size);
  io->fd = -1;
  io->user = NULL;
  kr_io2_restart(io);
  return io;
}

kr_io2_t *kr_io2_create(void) {
  return kr_io2_create_size(KR_IO2_BUF_SZ);
}

int kr_io2_set_io(kr_io2_t *io, kr_io_cb *rd, kr_io_cb *wr, void *user) {
  if (!io || !rd || !wr || !user) return -1;
  io->rd = rd;
  io->wr = wr;
  io->user = user;
  return 0;
}

int kr_io2_destroy(kr_io2_t **io) {
  if ((io != NULL) && (*io != NULL)) {
    free((*io)->buffer);
    free(*io);
    *io = NULL;
    return 0;
  }
  return -1;
}

int kr_io2_set_fd(kr_io2_t *io, int fd) {
  if (!io) return -1;
  if (fd < 0) {
    return -1;
  }
  io->fd = fd;
  return 0;
}

ssize_t kr_io2_buffered_bytes(kr_io2_t *io) {
  if (!io) return -1;
  return io->len;
}

int kr_io2_want_out(kr_io2_t *io) {
  if (!io) return -1;
  if (io->len > 0) {
    return 1;
  }
  return 0;
}

int kr_io2_has_in(kr_io2_t *io) {
  if (!io) return -1;
  if (io->len > 0) {
    return 1;
  }
  return 0;
}

void kr_io2_advance(kr_io2_t *io, size_t bytes) {
  io->len += bytes;
  io->space -= bytes;
  io->buf += bytes;
}

int kr_io2_pack(kr_io2_t *io, void *buffer, size_t len) {
  if (len > io->space) {
    printk("io2_pack len: %zu buf len: %zu new len: %zu size: %zu space: %zu",
     len, io->len, len + io->len, io->size, io->space);
    printke("buffer overpack cancelling!");
    return -1;
  }
  memcpy(io->buf, buffer, len);
  kr_io2_advance(io, len);
  return len;
}

int kr_io2_unpack(kr_io2_t *io, void *buffer, size_t len) {
  // memcpy len from the buffer, then defere to pulled
  if (len > io->len) len = io->len;
  memcpy(buffer, io->rd_buf, len);
  kr_io2_pulled(io, len);
  return len;
}

int kr_io2_unpeek(kr_io2_t *io, void *buffer, size_t len) {
  if (len > io->len) len = io->len;
  memcpy(buffer, io->rd_buf, len);
  return len;
}

static ssize_t kr_io2_write(kr_io2_t *io) {
  if (io->user) {
    return io->wr(io->user, io->wr_buf, io->len);
  } else {
    return write(io->fd, io->wr_buf, io->len);
  }
}

int kr_io2_restart(kr_io2_t *io) {
  io->buf = io->buffer;
  io->rd_buf = io->buffer;
  io->wr_buf = io->buffer;
  io->len = 0;
  io->space = io->size;
  return 0;
}

int kr_io2_sync(kr_io2_t *io) {
  // FILES ONLY blocking or failing!
  ssize_t ret;
  if (!kr_io2_want_out(io)) {
    return 0;
  }
  ret = kr_io2_write(io);
  if (ret != io->len) {
    printk("Could not write all we wanted to: %zd of %zu", ret, io->len);
    printke("Failing here a, this should not happen as a file only func");
    exit(77);
  }
  kr_io2_restart(io);
  return 0;
}

int kr_io2_output(kr_io2_t *io, ssize_t *bytes_written) {
  ssize_t ret;
  if (!io || !bytes_written) return -1;
  *bytes_written = 0;
  if (!kr_io2_want_out(io)) {
    return 0;
  }
  ret = kr_io2_write(io);
  *bytes_written += ret;
  if (ret != io->len) {
    if (ret < 1) {
      printke("its bad we should certainly fail, ret %d len %zu", ret,
       io->len);
      return ret;
    } else {
      io->len -= ret;
      io->wr_buf += ret;
    }
    return 0;
  }
  kr_io2_restart(io);
  return 0;
}

void kr_io2_pulled(kr_io2_t *io, size_t bytes) {
  io->len -= bytes;
  io->rd_buf += bytes;
  if (io->len == 0) {
    kr_io2_restart(io);
  }
}

size_t kr_io2_read(kr_io2_t *io) {
  size_t ret;
  size_t len;
  len = 0;
  if (io->len > 0 && io->len < io->size) {
    memmove(io->buffer, io->rd_buf, io->len);
    io->space = io->size - io->len;
    io->buf = io->buffer + io->len;
    io->rd_buf = io->buffer;
    io->wr_buf = io->buffer;
    len = io->len;
  }
  if (io->user) {
    ret = io->rd(io->user, io->buf, io->space);
  } else {
    ret = read(io->fd, io->buf, io->space);
  }
  if (ret > 0) {
    kr_io2_advance(io, ret);
  } else {
    kr_io2_restart(io);
    return ret;
  }
  return ret + len;
}
