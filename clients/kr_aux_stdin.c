#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <krad/app/debug.h>
#include <krad/time/timer.h>
#include <krad/mem/mem.h>
#include <krad/ring/ring.h>
#include <krad/loop/loop.h>
#include <krad/io/file.h>
#include <krad/radio/client.h>
#include <krad/time/stamp.h>

typedef struct {
  char name[KRAD_SYSNAME_SZ];
  char callsign[KRAD_SYSNAME_SZ];
  kr_loop *loop;
  kr_radio_client *client;
  kr_aux_params params;
  kr_aux *aux;
  kr_ringbuf *samples[KR_AUX_KLUDGE_NCHANNELS];
  kr_audio audio;
} aux_test;

static int audio_gen(aux_test *at, kr_audio *a) {
  int i;
  int ret;
  size_t avail;
  size_t read;
  read = 0;
  avail = krad_ringbuffer_read_space(at->samples[0]);
  if (a->count < 1 || a->count * sizeof(float) > avail) {
    for (i = 0; i < KR_AUX_KLUDGE_NCHANNELS; i++) {
      memset(a->samples[i], 0, sizeof(float) * KR_AUX_KLUDGE_PERIOD_NFRAMES);
    }
    ret = kr_aux_audio_write(at->aux, a);
    return ret;
  }
  for (i = 0; i < KR_AUX_KLUDGE_NCHANNELS; i++) {
    read = krad_ringbuffer_read(at->samples[i], (char *)a->samples[i],
     a->count * sizeof(float));
  }
  printkd("RING: read %zu", read);
  ret = kr_aux_audio_write(at->aux, a);
  return ret;
}

static int stdin_event(kr_event *e) {
  int i;
  int ret;
  aux_test *at;
  float data[KR_AUX_KLUDGE_PERIOD_NFRAMES];
  at = (aux_test *)e->user;
  ret = read(0, data, sizeof(data));
  if (ret < 0 || (ret % 8)) {
    printke("Read failed! %d", ret);
    return ret;
  }
  at->audio.count = ret / (KR_AUX_KLUDGE_NCHANNELS * sizeof(float));
  kr_audio_deinterleave(&at->audio, data);
  for (i = 0; i < KR_AUX_KLUDGE_NCHANNELS; i++) {
    if (krad_ringbuffer_write_space(at->samples[i]) < at->audio.count * sizeof(float)) {
      printke("Not enough space in ringbuffer!");
      // making room
      krad_ringbuffer_read_advance(at->samples[i], at->audio.count * sizeof(float));
    }
    krad_ringbuffer_write(at->samples[i], (char *)at->audio.samples[i], at->audio.count * sizeof(float));
  }
  printkd("RING: wrote %zu", at->audio.count * sizeof(float));
  return 0;
}

static int audio_in_event(kr_event *e) {
  int ret;
  if (!e) return -1;
  aux_test *at;
  kr_audio audio;
  at = (aux_test *)e->user;
  if (e->events & EPOLLRDHUP || e->events & EPOLLHUP || e->events & EPOLLERR) {
    return -1;
  }
  if (e->events & EPOLLIN) {
    ret = kr_aux_audio_read(at->aux, &audio);
    if (ret > 0) {
      printkd("Got Audio event ret %d", ret);
      ret = kr_aux_audio_acquire(at->aux, &audio);
      ret = audio_gen(at, &audio);
    }
    return 0;
  }
  return -1;
}

static int at_signal(kr_event *event) {
  ssize_t s;
  aux_test *at;
  struct signalfd_siginfo fdsi;
  at = (aux_test *)event->user;
  for (;;) {
    s = read(event->fd, &fdsi, sizeof(fdsi));
    if ((s == -1) && (errno == EAGAIN)) break;
    if (s != sizeof(fdsi)) {
      failfast("%s signal: Error reading signalfd", at->name);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printk("%s signal: Got HANGUP Signal!", at->name);
        break;
      case SIGINT:
        printk("\n%s: Got INT Signal!", at->name);
        printk("%s: Shutting down", at->name);
        return -1;
      case SIGTERM:
        printk("%s: Got TERM Signal!", at->name);
        printk("%s: Shutting down", at->name);
        return -1;
      default:
        printk("%s: Got Signal %u", at->name, fdsi.ssi_signo);
    }
  }
  return 0;
}

