#if !defined(_app_queue_H)
# define _app_queue_H (1)

#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>

#define KR_QUEUE_PRODUCERS_MAX 8

typedef struct kr_queue kr_queue;
typedef struct kr_queue_producer kr_queue_producer;

typedef struct {
  int user;
  int sz;
} kr_queue_producer_setup;

typedef struct {
  int nproducers;
  kr_queue_producer_setup producer[KR_QUEUE_PRODUCERS_MAX];
} kr_queue_setup;

typedef struct {
  int user;
  void *ptr;
} kr_queue_item;

int kr_queue_read(kr_queue *q, kr_queue_item *item);
int kr_queue_write(kr_queue_producer *p, kr_queue_item *item);
kr_queue_producer *kr_queue_get_producer(kr_queue *q, int user);
int kr_queue_get_fd(kr_queue *q);
int kr_queue_destroy(kr_queue *q);
kr_queue *kr_queue_create(kr_queue_setup *setup);

#endif
