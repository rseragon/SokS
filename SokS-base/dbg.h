#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>

// mine
#define warn(A, M, ...) if(!(A)) {\
  fprintf(stderr,"[WARN]: " M "\n", ##__VA_ARGS__);\
}

#define error_exit(A, M, ...) if(!(A)) {\
  fprintf(stderr, "[ERROR] %s:" M "\n", EXEC_NAME, ##__VA_ARGS__);\
  exit(1);\
}

#define error_return(A, R, M, ...) if(!(A)) {\
  fprintf(stderr, "[ERROR] %s:" M "\n", EXEC_NAME, ##__VA_ARGS__);\
  return R;\
}
#define verbose_file(FILE, M, ...) if(VERBOSE) {\
  fprintf(FILE,"!" M "\n", ##__VA_ARGS__);\
} 

#define verbose(M, ...) if(VERBOSE) {\
  verbose_file(stdout, M, ##__VA_ARGS__);\
}

#endif
