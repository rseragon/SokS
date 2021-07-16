#ifndef __UNIT_TESTS_H

#include "../SokS-base/dbg.h" // I should fix this 
#include <stdio.h>
#include <stdlib.h>


#define run_test(FUNC, MESSAGE) printf("Running test ---%s\n", " "#FUNC);\
  if(FUNC() != 0) fprintf(stderr, " " MESSAGE "\n");

int test_run; // No. of tests run

#endif


