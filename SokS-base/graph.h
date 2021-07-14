// TODO: Swtich to a file descriptor if possible , add error macros
#ifndef __GRAPH_H

#include <stdio.h>
#include <stdlib.h>

#define graph_create() if(GRAPH) {\
  verbose("Creating graph");\
  FILE* graph = fopen("graph.dot", "w+");\
  fprintf(graph, "digraph G {\n");\
  fclose(graph);\
}

// Since the node connects to the main
// NODE -> MAIN
#define graph_write(NODE, MAIN) if(GRAPH) {\
  verbose("Writing to graph");\
  FILE *graph = fopen("graph.dot", "a+");\
  fprintf(graph, "\"%s\" -> \"%s\"\n", NODE, MAIN);\
  fclose(graph);\
}

#define graph_end() if(GRAPH) {\
  verbose("ending graph dot file");\
  FILE *graph = fopen("graph.dot", "a+");\
  fprintf(graph, "\n}");\
  fclose(graph);\
}

#define graph_build(GRAPH_NAME) if(GRAPH) {\
  verbose("Building the graph");\
  char *graph_args[] = {"fdp", "graph.dot", "-Tjpg", "-o", GRAPH_NAME};\
  build_graph(5, graph_args);\
}

int build_graph(int argc, char **argv);

#endif
