#ifndef KRAD_VECTOR_H
#define KRAD_VECTOR_H

typedef struct kr_vector kr_vector;

#include "vector_types.h"

size_t kr_vector_sizeof();
void kr_vector_clear(kr_vector *vector);
int kr_vector_init(kr_vector *vector, char *type);
int kr_vector_info_get(kr_vector *vector, kr_vector_info *info);
void kr_vector_render(kr_vector *vector, cairo_t *cr);

#endif
