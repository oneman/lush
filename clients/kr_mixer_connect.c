#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <krad/radio/client.h>
#include <krad/mem/mem.h>
#include <krad/app/debug.h>

#define MAX_PATHS 16

static int coin_flip() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  return ((int)(100.0 * rand() / (RAND_MAX + 1.0)) + 1) % 2;
}

/*static int rand_rng(int min, int max) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  scale = (double)abs(min - max) / RAND_MAX;
  return min + floor(rand() * scale);
}*/

static int get_crate(kr_crate *cr, kr_client *client, char *addr) {
  int res;
  res = kr_get(client, addr);
  if (res != 0) return -1;
  res = kr_poll(client, 250);
  if (res > 0) {
    kr_delivery_recv(client);
    res = kr_crate_recv(cr, client);
    if (res == 1) return 0;
  }
  return 1;
}

static int mixer_connect(kr_client *client) {
  int res;
  int i;
  int j;
  kr_crate *cr;
  kr_crate *cr2;
  kr_path_info sources[MAX_PATHS];
  int nsources;
  kr_path_info outputs[MAX_PATHS];
  int noutputs;
  nsources = 0;
  noutputs = 0;
  cr = malloc(sizeof(kr_crate));
  cr2 = malloc(sizeof(kr_crate));
  res = get_crate(cr, client, "/mixer");
  if (res != 0) return -1;
  if (cr->type == KR_PATH_LIST) {
    for (i = 0; i < cr->info.path_list.len; i++) {
      //printk("GET %s", cr->info.path_list.path[i].name);
      res = get_crate(cr2, client, cr->info.path_list.path[i].name);
      if (res != 0) return -1;
      if (cr2->type == KR_MIXER_PATH_INFO) {
        if (cr2->info.mixer_path_info.type == KR_MXR_SOURCE) {
          if (nsources == MAX_PATHS) break;
          //printk("  %s is SOURCE", cr->info.path_list.path[i].name);
          sources[nsources++] = cr->info.path_list.path[i];
        } else if (cr2->info.mixer_path_info.type == KR_MXR_OUTPUT) {
          if (noutputs == MAX_PATHS) break;
          //printk("  %s is OUTPUT", cr->info.path_list.path[i].name);
          outputs[noutputs++] = cr->info.path_list.path[i];
        }
      }
    }
  }
  for (i = 0; i < noutputs; i++) {
    if (coin_flip()) continue;
    for (j = 0; j < nsources; j++) {
      if (coin_flip()) continue;
      memset(cr, 0, sizeof(*cr));
      cr->method = KR_PUT;
      cr->type = KR_MIXER_PATH_INFO;
      sprintf(cr->address, "%s%s", outputs[i].name,
       strstr(sources[j].name, "/mixer") + 6);
      printk("  PUT %s", cr->address);
      kr_mixer_path_info_random(&cr->info.mixer_path_info);
      cr->info.mixer_path_info.type = KR_MXR_INPUT;
      cr->info.mixer_path_info.channels = KR_STEREO;
      res = kr_crate_send(client, cr);
    }
  }
  free(cr);
  free(cr2);
  return 0;
}

int main(int argc, char *argv[]) {
  int res;
  kr_client *client;
  char *sysname;
  sysname = NULL;
  client = NULL;
  if (argc < 2) {
    fprintf(stderr, "Specify a station sysname!\n");
    return 1;
  }
  if (kr_valid_host_and_port(argv[1])) {
    sysname = argv[1];
  } else {
    if (!kr_sysname_valid(argv[1])) {
      fprintf(stderr, "Invalid station sysname!\n");
      return 1;
    } else {
      sysname = argv[1];
    }
  }
  client = kr_client_create("krad simple client");
  if (client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  printf("Connected to %s!\n", sysname);
  res = mixer_connect(client);
  if (res != 0) printke("mixer_connect failed.");
  printf("Disconnecting from %s..\n", sysname);
  kr_disconnect(client);
  printf("Disconnected from %s.\n", sysname);
  printf("Destroying client..\n");
  kr_client_destroy(&client);
  printf("Client Destroyed.\n");
  return res;
}
