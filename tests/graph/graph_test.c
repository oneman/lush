#include <krad/time/timer.h>
#include <krad/graph/graph.h>
#include <krad/graph/graph.c>
#include "sys/time.h"
#include "math.h"
#include "data.h"

uint randr(unsigned int min, unsigned int max) {
  struct timeval tv;
  double scale;
  gettimeofday(&tv, NULL);
  srand(tv.tv_sec + tv.tv_usec * 1000000ul);
  scale = (double)abs((int32_t)(min - max)) / RAND_MAX;
  return min + floor(rand() * scale);;
}

int vertex_to_index(kr_graph *graph, kr_vertex *vertex) {
  int i;
  for (i = 0; i < MAX_VERTICES; i++) {
    if (vertex == &graph->vertices[i]) {
      return i;
    }
  }
  return -1;
}

void print_usage(const char *cmd) {
  printf("Usage:\n  %s num_of_vertices num_of_edges\n",cmd);
}

void print_chain(kr_graph *graph, kr_graph_chain *chain) {
  int i;
  for (i = 0; i < chain->len; i++) {
    printf("%s ",names[vertex_to_index(graph,chain->chain[i])]);
    if (i != (chain->len - 1))
      printf("--> ");
  }
}

void graph_print_chains(kr_graph *graph, kr_vertex *to, kr_vertex *from) {
  kr_graph_chain chains[MAX_VERTICES];
  int n;
  int i;
  int k;
  memset(&chains,0,sizeof(chains));
  n = all_chains_from(graph, from, chains);
  for (i = k = 0; i < n; i++) {
    if (chains[i].chain[chains[i].len-1] == to) {
      printf("%d. ",++k);
      print_chain(graph, &chains[i]);
      printf("\n");
    }
  }
}

char *kr_graph_vertex_type_to_str(kr_vertex_type type) {
  switch (type) {
    case KR_VERTEX_SOURCE: return "SOURCE";
    case KR_VERTEX_BUS: return "BUS";
    case KR_VERTEX_OUTPUT: return "OUTPUT";
    default: break;
  }
  return NULL;
}

void print_graph_info(kr_graph *graph) {
  int i;
  int j;
  int from;
  int to;
  for (i = j = 0; i < MAX_VERTICES; i++) {
    if (graph->vertices[i].type) {
      j++;
      printf("  vertex %s of type %s\n",
        names[i],kr_graph_vertex_type_to_str(graph->vertices[i].type));
    }
  }
  if (j == 0) {
    printf("  your graph is empty...\n");
    return;
  }
  for (i = 0; i < MAX_EDGES; i++) {
    if (graph->edges[i].from && graph->edges[i].to) {
      from = vertex_to_index(graph, graph->edges[i].from);
      to = vertex_to_index(graph, graph->edges[i].to);
      if (from < 0 || to < 0) break;
      printf("  we have an edge from %s %s to %s %s\n",
        kr_graph_vertex_type_to_str(graph->edges[i].from->type), names[from],
        kr_graph_vertex_type_to_str(graph->edges[i].to->type), names[to]);
    }
  }
}

void print_random_chains(kr_graph *graph, kr_vertex **vertices, int vertex_count) {
  int from;
  int to;
  if (vertex_count) {
    from = randr(0,vertex_count);
    to = randr(0,vertex_count);
    printf("Chains from %s to %s\n",names[from],names[to]);
    graph_print_chains(graph,vertices[to],vertices[from]);
  }
}

void print_random_deps_info(kr_graph *graph, kr_vertex **vertices, int vertex_count) {
    int v;
    int dep_count;
    int i;
    if (vertex_count) {
      v = randr(0,vertex_count);
      kr_vertex *deps[vertex_count];
      dep_count = vertex_deps(graph,vertices[v],deps,vertex_count);
      if (dep_count) {
        printf("Found %d deps for %s\n",dep_count,names[v]);
        for (i = 0; i < dep_count; i++) {
          printf("  %s\n",names[vertex_to_index(graph, deps[i])]);
        }
      } else {
        printf("No deps found for %s\n",names[v]);
      }
    }
}

int random_vertices_gen(kr_graph *graph, int n, kr_vertex **vertices) {
  int i;
  int k;
  for (i = k = 0; i < n; i++) {
    vertices[k] = vertex_create(graph,randr(1,4),NULL);
    if (vertices[k]) {
      k++;
    }
  }
  return k;
}

int random_edges_gen(kr_graph *graph, int n, int vcount, kr_vertex **vertices) {
  int i;
  int j;
  int k;
  int l;
  for (i = l = 0; i < n; i++) {
    j = randr(0,vcount);
    k = randr(0,vcount);
    printf("Generating random edge from %s to %s\n",names[k],names[j]);
    if (edge_create(graph,vertices[j],vertices[k],NULL)) {
      l++;
    }
  }
  return l;
}

int main(int argc, char const *argv[]) {
  kr_graph *graph;
  kr_timer *timer;
  kr_graph_setup graph_setup;
  kr_vertex *vertices[MAX_VERTICES];
  int vertex_count;
  int edge_count;
  if (argc != 3) {
    print_usage(argv[0]);
    return 0;
  }
  if (atoi(argv[1]) > MAX_VERTICES) {
    fprintf(stderr, "MAX_VERTICES (%d) exceeded\n",MAX_VERTICES);
    return 1;
  }
  if (atoi(argv[2]) > MAX_EDGES) {
    fprintf(stderr, "MAX_EDGES (%d) exceeded\n",MAX_EDGES);
    return 1;
  }
  memset(&graph_setup,0,sizeof(kr_graph_setup));
  timer = kr_timer_create();
  kr_timer_start(timer);
  graph = kr_graph_create(&graph_setup);
  if (graph == NULL) {
    fprintf(stderr, "graph creation failed!\n");
    return 1;
  }
  printf("graph created successfully!\n\n");
  vertex_count = random_vertices_gen(graph,atoi(argv[1]),vertices);
  printf("Generated %d random vertices\n\n",vertex_count);
  kr_timer_status(timer);
  edge_count = random_edges_gen(graph,atoi(argv[2]),vertex_count,vertices);
  printf("\nGenerated %d random edges\n\n",edge_count);
  kr_timer_status(timer);
  printf("\n");
  printf("graph info: \n\n");
  print_graph_info(graph);
  printf("\n");
  print_random_deps_info(graph,vertices,vertex_count);
  printf("\n");
  print_random_chains(graph,vertices,vertex_count);
  printf("\n");
  printf("destroying graph now.\n");
  kr_graph_destroy(graph);
  printf("\n");
  kr_timer_status(timer);
  kr_timer_finish(timer);
  kr_timer_destroy(timer);
  return 0;
}
