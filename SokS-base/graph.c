#include <gvc.h>
#include "graph.h"

// Taken from dot.c graphviz doc
/*
 * Builds a graph
 */
int build_graph(int argc, char **argv) {

  graph_t *g, *prev = NULL;
  GVC_t *gvc;

  gvc = gvContext();
  gvParseArgs(gvc, argc, argv);

  while((g = gvNextInputGraph(gvc))) {

    if(prev) {
      gvFreeLayout(gvc, prev);
      agclose(prev); 
    }
    gvLayoutJobs(gvc, g);
    gvRenderJobs(gvc, g);
    prev = g;
  }
  
  return (gvFreeContext(gvc));

}
