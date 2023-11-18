#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <krad/app/spinlock.h>
#include "graph.h"

enum deps_indeps {
  DEPS = 1,
  INDEPS
};

enum kr_edge_dir{
  IN = 1,
  OUT
};

struct kr_vertex {
  uint8_t adj[MAX_VERTICES];
  uint8_t deps[MAX_VERTICES];
  kr_vertex_type type;
  void *user;
};

struct kr_edge {
  kr_vertex *from;
  kr_vertex *to;
  void *user;
};

struct kr_graph {
  kr_vertex vertices[MAX_VERTICES];
  kr_edge edges[MAX_EDGES];
  kr_graph_edge_destroy_cb *edge_destroy_cb;
  kr_graph_vertex_process_cb *vertex_process_cb;
  int use_locking;
  kr_spinlock lock;
};

typedef struct {
  kr_vertex *chain[MAX_VERTICES];
  int len;
} kr_graph_chain;

static void lock_graph(kr_graph *g);
static void unlock_graph(kr_graph *g);
static int vertex_index(kr_graph *graph, kr_vertex *v);
static int visit(kr_graph *graph, uint8_t v, uint8_t *marked);
static int graph_is_cyclic(kr_graph *graph);
static int get_in_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max);
static int get_out_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max);
static int in_between_edges(kr_graph *graph, kr_vertex *to, kr_vertex *from, void **user, int max);
static int graph_output_edges(kr_graph *graph, kr_edge **edges);
static int vertex_deps_indeps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **out, int max, int type);
static int vertex_deps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **deps, int max);
static int vertex_indeps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **indeps, int max);
static void chain_push_vertex(kr_graph_chain *chain, kr_vertex *vertex);
static void chain_pop_vertex(kr_graph_chain *chain);
static int all_chains_from(kr_graph *graph, kr_vertex *from, kr_graph_chain *chains);
static int edge_destroy(kr_graph *graph, kr_edge *edge);
static kr_edge *edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from, void *user);
static int vertex_destroy(kr_graph *graph, kr_vertex *vertex);
static kr_vertex *vertex_create(kr_graph *graph, kr_vertex_type type, void *user);
static int output_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max);
static int source_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max);
static int graph_chains(kr_graph *graph, kr_vertex *to,
  kr_vertex *from, kr_chain *chains, int max);
static int graph_chains_full(kr_graph *graph, kr_vertex *to,
  kr_vertex *from, kr_chain *chains, int max);

static void lock_graph(kr_graph *g) {
  kr_spin_lock(&g->lock);
}

static void unlock_graph(kr_graph *g) {
  kr_spin_unlock(&g->lock);
}

static int vertex_index(kr_graph *graph, kr_vertex *v) {
  int8_t i;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type && (&graph->vertices[i] == v)) {
      return i;
    }
  }
  return -1;
}

static int visit(kr_graph *graph, uint8_t v, uint8_t *marked) {
  uint16_t i;
  uint8_t *adj;
  kr_vertex *ve;
  marked[v] = 1;
  ve = &graph->vertices[v];
  adj = ve->adj;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (adj[i]) {
      if (marked[i] == 1) {
        return 1;
      }
      else if (marked[i] == 0) {
        if (visit(graph, i, marked)) return 1;
      }
    }
  }
  marked[v] = 2;
  return 0;
}

static int graph_is_cyclic(kr_graph *graph) {
  uint8_t marked[MAX_VERTICES];
  uint8_t i;
  memset(marked, 0, sizeof(marked));
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type) {
      if (marked[i] == 0) {
        if (visit(graph, i, marked)) return 1;
      }
    }
  }
  return 0;
}

static int get_in_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max) {
  uint16_t i;
  int count;
  for (i = count = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].to == vertex) {
      if (graph->edges[i].user) {
        if (count == max) return count;
        user[count++] = graph->edges[i].user;
      }
    }
  }
  return count;
}

static int get_out_edges(kr_graph *graph, kr_vertex *vertex, void **user, int max) {
  uint16_t i;
  int count;
  for (i = count = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from == vertex) {
      if (graph->edges[i].user) {
        if (count == max) return count;
        user[count++] = graph->edges[i].user;
      }
    }
  }
  return count;
}

