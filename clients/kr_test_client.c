#include <krad/radio/client.h>

static int test_wl_create(kr_client *client);
static int test_v4l2_create(kr_client *client);
static int test_aux_create(kr_client *client);
static int test_aux_in_create(kr_client *client);
static int test_wayland_output_create(kr_client *client);
static int test_complink_create(kr_client *client);

/*
static int test_jack_input_create(kr_client *client);
static int test_jack_output_create(kr_client *client);
static int test_decklink_input_create(kr_client *client, int dev);
*/

static int test_aux_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  name = "Auxilirator";
  info.type = KR_AUX;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

static int test_wl_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  name = "Wayland";
  info.type = KR_WAYLAND;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

static int test_v4l2_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  kr_v4l2_info *vi;
  vi = &info.v4l2;
  char *name;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  name = "V4L2";
  info.type = KR_V4L2;
  vi->dev = 0;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

static int test_v4l2_input_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  kr_v4l2_path_info *vc;
  char *name;
  name = "V4L2/Capture";
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  vc = &info.v4l2_in;
  info.type = KR_V4L2_IN;
  vc->width = 640;
  vc->height = 360;
  vc->num = 30;
  vc->den = 1;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

static int test_complink_create(kr_client *client) {
  int ret;
  kr_crate crate;
  memset(&crate, 0, sizeof(crate));
  kr_compositor_path_info *com_path;
  kr_compositor_input_info *input;
  strcpy(crate.address, "/compositor/TestWindow/Capture");
  crate.method = KR_PUT;
  crate.type = KR_COMPOSITOR_PATH_INFO;
  com_path = &crate.info.compositor_path_info;
  com_path->type = KR_COM_INPUT;
  input = &com_path->input;
  input->pos.x = 0;
  input->pos.y = 0;
  input->pos.w = 7680;
  input->pos.h = 4320;
  input->opacity = 1.0f;
  input->rotation = 0;
  input->z = 0;
  ret = kr_crate_send(client, &crate);
  return ret;
}

static int test_aux_in_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  name = "AUX INPUT";
  info.type = KR_AUX_VIDEO_IN;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

/*
static int test_jack_input_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int channels;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  channels = 2;
  name = "Music1";
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_JACK;
  strcpy(info.input.info.adapter_path_info.info.jack.name, name);
  info.input.info.adapter_path_info.info.jack.channels = channels;
  info.input.info.adapter_path_info.info.jack.direction = KR_JACK_INPUT;
  info.output.type = KR_XPDR_MIXER;
  info.output.info.mixer_path_info.channels = channels;
  info.output.info.mixer_path_info.type = KR_MXR_SOURCE;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_jack_output_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int channels;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  channels = 2;
  name = "Main";
  info.input.type = KR_XPDR_MIXER;
  info.input.info.mixer_path_info.channels = channels;
  info.input.info.mixer_path_info.type = KR_MXR_OUTPUT;
  info.output.type = KR_XPDR_ADAPTER;
  info.output.info.adapter_path_info.api = KR_ADP_JACK;
  strcpy(info.output.info.adapter_path_info.info.jack.name, name);
  info.output.info.adapter_path_info.info.jack.channels = channels;
  info.output.info.adapter_path_info.info.jack.direction = KR_JACK_OUTPUT;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_x11_input_create(kr_client *client, int dev) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  int width;
  int height;
  int num;
  int den;
  name = "X11Test";
  width = 640;
  height = 360;
  num = 30;
  den = 1;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_X11;
  strcat(info.input.info.adapter_path_info.info.x11.display, "");
  info.input.info.adapter_path_info.info.x11.num = num;
  info.input.info.adapter_path_info.info.x11.den = den;
  info.input.info.adapter_path_info.info.x11.width = width;
  info.input.info.adapter_path_info.info.x11.height = height;
  info.output.type = KR_XPDR_COMPOSITOR;
  info.output.info.compositor_path_info.info.source_info.w = width;
  info.output.info.compositor_path_info.info.source_info.h = height;
  info.output.info.compositor_path_info.type = KR_COM_SOURCE;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}

static int test_decklink_input_create(kr_client *client, int dev) {
  int ret;
  kr_xpdr_path_info info;
  char *name;
  char *video_connector;
  char *audio_connector;
  int width;
  int height;
  int num;
  int den;
  name = "DecklinkTest";
  video_connector = "hdmi";
  audio_connector = "hdmi";
  width = 1280;
  height = 720;
  num = 60000;
  den = 1001;
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  info.input.type = KR_XPDR_ADAPTER;
  info.input.info.adapter_path_info.api = KR_ADP_DECKLINK;
  snprintf(info.input.info.adapter_path_info.info.decklink.device,
   sizeof(info.input.info.adapter_path_info.info.decklink.device), "%d", dev);
  strcpy(info.input.info.adapter_path_info.info.decklink.video_connector,
   video_connector);
  strcpy(info.input.info.adapter_path_info.info.decklink.audio_connector,
   audio_connector);
  info.input.info.adapter_path_info.info.decklink.num = num;
  info.input.info.adapter_path_info.info.decklink.den = den;
  info.input.info.adapter_path_info.info.decklink.width = width;
  info.input.info.adapter_path_info.info.decklink.height = height;
  info.output.type = KR_XPDR_COMPOSITOR;
  info.output.info.compositor_path_info.info.source_info.w = width;
  info.output.info.compositor_path_info.info.source_info.h = height;
  info.output.info.compositor_path_info.type = KR_COM_SOURCE;
  ret = kr_xpdr_mkpath(client, name, &info);
  return ret;
}
*/
static int test_wayland_output_create(kr_client *client) {
  int ret;
  kr_xpdr_path_info info;
  kr_wayland_path_info *window;
  char *name;
  name = "Wayland/TestWindow";
  memset(&info, 0, sizeof(kr_xpdr_path_info));
  window = &info.wl_out;
  info.type = KR_WAYLAND_OUT;
  window->width = 640;
  window->height = 360;
  window->fullscreen = 0;
  ret = kr_xpdr_make(client, name, &info);
  return ret;
}

