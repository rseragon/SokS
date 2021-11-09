#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include <omp.h>

#include "host_utils.h"
#include "dbg.h"
#include "main.h"
#include "targets.h" 
#include "helper.h"
#include "graph.h"

// Config file from CMake
#include "SoksConfig.h"

// Hated Globals
int VERBOSE = FALSE;
int DEBUG = FALSE; 
char *EXEC_NAME = NULL; // The executable name
int GRAPH = FALSE; // for graph
char *CLIENT_IP; // This machines IP

//char *VERSION = "0.8";

int main(int argc, char *argv[])
{


  // Start time
  double start = omp_get_wtime();

  EXEC_NAME = strdup(argv[0]);

  if(EXEC_NAME == NULL) {
    fprintf(stderr,"Failed to allocate memory. Try running again?\n");
    exit(-1);
  }

  char* graph_name = NULL; // Graph output name

  if(argc == 1) {
    print_help(); // prints help if no args are given
    exit(EXIT_SUCCESS);  
  } 

  // getopt variables
  int arg;
  int option_index;

  char *ports_str = NULL;
  bool scan_ports = FALSE;
  bool tcp_scan = FALSE; // default scan type
  bool udp_scan = FALSE;
  bool list_scan = FALSE;


  int total_hosts = 0;
  int alive_hosts = 0;

  static struct option long_options[] = {

    {"scanflag", required_argument, 0, 's'},
    {"verbose", no_argument, &VERBOSE, 'v'},
    {"version", no_argument, 0, 'V'},
    {"debug", no_argument, 0, 'd'},
    {"graph", required_argument, 0, 'g'},
    {"port", required_argument, 0, 'p'},
    {"help", no_argument, 0, 'h'},
    {0,0,0,0}

  };


  while((arg = getopt_long_only(argc, argv, "s:p:vVg:", long_options, &option_index)) != EOF) {

    switch(arg) {

      case 0:
        if(long_options[option_index].flag != 0)
          break;
        warn(0, "options %s",long_options[option_index].name);
        if(optarg)
          printf(" with arg %s", optarg);
        printf("\n");
        break;

      case 's':
        if(!optarg || !*optarg) {
          error_exit(0,"Scan type not provided");
        }
        else if(strcmp(optarg,"T") == 0) {
          tcp_scan = TRUE;
        } 
        else if(strcmp(optarg,"U") == 0) {
          udp_scan = TRUE;
        }
        else if(strcmp(optarg, "L") == 0) {
          list_scan = TRUE;
        }
        else {
          error_exit(0, "Unknown scan type: %s",optarg);
        }
        break;

      case 'v':
        VERBOSE = TRUE;
        break;

      case 'V':
        print_version();
        exit(0);
        break;

      case 'd':
        DEBUG = TRUE;
        break;

      case 'g':
        GRAPH = TRUE;
        error_exit((optarg), "Graph name not provided");
        graph_name = strdup(optarg);
        error_exit((graph_name != NULL), "Failed to set graph name");
        break;

      case 'p':
        if(!optarg) {
          error_exit(0, "Ports not provided");
        }
        ports_str = strdup(optarg);
        error_exit((ports_str != NULL), "Ports list is empty");
        scan_ports = TRUE;
        //puts(ports_str);
        break;

      case 'h':
        print_help();
        exit(EXIT_SUCCESS);
      case '?':
        break;

      default:
        error_exit(0, "Wot??");
    }

  }

  // Start scanning
  printf("Starting scan, Time:"); 
  print_curr_time();
  fflush(stdout); // idk why this is getting overlapped
  verbose("MAX usable threads: %d\n", omp_get_num_procs());


  if(optind < argc) {
    
    // graph macro
    graph_create();

    // Get This machine's ip
    char client_ip[20];
    int ret = get_local_ip(client_ip); // get the machine's local ip
    if(ret != 0) {
      if(ret == -1) {
        warn(0, "Failed to get this machine's ip, Are you connected to internet?");
      }
      else {
        warn((ret == -1), "Unable to get the socket details, is your machine alright?");
      }
      debug_warn(0, "Unable to get machine's ip, diabled Graph flag");
      GRAPH = FALSE;
    }
    CLIENT_IP = client_ip;
    

    while (optind < argc) {
      //total_hosts++; 

      char *h = strdup(argv[optind++]);

      if(list_scan) {
        scan_wrapper(h, &total_hosts, 0, NULL);
      }
      else {
        if(scan_ports == TRUE) {
          if(tcp_scan && udp_scan) {
            alive_hosts = scan_wrapper(h, &total_hosts, 3, ports_str);
          }
          else if(udp_scan) {
            alive_hosts = scan_wrapper(h, &total_hosts, 2, ports_str);
          }
          else {
            if(!tcp_scan)
              warn(0, "Scan Type not provided (defaulting to TCP)");
            alive_hosts = scan_wrapper(h, &total_hosts, 1, ports_str);
          }
        }
        else {
          alive_hosts = scan_wrapper(h, &total_hosts, -1, NULL);
        }
      }
    }
    
    // end graph macro
    graph_end();
    graph_build(graph_name); // build the graph


  }

  // Warn if total hosts given = 0
  warn((total_hosts != 0), "No targets given, so 0 hosts scanned");

  // End scan
  // Caculate time taken
  double time_taken = omp_get_wtime() - start;

  printf("\nScan done: %d IP address (%d alive) Time taken: %f sec\n",total_hosts, alive_hosts, time_taken);

  return 0;

}
