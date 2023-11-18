#if !defined(_SYSTEM_GEN_TYPES_HELPERS_H)
#define _SYSTEM_GEN_TYPES_HELPERS_H (1)

#include <stdio.h>
#include <stdint.h>
#include "../types.h"
#include <krad/app/path.h>
#include <krad/app/patch.h>
#include <krad/app/diff.h>
int str_to_kr_priority(char *str);
char *kr_priority_to_str(int val);
#endif

