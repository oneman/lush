#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/ebml/ebml.h>
#include <krad/util/util.h>
#include <krad/gen/metainfo.h>

int jschema_test() {
  char json[8192];
  int res;
  memset(json, 0, sizeof(json));
  res = metainfo_to_jschema(json, sizeof(json), KR_KR_MIXER_PATH_INFO_META_INFO);
  if (res >= sizeof(json)) return -1;
  printf("JSCHEMA: \n%s\n", json);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = jschema_test();
  if (res != 0) {
    fprintf(stderr, "Failure!\n");
    return -1;
  }
  printf("Success!\n");
  return 0;
}