static int in_between_edges(kr_graph *graph, kr_vertex *to, kr_vertex *from, void **user, int max) {
  uint16_t i;
  int count;
  count = 0;
  for (i = 0; i < MAX_EDGES; i++) {
    if ( (graph->edges[i].from == from) && (graph->edges[i].to == to) ) {
      if (graph->edges[i].user) {
        if (count == max) return count;
        user[count++] = graph->edges[i].user;
      }
    }
  }
  return count;
}

static int graph_output_edges(kr_graph *graph, kr_edge **edges) {
  int i;
  int k;
  for (i = k = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].to != NULL) {
      if (graph->edges[i].to->type == KR_VERTEX_OUTPUT) {
        edges[k++] = &graph->edges[i];
      }
    }
  }
  return k;
}

static int distinct(kr_vertex **vertices, kr_vertex *v, int n) {
  int i;
  for (i = 0; i < n; i++) {
    if (vertices[i] == v) return 0;
  }
  return 1;
}

static int vertex_deps_indeps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **out, int max, int type) {
  kr_vertex *vertices[MAX_VERTICES];
  kr_vertex *done[MAX_EDGES];
  uint8_t *vrts;
  uint16_t k;
  int16_t i;
  //int16_t j;
  int16_t count;
  k = 0;
  count = 0;
  vertices[k++] = vertex;
  while (k) {
    vertex = vertices[--k];
    if (type == DEPS)
      vrts = vertex->deps;
    else
      vrts = vertex->adj;
    for (i = 0; i < MAX_VERTICES; i++) {
      if (vrts[i]) {
        //for (j = 0; j < vrts[i]; j++) {
        if (count >= MAX_EDGES) return -1;
        done[count++] = &graph->vertices[i];
        //}
        vertices[k++] = &graph->vertices[i];
      }
    }
  }
  k = 0;
  for (i = count-1; i >= 0; i--) {
    if (k >= max) return k;
    if (distinct(out, done[i], k)) {
      out[k++] = done[i];
    }
  }
  return k;
}

static int vertex_deps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **deps, int max) {
  if (max == 0) return 0;
  return vertex_deps_indeps(graph, vertex, deps, max, DEPS);
}

static int vertex_indeps(kr_graph *graph, kr_vertex *vertex,
  kr_vertex **indeps, int max) {
  if (max == 0) return 0;
  return vertex_deps_indeps(graph, vertex, indeps, max, INDEPS);
}

static void chain_push_vertex(kr_graph_chain *chain, kr_vertex *vertex) {
  chain->chain[chain->len++] = vertex;
}

static void chain_pop_vertex(kr_graph_chain *chain) {
  chain->len--;
}

static int all_chains_from(kr_graph *graph, kr_vertex *from, kr_graph_chain *chains) {
  kr_graph_chain ch;
  kr_graph_chain chain_stack[256];
  int i;
  int n;
  int k;
  memset(&chain_stack, 0, sizeof(chain_stack));
  n = 0;
  k = 0;
  chain_push_vertex(&chain_stack[k++], from);
  while (k) {
    ch = chain_stack[--k];
    chains[n++] = ch;
    for (i = 0; i < MAX_VERTICES; i++) {
      if (ch.chain[ch.len-1]->adj[i]) {
        if (k >= 256) return -1;
        chain_push_vertex(&ch, &graph->vertices[i]);
        chain_stack[k++] = ch;
        chain_pop_vertex(&ch);
      }
    }
  }
  return n;
}

static int edge_destroy(kr_graph *graph, kr_edge *edge) {
  if(!edge->to || !edge->from) {
    memset(edge, 0, sizeof(kr_edge));
    return 1;
  } else {
    if (graph->edge_destroy_cb) {
      graph->edge_destroy_cb(edge->user);
    }
    edge->from->adj[vertex_index(graph, edge->to)]--;
    edge->to->deps[vertex_index(graph, edge->from)]--;
    memset(edge, 0, sizeof(kr_edge));
    return 0;
  }
}

