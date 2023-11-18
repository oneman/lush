#include "types_helpers.h"

#include <krad/util/util.h>
int str_to_kr_container_type(char *str) {
  return str_to_enum(KR_KR_CONTAINER_TYPE_META_INFO, str);
}

char *kr_container_type_to_str(int val) {
  return enum_to_str(KR_KR_CONTAINER_TYPE_META_INFO, val);
}

