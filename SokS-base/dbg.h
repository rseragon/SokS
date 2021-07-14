#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>

//Debugging
#define debug_info(A, M, ...) if(!(A) && DEBUG) {\
  fprintf(stdout, "[DEBUG Info] %s:%s:%d " M "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
}

#define debug_warn(A, M, ...) if(!(A) && DEBUG) {\
  fprintf(stdout, "[DEBUG Warn] %s:%s:%d " M "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__);\
}

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
