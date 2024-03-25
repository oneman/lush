#include "kr_client.h"
void insert_coin_to_continue() {
  char c;
  printf("Insert Coin To Continue!$# ");
  scanf("%c", &c);
  printf("\nOK %c\n", c);
}

int wayland_dis(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_WAYLAND;
  sprintf(nfo.wl.display_name, "wayland-1");
  nfo.wl.state = KR_WL_CONNECTED;
  ret = kr_xpdr_make(client, "oneway", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_WAYLAND_OUT;
  nfo.wl_out.width = 1920;
  nfo.wl_out.height = 1080;
  nfo.wl_out.fullscreen = 0;
  ret = kr_xpdr_make(client, "oneway/onewin", &nfo);

  return ret;
}

int mixer_link(kr_client *client) {
  int ret;
  kr_mixer_path_info nfo;

  insert_coin_to_continue();
  kr_mixer_path_info_init(&nfo);
  nfo.type = KR_MXR_INPUT;
  nfo.channels = 2;
  ret = kr_mixer_make(client, "out/in", &nfo);
  return ret;
}

int jack_cap(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK;
  sprintf(nfo.jack.client_name, "demo");
  sprintf(nfo.jack.server_name, "");
  nfo.jack.sample_rate = 48000;
  nfo.jack.period_size = 1024;
  ret = kr_xpdr_make(client, "jackpipe", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK_IN;
  sprintf(nfo.jack_in.name, "in");
  nfo.jack_in.channels = 2;
  nfo.jack_in.direction = KR_JACK_INPUT;
  ret = kr_xpdr_make(client, "jackpipe/in", &nfo);

  insert_coin_to_continue();
  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK_IN;
  sprintf(nfo.jack_in.name, "in2");
  nfo.jack_in.channels = 2;
  nfo.jack_in.direction = KR_JACK_INPUT;
  ret = kr_xpdr_make(client, "jackpipe/in2", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_JACK_OUT;
  sprintf(nfo.jack_out.name, "out");
  nfo.jack_out.channels = 2;
  nfo.jack_out.direction = KR_JACK_OUTPUT;
  ret = kr_xpdr_make(client, "jackpipe/out", &nfo);

  return ret;
}

int v4l2_cap(kr_client *client) {
  int ret;
  kr_xpdr_path_info nfo;

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_V4L2;
  nfo.v4l2.dev = 0;
  nfo.v4l2.priority = 0;
  ret = kr_xpdr_make(client, "v4l2", &nfo);

  insert_coin_to_continue();
  kr_xpdr_path_info_init(&nfo);
  nfo.type = KR_V4L2_IN ;
  nfo.v4l2_in.width = 1920;
  nfo.v4l2_in.height = 1080;
  nfo.v4l2_in.num = 60;
  nfo.v4l2_in.den = 1;
  ret = kr_xpdr_make(client, "v4l2/cam", &nfo);

  return ret;
}

int rig(int argc, char *argv[]) {
  return paper(argc, argv);
  int ret;
  kr_client *client;
  char *sysname = "demo";
  client = kr_client_create("demo cmdr");
  if(client == NULL) {
    fprintf(stderr, "Could create client\n");
    return 1;
  }
  if (!kr_connect(client, sysname)) {
    fprintf(stderr, "Could not connect to %s krad radio daemon\n", sysname);
    kr_client_destroy(&client);
    return 1;
  }
  ret = jack_cap(client);
  ret = mixer_link(client);
  //ret = v4l2_cap(client);
  //ret = wayland_dis(client);
  kr_client_destroy(&client);
  return ret;
}