static kr_edge *edge_create(kr_graph *graph, kr_vertex *to, kr_vertex *from, void *user) {
  uint16_t i;
  if (to == NULL || from == NULL) return NULL;
  if (to == from) return NULL;
  if (to->type == 0 || from->type == 0) return NULL;
  if (from->type == KR_VERTEX_OUTPUT || to->type == KR_VERTEX_SOURCE) return NULL;
  for (i = 0; i < MAX_EDGES; i++) {
    if (!graph->edges[i].from && !graph->edges[i].to) {
      graph->edges[i].from = from;
      graph->edges[i].to = to;
      graph->edges[i].user = user;
      from->adj[vertex_index(graph, to)]++;
      to->deps[vertex_index(graph, from)]++;
      if (graph_is_cyclic(graph)) {
        printf("Cycle detected!\n");
        edge_destroy(graph, &graph->edges[i]);
        return NULL;
      } else {
        return &graph->edges[i];
      }
    }
  }
  return NULL;
}

static int vertex_destroy(kr_graph *graph, kr_vertex *vertex) {
  uint16_t i;
  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from == vertex || graph->edges[i].to == vertex) {
      if (edge_destroy(graph, &graph->edges[i])) {
        return 1;
      }
    }
  }
  memset(vertex, 0, sizeof(kr_vertex));
  return 0;
}

static kr_vertex *vertex_create(kr_graph *graph, kr_vertex_type type, void *user) {
  uint8_t i;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == 0) {
      graph->vertices[i].type = type;
      graph->vertices[i].user = user;
      return &graph->vertices[i];
    }
  }
  return NULL;
}

static int output_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max) {
  kr_vertex *indeps[MAX_VERTICES];
  int n;
  int i;
  int k;
  if (vertex == NULL) return 0;
  k = 0;
  if (vertex->type == KR_VERTEX_OUTPUT) {
    user[k++] = vertex->user;
    max--;
  }
  n = vertex_indeps(graph, vertex, indeps, max);
  for (i = 0; i < n; i++) {
    if (k >= max) return k;
    if (indeps[i]->type == KR_VERTEX_OUTPUT) {
      user[k] = indeps[i]->user;
      k++;
    }
  }
  return k;
}

static int source_users_from(kr_graph *graph, kr_vertex *vertex, void **user, int max){
  kr_vertex *deps[MAX_VERTICES];
  int n;
  int i;
  int k;
  if (vertex == NULL) return 0;
  k = 0;
  if (vertex->type == KR_VERTEX_SOURCE) {
    user[k++] = vertex->user;
    max--;
  }
  n = vertex_deps(graph, vertex, deps, max);
  for (i = 0; i < n; i++) {
    if (k >= max) return k;
    if (deps[i]->type == KR_VERTEX_SOURCE) {
      user[k] = deps[i]->user;
      k++;
    }
  }
  return k;
}

static int graph_chains(kr_graph *graph, kr_vertex *to,
  kr_vertex *from, kr_chain *chains, int max) {
  kr_graph_chain gchains[256];
  int i;
  int j;
  int n;
  int count;
  count = 0;
  if (max == 0) return 0;
  n = all_chains_from(graph, from, gchains);
  if (n >= max) return 0;
  for (i = 0; i < n; i++) {
    if (gchains[i].chain[gchains[i].len-1] == to) {
      chains[count].len = gchains[i].len;
      for (j = 0; j < gchains[i].len; j++) {
        if (j >= max) {
          chains[count].len = 0;
          return 0;
        }
        chains[count].users[j] = gchains[i].chain[j]->user;
      }
      count++;
    }
  }
  return count;
}

