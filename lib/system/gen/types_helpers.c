#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_priority(char *str) {
  return str_to_enum(KR_KR_PRIORITY_META_INFO, str);
}

char *kr_priority_to_str(int val) {
  return enum_to_str(KR_KR_PRIORITY_META_INFO, val);
}