int test_x11_get(kr_client *client) {
  int ret;
  ret = kr_get(client, "/compositor/X11Test");
  kr_poll(client, 250);
  kr_delivery_recv(client);
  kr_streamer45(client);
  return ret;
}

int make_masterbus(kr_client *client) {
  int ret;
  kr_crate crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *masterbus;
  strcpy(crate.address, "/mixer/Master");
  crate.method = KR_PUT;
  crate.type = KR_MIXER_PATH_INFO;
  masterbus = &crate.info.mixer_path_info;
  masterbus->channels = 2;
  masterbus->type = KR_MXR_BUS;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int make_musicbus(kr_client *client) {
  int ret;
  kr_crate crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *bus;
  strcpy(crate.address, "/mixer/Music");
  crate.method = KR_PUT;
  crate.type = KR_MIXER_PATH_INFO;
  bus = &crate.info.mixer_path_info;
  bus->channels = 2;
  bus->type = KR_MXR_BUS;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int make_musicmaster(kr_client *client) {
  int ret;
  kr_crate crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *in;
  strcpy(crate.address, "/mixer/Master/Music");
  crate.method = KR_PUT;
  crate.type = KR_MIXER_PATH_INFO;
  in = &crate.info.mixer_path_info;
  in->channels = 2;
  in->type = KR_MXR_INPUT;
  ret = kr_crate_send(client, &crate);
  return ret;
}

int make_jackinout(kr_client *client) {
  int ret;
  kr_crate crate;
  memset(&crate, 0, sizeof(crate));
  kr_mixer_path_info *in;
  strcpy(crate.address, "/mixer/Main/Music1");
  crate.method = KR_PUT;
  crate.type = KR_MIXER_PATH_INFO;
  in = &crate.info.mixer_path_info;
  in->channels = 2;
  in->type = KR_MXR_INPUT;
  ret = kr_crate_send(client, &crate);
  return ret;
}

static int strmatch(char *str1, char *str2) {
  if ((strlen(str1) == strlen(str2)) && (strcmp(str1, str2) == 0)) return 1;
  return 0;
}

int run_test(kr_client *client, char *test) {
  int ret;
  ret = -1;
  if (strmatch(test, "aux")) {
    ret = test_aux_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "auxin")) {
    ret = test_aux_in_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "masterbus")) {
    ret = make_masterbus(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "musicbus")) {
    ret = make_musicbus(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "musicmaster")) {
    ret = make_musicmaster(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "getx11")) {
    ret = test_x11_get(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "wayland")) {
    ret = test_wl_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "v4l2")) {
    ret = test_v4l2_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "v4l2cap")) {
    ret = test_v4l2_input_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "window")) {
    ret = test_wayland_output_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "complink")) {
    ret = test_complink_create(client);
    if (ret != 0) return ret;
  }
  /*
  if (strmatch(test, "x11")) {
    ret = test_x11_input_create(client, 0);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "jackin")) {
    ret = test_jack_input_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "jackout")) {
    ret = test_jack_output_create(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "jackinout")) {
    ret = make_jackinout(client);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "decklink")) {
      ret = test_decklink_input_create(client, 0);
    if (ret != 0) return ret;
  }
  if (strmatch(test, "alsa")) {
    if (ret != 0) return ret;
  }
  */
  return ret;
}

int main(int argc, char *argv[]) {
  kr_client *client;
  char *sysname;
  const char *tests;
  int ret;
  int i;
  tests = "jack decklink wayland x11 v4l2 alsa";
  client = NULL;
  sysname = NULL;
  ret = 0;
  if (argc < 3) {
    fprintf(stderr, "Usage: %s [STATION] [TESTS]\nTests: %s\n",
     "kr_test_client", tests);
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
  printf("Running Tests\n");
  for (i = 0; i < (argc - 2); i++) {
    ret = run_test(client, argv[2 + i]);
    if (ret != 0) break;
  }
  printf("Disconnecting from %s..\n", sysname);
  kr_disconnect(client);
  printf("Disconnected from %s.\n", sysname);
  printf("Destroying client..\n");
  kr_client_destroy(&client);
  printf("Client Destroyed.\n");
  return ret;
}