static int graph_chains_full(kr_graph *graph, kr_vertex *to,
  kr_vertex *from, kr_chain *chains, int max) {
  kr_graph_chain gchains[256];
  void *users[max];
  int e;
  int i;
  int j;
  int n;
  int count;
  count = 0;
  if (max == 0) return 0;
  n = all_chains_from(graph, from, gchains);
  if (n >= max) return 0;
  for (i = 0; i < n; i++) {
    if (gchains[i].chain[gchains[i].len-1] == to) {
      chains[count].len = gchains[i].len;
      e = 0;
      for (j = 0; j < gchains[i].len; j++) {
        if (j >= max) {
          chains[count].len = 0;
          return 0;
        }
        chains[count].users[j + e] = gchains[i].chain[j]->user;
        if ( (j + 1) < gchains[i].len) {
         if (in_between_edges(graph, gchains[i].chain[j+1], gchains[i].chain[j], users, max)) {
            chains[count].users[e + j + 1] = users[0];
            chains[count].len++;
            e++;
          }
        // TO-DO, handle multigraph case
        }
      }
      count++;
    }
  }
  return count;
}

static int in_out_links(kr_graph *graph, kr_vertex *vertex,
  int dir, void **user, int max) {
  int count;
  if (max == 0 || vertex == NULL) return 0;
  count = 0;
  if (dir == IN) {
    count = get_in_edges(graph, vertex, user, max);
  } else if (dir == OUT) {
    count = get_out_edges(graph, vertex, user, max);
  } else {
    count += get_in_edges(graph, vertex, user, max);
    count += get_out_edges(graph, vertex, user, max);
  }
  return count;
}

static int graph_vertices(kr_graph *graph, kr_vertex_type type,
  void **user, int max) {
  int i;
  int k;
  for (i = k = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == type) {
      if (k >= max) return k;
      user[k++] = graph->vertices[i].user;
    }
  }
  return k;
}

int kr_graph_set_user(kr_graph *graph, kr_graph_elem *elem, void *user) {
  if (graph == NULL) return 1;
  if (elem->type != KR_GRAPH_VERTEX
    || elem->type != KR_GRAPH_EDGE) return 1;
  if (graph->use_locking) lock_graph(graph);
  if (elem->type == KR_GRAPH_VERTEX) {
    elem->vertex->user = user;
  } else {
    elem->edge->user = user;
  }
  if (graph->use_locking) unlock_graph(graph);
  return 0;
}

