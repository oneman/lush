#include <stdio.h>
#include <krad/xpdr/types.h>

int main(int argc, char const *argv[]) {
  char json[4096];
  char text[4096];
  int res;

  kr_xpdr_path_info test;
  memset(&test, 0, sizeof(test));

  test.type = KR_JACK;

  res = kr_xpdr_path_info_to_text(text, &test, sizeof(text));
  if (res > 0) {
    printf("%s\n",text);
  }

  res = kr_xpdr_path_info_to_json(json, &test, sizeof(json));
  if (res > 0) {
    printf("%s\n\n",json);
  }

  kr_xpdr_path_info test2;
  memset(&test2, 0, sizeof(test2));
  res = kr_xpdr_path_info_fr_json(json, &test2);

  printf("res fr_json %d\n\n",res);

  res = kr_xpdr_path_info_to_text(text, &test2, sizeof(text));
  if (res > 0) {
    printf("%s\n",text);
  }

  return 0;
}
