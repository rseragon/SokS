#include <stdio.h>
#include "unit_test.h"

#include <graph.h> 

// modified version of `graph_build` macro
int graph_test() {

  char *graph_args[] = {"fdp", "tests/test_graph.dot", "-Tjpg", "-o", "tests/test_graph.jpg"}; 

  int ret = build_graph(5, graph_args);

  if(ret != 0){
    return -1;
  }

  test_run++;
  return 0;

}



int main() {

  printf("Starting tests...\n");

  run_test(graph_test, "Failed unit test for graph");

  printf("Total Successful tests: %d\n", test_run);

  return 0;

}
