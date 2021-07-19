#include <stdio.h>
#include "unit_test.h"

#include <graph.h> 
#include <host_utils.h>

// some globals for the functions to compile
char *EXEC_NAME = "TESTING";

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

// cidr test
int cidr_test() {

  uint32_t answer_ip = 16843009; 
  uint32_t answer_mask = 4294967295;

  char *cidr = "1.1.1.1/32";
  uint32_t ip, mask;

  cidr_to_ip(cidr, &ip, &mask);


  if(ip != answer_ip || mask != answer_mask) {
    return -1;
  }
  
  test_run++;
  return 0;
}


int main() {

  printf("Starting tests...\n");

  run_test(graph_test, "Failed unit test for graph");
  run_test(cidr_test, "Failed unit test for cidr");

  printf("Total Successful tests: %d\n", test_run);

  return 0;

}