static int at_startup(kr_loop *loop, void *user) {
  int i;
  int fail;
  int ret;
  kr_event e;
  aux_test *at;
  at = (aux_test *)user;
  printk("%s startup", at->name);
  at->loop = loop;
  fail = 0;
  snprintf(at->params.adapter, sizeof(at->params.adapter), "AuxAudio");
  snprintf(at->params.port, sizeof(at->params.port), "%s", at->name);
  at->client = kr_client_create("krad aux test");
  if (!at->client) {
    fprintf(stderr, "Could not create KR client.\n");
    fail = 1;
  }
  if (!fail) {
    kr_connect(at->client, at->callsign);
    if (!kr_connected(at->client)) {
      fprintf(stderr, "Could not connect to %s krad radio daemon.\n",
       at->callsign);
      fail = 1;
    }
  }
  if (!fail) {
    /* check if this exists */
    ret = kr_aux_create(at->client, at->params.adapter);
    if (ret) {
      fprintf(stderr, "Could not create aux adapter.\n");
      fail = 1;
    }
  }
  if (!fail) {
    /* check if this exists */
    ret = kr_aux_port_create(at->client, at->params.type, at->params.adapter,
     at->params.port);
    if (ret) {
      fprintf(stderr, "Could not create aux video input.\n");
      fail = 1;
    }
  }
  if (!fail) {
    /* check if this is connected */
    at->aux = kr_aux_connect(at->client, &at->params);
    if (!at->aux) {
      fprintf(stderr, "Could not connect to aux adapter.\n");
      fail = 1;
    }
  }
  if (!fail) {
    if (at->params.type == KR_AUX_AUDIO_IN) {
      memset(&at->audio, 0, sizeof(at->audio));
      at->audio.channels = KR_AUX_KLUDGE_NCHANNELS;
      at->audio.count = KR_AUX_KLUDGE_PERIOD_NFRAMES;
      at->audio.rate = 48000;
      for (i = 0; i < KR_AUX_KLUDGE_NCHANNELS; i++) {
        at->samples[i] = krad_ringbuffer_create(48000 * 4 * sizeof(float));
        if (!at->samples[i]) {
          fprintf(stderr, "Could not create ringbuffers.\n");
          fail = 1;
        }
        at->audio.samples[i] = kr_alloc(KR_AUX_KLUDGE_PERIOD_NFRAMES * sizeof(float));
      }
    }
  }
  if (!fail) {
    e.events = EPOLLIN;
    e.user = at;
    e.fd = kr_aux_get_fd(at->aux);
    e.handler = audio_in_event;
    kr_loop_add(at->loop, &e);
    e.events = EPOLLIN;
    e.user = at;
    e.fd = STDIN_FILENO;
    e.handler = stdin_event;
    kr_loop_add(at->loop, &e);
  }
  if (fail) {
    kr_client_destroy(&at->client);
  }
  return fail;
}

static int at_shutdown(void *user) {
  int i;
  int ret;
  int fail;
  aux_test *at;
  at = (aux_test *)user;
  printk("%s shutdown", at->name);
  fail = 0;
  if (!at) return -1;
  if (!at->client) return -2;
  if (at->params.type == KR_AUX_AUDIO_IN) {
    for (i = 0; i < KR_AUX_KLUDGE_NCHANNELS; i++) {
      krad_ringbuffer_free(at->samples[i]);
      free(at->audio.samples[i]);
    }
  }
  if (at->aux) {
    ret = kr_aux_close(at->client, at->aux);
    if (ret) {
      fail++;
      fprintf(stderr, "Demo: Could not disconnect aux adapter\n");
    }
    ret = kr_aux_destroy(at->client, at->params.adapter);
    if (ret) {
      fail++;
      fprintf(stderr, "Could not create aux adapter.\n");
    }
  }
  kr_client_destroy(&at->client);
  if (fail) return 1;
  return 0;
}

static int run_test(char *callsign, char *name) {
  aux_test at;
  kr_mainloop_setup setup;
  at.params.type = KR_AUX_AUDIO_IN;
  snprintf(at.name, sizeof(at.name), "%s", name);
  snprintf(at.callsign, sizeof(at.callsign), "%s", callsign);
  setup.user = &at;
  setup.signal_handler = at_signal;
  setup.startup_handler = at_startup;
  setup.shutdown_handler = at_shutdown;
  kr_mainloop(&setup);
  return 0;
}

int main(int argc, char *argv[]) {
  int ret;
  if (argc == 3) {
    ret = run_test(argv[1], argv[2]);
  } else {
    fprintf(stderr, "Need station callsign and an aux name\n");
    ret = 1;
  }
  return ret;
}
