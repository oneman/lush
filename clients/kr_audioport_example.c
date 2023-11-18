#include <cairo.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <sys/signalfd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <krad/radio/client.h>

typedef struct aux_audio_demo aux_audio_demo;

struct aux_audio_demo {
  kr_radio_client *client;
  kr_aux_params params;
  kr_aux *aux;
  uint64_t frames;
};

int main(int argc, char *argv[]) {
  int ret;
  int destroy;
  aux_audio_demo *demo;
  int sfd;
  struct signalfd_siginfo fdsi;
  ssize_t s;
  sigset_t mask;
  uint64_t now;
  now = time(NULL);
  ret = 0;
  destroy = 0;
  if (argc != 2) {
    if (argc > 2) {
      fprintf(stderr, "Only takes station argument.\n");
    } else {
      fprintf(stderr, "No station specified.\n");
    }
    return 1;
  }
  sigemptyset(&mask);
  sigfillset(&mask);
  if (sigprocmask(SIG_BLOCK, &mask, NULL) != 0) {
    fprintf(stderr, "Daemon: Could not set signal mask!");
    exit(1);
  }
  sfd = signalfd(-1, &mask, SFD_CLOEXEC);
  if (sfd == -1) {
    fprintf(stderr, "Radio Client: could not setup signalfd");
    exit(1);
  }
  demo = calloc(1, sizeof(aux_audio_demo));
  snprintf(demo->params.adapter, sizeof(demo->params.adapter), "AuxDemo");
  snprintf(demo->params.port, sizeof(demo->params.port), "Noise_%4lu", now);
  demo->client = kr_client_create("krad audio noise client");
  if (demo->client == NULL) {
    fprintf(stderr, "Could not create KR client.\n");
    free(demo);
    close(sfd);
    return 1;
  }
  kr_connect(demo->client, argv[1]);
  if (!kr_connected(demo->client)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon.\n", argv[1]);
    kr_client_destroy(&demo->client);
    free(demo);
    close(sfd);
    exit(1);
  }
  ret = kr_aux_create(demo->client, demo->params.adapter);
  if (ret != 0) {
    fprintf(stderr, "Could not create aux adapter.\n");
    kr_client_destroy(&demo->client);
    free(demo);
    close(sfd);
    exit(1);
  }
  ret = kr_video_in(demo->client, demo->params.adapter, demo->params.port);
  if (ret != 0) {
    fprintf(stderr, "Could not create aux video input.\n");
    kr_client_destroy(&demo->client);
    free(demo);
    close(sfd);
    exit(1);
  }
  demo->aux = kr_aux_connect(demo->client, &demo->params);
  if (demo->aux == NULL) {
    fprintf(stderr, "Could not connect to aux adapter.\n");
    kr_client_destroy(&demo->client);
    free(demo);
    close(sfd);
    exit(1);
  }
  /*
  kr_videoport_set_callback(videoport, videoport_process, demo);
  kr_videoport_activate(videoport);*/
  printf("Connected!\n");

  int i = 1000000;
  kr_frame frame;
  while (i--) {
    usleep(1000 * 31);
    ret = kr_aux_frame_acquire(demo->aux, &frame);
    ret = videoport_process(&frame.image, demo);
    if (ret) printke("AUX: Video Port ret %d", ret);
    kr_aux_frame_write(demo->aux, &frame);
//  kr_aux_frame_release(demo->aux, frame);
  }

  while (!destroy) {
    s = read(sfd, &fdsi, sizeof(fdsi));
    if (s != sizeof(fdsi)) {
      fprintf(stderr, "Radio Client: Error reading signalfd\n");
      kr_client_destroy(&demo->client);
      free(demo);
      exit(1);
    }
    switch (fdsi.ssi_signo) {
      case SIGHUP:
        printf("Radio Client: Got HANGUP Signal!\n");
        break;
      case SIGINT:
        printf("\nRadio Client: Got INT Signal!\n");
        printf("Radio Client: Shutting down\n");
        destroy = 1;
        break;
      case SIGTERM:
        printf("Radio Client: Got TERM Signal!\n");
        printf("Radio Client: Shutting down\n");
        destroy = 1;
        break;
      default:
        printf("Radio Client: Got Signal %u\n", fdsi.ssi_signo);
    }
  }
  /*if (kr_videoport_error(videoport)) {
    fprintf(stderr, "Error: %s\n", "videoport Error");
    ret = 1;
  }*/
  ret = kr_aux_close(demo->client, demo->aux);
  if (ret != 0) {
    fprintf(stderr, "Demo: Could not disconnect aux adapter\n");
  }
  ret = kr_aux_destroy(demo->client, demo->params.adapter);
  if (ret != 0) {
    fprintf(stderr, "Could not create aux adapter.\n");
    kr_client_destroy(&demo->client);
    free(demo);
    close(sfd);
    exit(1);
  }
  /* kr_videoport_destroy(videoport);*/
  kr_client_destroy(&demo->client);
  if (demo->frames > 0) {
    printf("Rendered %d frames!\n", demo->frames);
  }
  free(demo);
  close(sfd);
  printf("Worked!\n");
  return 0;
}
