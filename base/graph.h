// TODO: Swtich to a file descriptor if possible; add error macros
#ifndef __GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

#define graph_create() if(GRAPH) {\
  verbose("Creating graph");\
  FILE* graph = fopen("graph.dot", "w+");\
  error_exit((graph != NULL), "Failed to create graph");\
  fprintf(graph, "digraph G {\n");\
  fclose(graph);\
}

// Since the node connects to the main
// NODE -> MAIN
#define graph_write(NODE, MAIN) if(GRAPH) {\
  verbose("Writing to graph");\
  FILE *graph = fopen("graph.dot", "a+");\
  error_exit((graph != NULL), "Failed to write to graph");\
  fprintf(graph, "\"%s\" -> \"%s\"\n", NODE, MAIN);\
  fclose(graph);\
}

#define graph_end() if(GRAPH) {\
  verbose("ending graph dot file");\
  FILE *graph = fopen("graph.dot", "a+");\
  error_exit((graph != NULL), "Failed to write to graph file");\
  fprintf(graph, "\n}");\
  fclose(graph);\
}

#define graph_build(GRAPH_NAME) if(GRAPH) {\
  verbose("Building the graph");\
  char *graph_args[] = {"fdp", "graph.dot", "-Tjpg", "-o", GRAPH_NAME};\
  build_graph(5, graph_args);\
  warn((remove("graph.dot") == 0), "Failed to remove graph dot file");\
}

int build_graph(int argc, char **argv);

#endif
