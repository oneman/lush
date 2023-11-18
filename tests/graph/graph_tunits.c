#include "graph_tunits.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

uint randr(uint min, uint max) {
  struct timeval tv;
  double scale;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  scale = (double)abs((int32_t)(min - max)) / RAND_MAX;
  return min + floor(rand() * scale);;
}

char *tunit_type_to_str(int type) {
  switch(type) {
    case TU_SOURCE: return "SOURCE";
    case TU_BUS: return "BUS";
    case TU_OUTPUT: return "OUTPUT";
    case TU_LINK: return "LINK";
    default: return NULL;
  }
}

kr_vertex_type tunit_type_to_vertex_type(int type) {
  switch(type) {
    case TU_SOURCE: return KR_VERTEX_SOURCE;
    case TU_BUS: return KR_VERTEX_BUS;
    case TU_OUTPUT: return KR_VERTEX_OUTPUT;
    default:
      fprintf(stderr, "Tunit type not a vertex\n");
      exit(1);
  }
}

tunit *tunit_create(tunit_setup *setup) {
  tunit *unit;
  kr_vertex_type vt;
  unit = calloc(1, sizeof(tunit));
  if (unit == NULL) return unit;
  unit->type = setup->type;
  if (setup->type != TU_LINK) {
    vt = tunit_type_to_vertex_type(setup->type);
    unit->elem = kr_graph_vertex(setup->graph, vt, unit);
  } else {
    unit->elem = kr_graph_link(setup->graph, &setup->to->elem,
     &setup->from->elem, unit);
  }
  if (unit->elem.type == 0) {
    free(unit);
    return NULL;
  }
  strncpy(unit->name, setup->name, sizeof(unit->name));
  unit->integer = setup->integer;
  return unit;
}

int parallel_chain(kr_chain *ch1, kr_chain *ch2) {
  int i;
  int j;
  tunit *unit1;
  tunit *unit2;
  for (i = 1; i < ch1->len - 1; i++) {
    unit1 = (tunit *)ch1->users[i];
    for (j = 1; j < ch2->len - 1; j++) {
      unit2 = (tunit *)ch1->users[j];
      if (unit1->elem.vertex == unit2->elem.vertex) {
        return 2;
      }
    }
  }
  return 1;
}

void print_chain(kr_chain *chain) {
  int j;
  tunit *unit;
  printf("  ");
  for (j = 0; j < chain->len; j++) {
    unit = (tunit *)chain->users[j];
    printf("%s ",unit->name);
    if (j != (chain->len - 1))
      printf("---> ");
  }
  printf("\n");
}

void print_chains(kr_chain *chains, int n) {
  int i;
  int j;
  char parallel[n][n];
  memset(parallel, 0, sizeof(parallel));
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i != j) {
        parallel[i][j] = parallel_chain(&chains[i], &chains[j]);
      }
    }
  }
  for (i = 0; i < n; i++) {
    print_chain(&chains[i]);
    for (j = 0; j < n; j++) {
      if (parallel[i][j] == 1) {
        printf("    parallel to");
        print_chain(&chains[j]);
      } else if (parallel[i][j] == 2) {
        printf("    serial to");
        print_chain(&chains[j]);
      }
    }
  }
}

int tunit_process(kr_graph *graph, tunit *unit) {
  int n;
  int m;
  int i;
  void *outputs[32];
  kr_chain chains[32];
  tunit *output;
  printf("Processing SOURCE %s\n\n",unit->name);
  n = kr_graph_outputs(graph, &unit->elem, outputs, 32);
  printf("%d OUTPUT(s) connected to SOURCE %s\n",n,unit->name);
  for (i = 0; i < n; i++) {
    output = (tunit *)outputs[i];
    m = kr_graph_chains(graph, &output->elem, &unit->elem, chains, 32);
    printf("%d chain(s) from SOURCE %s to OUTPUT %s:\n",m,unit->name,output->name);
    print_chains(chains, m);
  }
  printf("\n");
  return 0;
}

int tunit_ctl(tunit *unit) {
  return 0;
}

int tunit_destroy(tunit *unit) {
  free(unit);
  return 0;
}

tunit *tunit_random_gen(kr_graph *graph, int type, int k, tunit **units, int nv) {
  tunit_setup tunit_setup;
  tunit *unit;
  memset(&tunit_setup, 0, sizeof(tunit_setup));
  tunit_setup.graph = graph;
  tunit_setup.name = names[k];
  tunit_setup.integer = randr(0, 10);
  tunit_setup.type = type;
  if (type == TU_LINK) {
    tunit_setup.from = units[randr(0, nv)];
    tunit_setup.to = units[randr(0, nv)];
  }
  unit = tunit_create(&tunit_setup);
  if (unit && type == TU_LINK) {
    printf("new %s %s from %s to %s\n",tunit_type_to_str(unit->type),
      unit->name,tunit_setup.from->name,tunit_setup.to->name);
  } else if (unit && type != TU_LINK) {
    printf("new %s %s\n",tunit_type_to_str(unit->type),unit->name);
  }
  return unit;
}

void tunits_destroy(tunit **units, int n) {
  int i;
  for (i = 0; i < n; i++) {
    tunit_destroy(units[i]);
  }
}

int tunits_random_populate(kr_graph *graph, tunit **units, int max) {
  int n;
  int v;
  int e;
  int i;
  tunit *tunit;
  n = randr(max/4,max/2);
  for (i = v = 0; i < n; i++) {
    tunit = tunit_random_gen(graph, randr(TU_SOURCE, TU_OUTPUT+1), v, NULL, 0);
    if (tunit) {
      units[v] = tunit;
      v++;
    }
  }
  n = randr(max/4,max/2);
  for (i = e = 0; i < n; i++) {
    tunit = tunit_random_gen(graph, TU_LINK, v+e, units, v);
    if (tunit) {
      units[v+e] = tunit;
      e++;
    }
  }
  printf("\n%d random tunits generated! %d vertices, %d edges\n\n",v+e,v,e);
  return v+e;
}

int run_test() {
  int n;
  int i;
  tunit *units[MAX_TUNITS];
  kr_graph *graph;
  kr_graph_setup graph_setup;
  memset(&graph_setup, 0, sizeof(graph_setup));
  graph = kr_graph_create(&graph_setup);
  if (graph == NULL) return 1;
  n = tunits_random_populate(graph, units, MAX_TUNITS);
  for (i = 0; i < n; i++) {
    if (units[i]->type == TU_SOURCE) {
      tunit_process(graph, units[i]);
    }
  }
  tunits_destroy(units,n);
  printf("%d units destroyed\n",n);
  kr_graph_destroy(graph);
  return 0;
}

int main(int argc, char const *argv[]) {
  int res;
  res = run_test();
  return res;
}
