#include <stdint.h>
#ifndef KRAD_GRAPH_H
#define KRAD_GRAPH_H
#define MAX_VERTICES 64
#define MAX_EDGES 1024
#define MAX_USERS 32

typedef struct kr_graph kr_graph;
typedef struct kr_vertex kr_vertex;
typedef struct kr_edge kr_edge;
typedef struct kr_graph_elem kr_graph_elem;
typedef struct kr_chain kr_chain;
typedef enum kr_vertex_type kr_vertex_type;
typedef enum kr_edge_dir kr_edge_dir;
typedef enum kr_graph_elem_type kr_graph_elem_type;

typedef void (kr_graph_edge_destroy_cb)(void *user);
typedef void (kr_graph_vertex_process_cb)(void *user_to, void **user_from, int from_len);

enum kr_vertex_type {
  KR_VERTEX_SOURCE = 1,
  KR_VERTEX_BUS,
  KR_VERTEX_OUTPUT
};

enum kr_graph_elem_type {
  KR_GRAPH_VERTEX = 1,
  KR_GRAPH_EDGE
};

struct kr_graph_elem {
  kr_graph_elem_type type;
  union {
    kr_edge *edge;
    kr_vertex *vertex;
  };
};

struct kr_chain {
  void *users[MAX_USERS];
  int len;
};

typedef struct {
  int inputs;
  int outputs;
  int buses;
  kr_graph_edge_destroy_cb *edge_destroy_cb;
  kr_graph_vertex_process_cb *vertex_process_cb;
  int use_locking;
} kr_graph_setup;

int kr_graph_set_user(kr_graph *graph, kr_graph_elem *elem, void *user);
int kr_graph_in_out_links(kr_graph *graph, kr_graph_elem *elem, int dir, void **user, int max);
int kr_graph_outputs(kr_graph *graph, kr_graph_elem *elem, void **user, int max);
int kr_graph_sources(kr_graph *graph, kr_graph_elem *elem, void **user, int max);
int kr_graph_chains(kr_graph *graph, kr_graph_elem *to, kr_graph_elem *from, kr_chain *chain, int max);
int kr_graph_chains_full(kr_graph *graph, kr_graph_elem *to, kr_graph_elem *from, kr_chain *chain, int max);
kr_graph_elem kr_graph_link(kr_graph *graph, kr_graph_elem *to, kr_graph_elem *from, void *user);
kr_graph_elem kr_graph_vertex(kr_graph *graph, kr_vertex_type type, void *user);
int kr_graph_remove(kr_graph *graph, kr_graph_elem *elem);
int kr_graph_destroy(kr_graph *graph);
kr_graph *kr_graph_create(kr_graph_setup *setup);
#endif
