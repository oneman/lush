#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/epoll.h>
#include <krad/ring/ring.h>
#include <krad/app/debug.h>
#include <krad/app/notice.h>
#include <krad/mem/mem.h>
#include "queue.h"

struct kr_queue_producer {
  int user;
  int sz;
  kr_ringbuf *ring;
  int fd;
};

struct kr_queue {
  int nproducers;
  int fd;
  kr_queue_producer producer[KR_QUEUE_PRODUCERS_MAX];
};

#define KR_QUEUE_READ_NEVENTS 1

int kr_queue_read(kr_queue *q, kr_queue_item *item) {
  int err;
  int ret;
  int ms;
  kr_queue_producer *p;
  struct epoll_event events[KR_QUEUE_READ_NEVENTS];
  if (!q || !item) return -1;
  ms = 0;
  ret = epoll_wait(q->fd, events, KR_QUEUE_READ_NEVENTS, ms);
  if (ret == KR_QUEUE_READ_NEVENTS) {
    p = (kr_queue_producer *)events[0].data.ptr;
    item->user = p->user;
    if (krad_ringbuffer_read_space(p->ring) < 8) {
      printke("Queue read space less than 8");
      return 0;
    }
    //printk("%p reading", p);
    ret = krad_ringbuffer_read(p->ring, (void *)&item->ptr, sizeof(item->ptr));
    //printk("read %d bytes", ret);
    if (ret != sizeof(item->ptr)) return -2;
    if (krad_ringbuffer_read_space(p->ring) == 0) {
      ret = kr_take_notice(p->fd);
    }
    return KR_QUEUE_READ_NEVENTS;
  }
  if (ret < 0 || ret > KR_QUEUE_READ_NEVENTS) {
    err = errno;
    printke("Queue: error on epoll wait %s", strerror(err));
    return -1;
  }
  return 0;
}

int kr_queue_write(kr_queue_producer *p, kr_queue_item *item) {
  int ret;
  if (!p || !item) return -1;
  //printk("%p writing %p", p, item->ptr);
  if (krad_ringbuffer_write_space(p->ring) < 8) {
    printke("Queue out of write space");
    return 0;
  }
  ret = krad_ringbuffer_write(p->ring, (void *)&item->ptr, sizeof(item->ptr));
  //printk("wrote %d bytes", ret);
  ret = kr_give_notice(p->fd);
  return ret;
}

kr_queue_producer *kr_queue_get_producer(kr_queue *q, int user) {
  int i;
  if (!q) return NULL;
  for (i = 0; i < q->nproducers; i++) {
    if (q->producer[i].user == user) return &q->producer[i];
  }
  return NULL;
}

int kr_queue_get_fd(kr_queue *q) {
  if (!q) return -1;
  return q->fd;
}

int kr_queue_destroy(kr_queue *q) {
  int i;
  kr_queue_producer *p;
  if (!q) return -1;
  for (i = 0; i < q->nproducers; i++) {
    p = &q->producer[i];
    if (!p->user) break;
    if (p->fd) kr_notifier_destroy(p->fd);
    if (p->ring) krad_ringbuffer_free(p->ring);
  }
  if (q->fd != -1) close(q->fd);
  free(q);
  return 0;
}

kr_queue *kr_queue_create(kr_queue_setup *setup) {
  int i;
  int ret;
  int fail;
  kr_queue *q;
  kr_queue_producer *p;
  kr_queue_producer_setup *ps;
  struct epoll_event ev;
  fail = 0;
  if (!setup) return NULL;
  if (setup->nproducers < 0) return NULL;
  if (setup->nproducers > KR_QUEUE_PRODUCERS_MAX) return NULL;
  q = (kr_queue *)kr_allocz(1, sizeof(*q));
  if (!q) return NULL;
  q->fd = epoll_create1(EPOLL_CLOEXEC);
  if (q->fd == -1) {
    printke("Queue: epoll_create fail");
    fail = 1;
  }
  if (!fail) {
    for (i = 0; i < setup->nproducers; i++) {
      ps = &setup->producer[i];
      p = &q->producer[i];
      if (!ps->user) break;
      if (ps->sz < 1 || ps->sz > 4096) {
        fail = 1;
        break;
      }
      p->user = ps->user;
      p->sz = ps->sz;
      q->nproducers++;
    }
  }
  if (q->nproducers < 1) fail = 1;
  if (!fail) {
    for (i = 0; i < q->nproducers; i++) {
      p = &q->producer[i];
      p->fd = kr_notifier_create();
      if (p->fd < 1) {
        fail = 1;
        break;
      }
      memset(&ev, 0, sizeof(ev));
      ev.events = EPOLLIN;
      ev.data.ptr = p;
      ret = epoll_ctl(q->fd, EPOLL_CTL_ADD, p->fd, &ev);
      if (ret != 0) {
        printke("Queue: epoll ctl add producer fd fail");
        fail = 1;
        break;
      }
      p->ring = krad_ringbuffer_create(p->sz * sizeof(void *));
      if (!p->ring) {
        fail = 1;
        break;
      }
    }
  }
  if (fail) {
    kr_queue_destroy(q);
    return NULL;
  }
  return q;
}
