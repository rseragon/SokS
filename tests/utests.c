#include <stdio.h>
#include "unit_test.h"

#include <graph.h> 

// some globals for the functions to compile
char *EXEC_NAME = "TESTING";

// modified version of `graph_build` macro
int graph_test() {

  char *graph_args[] = {"fdp", "test_graph.dot", "-Tjpg", "-o", "test_graph.jpg"}; 

  int ret = build_graph(5, graph_args);

  if(ret != 0 || !fopen("test_graph.jpg", "r")) {
    return -1; 
  }

  test_run++;
  return 0;

}



int main() {

  printf("Starting tests...\n");

  run_test(graph_test, "Failed unit test for graph");

  return 0;

}