int kr_graph_in_out_links(kr_graph *graph, kr_graph_elem *elem,
  int dir, void **user, int max) {
  int ret;
  if (graph == NULL || elem == NULL) return 0;
  if (elem->type != KR_GRAPH_VERTEX) return 0;
  if (graph->use_locking) lock_graph(graph);
  ret = in_out_links(graph, elem->vertex, dir, user, max);
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

int kr_graph_outputs(kr_graph *graph, kr_graph_elem *elem, void **user, int max) {
  int ret;
  ret = 0;
  if (max == 0) return 0;
  if (graph->use_locking) lock_graph(graph);
  if (elem == NULL) {
    ret = graph_vertices(graph, KR_VERTEX_OUTPUT, user, max);
  } else {
    if (elem->type == KR_GRAPH_VERTEX) {
      ret = output_users_from(graph, elem->vertex, user, max);
    } else if (elem->type == KR_GRAPH_EDGE) {
      ret = output_users_from(graph, elem->edge->to, user, max);
    }
  }
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

int kr_graph_sources(kr_graph *graph, kr_graph_elem *elem, void **user, int max) {
  int ret;
  ret = 0;
  if (max == 0) return 0;
  if (graph->use_locking) lock_graph(graph);
  if (elem == NULL) {
    ret = graph_vertices(graph, KR_VERTEX_SOURCE, user, max);
  } else {
    if (elem->type == KR_GRAPH_VERTEX) {
      ret = source_users_from(graph, elem->vertex, user, max);
    } else if (elem->type == KR_GRAPH_EDGE) {
      ret = source_users_from(graph, elem->edge->from, user, max);
    }
  }
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

int kr_graph_chains(kr_graph *graph, kr_graph_elem *to,
  kr_graph_elem *from, kr_chain *chains, int max) {
  int ret;
  ret = 0;
  if (graph == NULL || chains == NULL) return 0;
  if (to == NULL || from == NULL) return 0;
  if (max > MAX_USERS) return 0;
  if (to->type != KR_GRAPH_VERTEX || from->type != KR_GRAPH_VERTEX) return 0;
  if (graph->use_locking) lock_graph(graph);
  ret = graph_chains(graph, to->vertex, from->vertex, chains, max);
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

int kr_graph_chains_full(kr_graph *graph, kr_graph_elem *to,
 kr_graph_elem *from, kr_chain *chains, int max) {
  int ret;
  ret = 0;
  if (graph == NULL || chains == NULL) return 0;
  if (to == NULL || from == NULL) return 0;
  if (max > MAX_USERS) return 0;
  if (to->type != KR_GRAPH_VERTEX || from->type != KR_GRAPH_VERTEX) return 0;
  if (graph->use_locking) lock_graph(graph);
  ret = graph_chains_full(graph, to->vertex, from->vertex, chains, max);
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

kr_graph_elem kr_graph_link(kr_graph *graph, kr_graph_elem *to, kr_graph_elem *from, void *user) {
  kr_graph_elem elem;
  memset(&elem, 0, sizeof(elem));
  if (graph == NULL) return elem;
  if (to == NULL || from == NULL) return elem;
  if (to->type != KR_GRAPH_VERTEX || from->type != KR_GRAPH_VERTEX) return elem;
  if (graph->use_locking) lock_graph(graph);
  elem.edge = edge_create(graph, to->vertex, from->vertex, user);
  if (elem.edge) elem.type = KR_GRAPH_EDGE;
  if (graph->use_locking) unlock_graph(graph);
  return elem;
}

int kr_graph_remove(kr_graph *graph, kr_graph_elem *elem) {
  int ret;
  ret = 0;
  if (graph == NULL) return 1;
  if (elem->vertex == NULL || elem->edge == NULL) return 1;
  if (graph->use_locking) lock_graph(graph);
  if (elem->type == KR_GRAPH_VERTEX) {
    ret = vertex_destroy(graph, elem->vertex);
  } else if (elem->type == KR_GRAPH_EDGE) {
    ret = edge_destroy(graph, elem->edge);
  } else {
    elem->type = 0;
  }
  if (graph->use_locking) unlock_graph(graph);
  return ret;
}

kr_graph_elem kr_graph_vertex(kr_graph *graph, kr_vertex_type type, void *user) {
  kr_graph_elem elem;
  memset(&elem, 0, sizeof(elem));
  if (graph == NULL || type == 0) return elem;
  if (graph->use_locking) lock_graph(graph);
  elem.vertex = vertex_create(graph, type, user);
  if (elem.vertex) elem.type = KR_GRAPH_VERTEX;
  if (graph->use_locking) unlock_graph(graph);
  return elem;
}

int kr_graph_destroy(kr_graph *graph) {
  int i;
  int e;
  kr_edge *output_edges[MAX_EDGES];
  if (graph == NULL) return 1;
  if (graph->use_locking) lock_graph(graph);
  e = graph_output_edges(graph, output_edges);
  for (i = 0; i < e; i++) {
    if (output_edges[i])
      edge_destroy(graph, output_edges[i]);
  }
  for (i = 0; i < MAX_EDGES; i++) {
    edge_destroy(graph, &graph->edges[i]);
  }
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == KR_VERTEX_BUS)
      vertex_destroy(graph, &graph->vertices[i]);
  }
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == KR_VERTEX_SOURCE)
      vertex_destroy(graph, &graph->vertices[i]);
  }
  for (i = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type == KR_VERTEX_OUTPUT)
      vertex_destroy(graph, &graph->vertices[i]);
  }
  free(graph);
  return 0;
}

kr_graph *kr_graph_create(kr_graph_setup *setup) {
  kr_graph *graph;
  if (setup == NULL) return NULL;
  graph = calloc(1, sizeof(kr_graph));
  graph->edge_destroy_cb = setup->edge_destroy_cb;
  graph->vertex_process_cb = setup->vertex_process_cb;
  if (setup->use_locking) graph->use_locking = 1;
  kr_spin_init(&graph->lock);
  return graph;
}
