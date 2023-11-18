#include <stdio.h>
#include "../lib/krad_compositor/gen/krad_compositor_common_helpers.h"
#include "../lib/krad_mixer/gen/krad_mixer_common_helpers.h"

int main(int argc, char const *argv[]) {
  char text[2048];
  int res;
  struct kr_mixer_info minfo;
  struct kr_compositor_info cinfo;

  memset(text,0,sizeof(text));

  printf("Running init helper on kr_mixer_info...\n\n");
  kr_mixer_info_init(&minfo);
  res = kr_mixer_info_to_text(text,&minfo,sizeof(text));
  if (res > 0)
    printf("%s\n",text);

  printf("Running init helper on kr_compositor_info...\n\n");
  kr_compositor_info_init(&cinfo);
  res = kr_compositor_info_to_text(text,&cinfo,sizeof(text));
  if (res > 0)
    printf("%s\n",text);

  printf("Running random init helper on kr_mixer_info...\n\n");
  kr_mixer_info_random(&minfo);
  res = kr_mixer_info_to_text(text,&minfo,sizeof(text));
  if (res > 0)
    printf("%s\n",text);

  printf("Running random init helper on kr_compositor_info...\n\n");
  kr_compositor_info_random(&cinfo);
  res = kr_compositor_info_to_text(text,&cinfo,sizeof(text));
  if (res > 0)
    printf("%s\n",text);

  printf("Running valid helper on kr_mixer_info...\n");
  res = kr_mixer_info_valid(&minfo);

  if (res >= 0) {
    printf("valid!\n\n");
  } else {
    printf("invalid at member %d\n\n",-(res+1));
  }

  printf("Running valid helper on kr_compositor_info...\n");
  res = kr_compositor_info_valid(&cinfo);

  if (res >= 0) {
    printf("valid!\n\n");
  } else {
    printf("invalid at member %d\n\n",-(res+1));
  }

  minfo.sample_rate = 256000;
  cinfo.texts = 256;

  printf("Running valid helper on kr_mixer_info again...\n");
  res = kr_mixer_info_valid(&minfo);

  if (res >= 0) {
    printf("valid!\n");
  } else {
    printf("invalid at member %d!\n\n",-(res+1));
  }

  printf("Running valid helper on kr_compositor_info again...\n");
  res = kr_compositor_info_valid(&cinfo);

  if (res >= 0) {
    printf("valid!\n");
  } else {
    printf("invalid at member %d!\n\n",-(res+1));
  }

  return 0;
}
