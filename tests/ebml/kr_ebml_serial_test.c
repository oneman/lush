#include <stdio.h>
#include "../lib/krad_radio/kr_client.h"

static void get_transponder_path_info(kr_transponder_path_info *info) {
  int ret;
  char *test_name;
  char *bus_name;
  int channels;
  /* init function? */
  memset(info, 0, sizeof(kr_xpdr_path_info));
  channels = 2;
  test_name = "Music";
  bus_name = "Master";
  info->input.type = KR_XPDR_ADAPTER;
  info->input.info.adapter_path_info.api = KR_ADP_JACK;
  strcpy(info->input.info.adapter_path_info.info.jack.name, test_name);
  info->input.info.adapter_path_info.info.jack.channels = channels;
  info->input.info.adapter_path_info.info.jack.direction = KR_JACK_INPUT;
  info->output.type = KR_XPDR_MIXER;
  strcpy(info->output.info.mixer_path_info.bus, bus_name);
  info->output.info.mixer_path_info.channels = channels;
  info->output.info.mixer_path_info.type = KR_MXR_INPUT;
}

int main(int argc, char const *argv[]) {
  char text[8192];
  int res;
  int valid;
  struct kr_mixer_info minfo;
  struct kr_compositor_info cinfo;
  struct kr_transponder_path_info tinfo;
  valid = 0;
  memset(text,0,sizeof(text));
  get_transponder_path_info(&tinfo);
  valid = kr_transponder_path_info_valid(&tinfo);
  if (valid) {
    fprintf(stderr, "invalid info\n");
    fprintf(stderr, "invalid at member %d\n\n", -(valid+1));
    return 1;
  }
  res = kr_transponder_path_info_to_text(text, &tinfo, sizeof(text));
  if (res > 0) {
    printf("%s\n",text);
  }
  printf("\n\n\n");
  res = kr_mixer_path_info_to_text(text, &tinfo.output.info.mixer_path_info, sizeof(text));
  if (res > 0) {
    printf("%s\n",text);
  }
  return 0;
}
