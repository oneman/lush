#include <krad/graph/graph.h>
#include "../data.h"
#define MAX_TUNITS 64

typedef enum {
  TU_SOURCE = 1,
  TU_BUS,
  TU_OUTPUT,
  TU_LINK
} tunit_type;

typedef struct {
  char name[64];
  int integer;
  kr_graph_elem elem;
  tunit_type type;
} tunit;

typedef struct {
  const char *name;
  tunit_type type;
  int integer;
  kr_graph *graph;
  tunit *to;
  tunit *from;
} tunit_setup;
