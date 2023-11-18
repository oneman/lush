#if !defined(_CONTAINER_GEN_TYPES_HELPERS_H)
#define _CONTAINER_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_container_type(char *str);
char *kr_container_type_to_str(int val);
#endif

