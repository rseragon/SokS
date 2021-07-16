#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/time.h>

#include "host_utils.h"
#include "dbg.h"
#include "main.h"
#include "targets.h" 
#include "helper.h"
#include "graph.h"

// Hated Globals
int VERBOSE = FALSE;
int DEBUG = FALSE; 
char *EXEC_NAME; // The executable name
bool GRAPH = FALSE; // for graph

char *VERSION = "0.4";

int main(int argc, char *argv[])
{

  // For calculating scanning time
  struct timeval start_time, end_time;
  gettimeofday(&start_time, NULL); // Start time

  EXEC_NAME = strdup(argv[0]);

  if(EXEC_NAME == NULL) {
    fprintf(stderr,"Failed to allocate memory. Try running again?\n");
    exit(-1);
  }

  char* graph_name = NULL; // Graph output name

  if(argc == 1) {
    print_help(); // prints help if no args are given
    exit(-1);  
  } 

  // getopt variables
  int arg;
  int option_index;

  char *ports_str = NULL;
  bool scan_ports = FALSE;
  bool tcp_scan = FALSE; // default scan type
  bool udp_scan = FALSE;


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
        else {
          error_exit(0, "Unknown scan type: %s",optarg);
        }
        break;

      case 'v':
        VERBOSE = TRUE;
        break;

      case 'V':
        print_version();
        exit(EXIT_SUCCESS);
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
    

    while (optind < argc) {
      total_hosts++; 

      char *h = argv[optind++];
      int alive = check_alive_ping(h); // check if host is alive
      if(alive == 1) 
      {
        // Graph Macro
        graph_write(h, client_ip);

        alive_hosts++;

        // DNS
        char *ip_addr = dns(h);
        verbose("DNS: %s", ip_addr);
        
        // rDNS
        char HName[255]; // longest hostname size
        int ret = rdns(ip_addr, HName);
        if(ret == 0) // Success in rDNS
          verbose("rDNS: %s",HName);

        // Port Scanning
        if (scan_ports == TRUE)
        {
          if(tcp_scan && udp_scan) {
            port_scan(h, ports_str, 3); 
          }
          else if(udp_scan) {
            port_scan(h, ports_str, 2);
          }
          else {
            if(!tcp_scan)
              warn(0, "Scan Type not provided (defaulting to TCP)");
            port_scan(h, ports_str, 1);
          }
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
  gettimeofday(&end_time, NULL);

  // Caculate time taken
  double time_taken = (end_time.tv_sec - start_time.tv_sec) +
                      (end_time.tv_usec - start_time.tv_usec)/1e6;

  printf("Scan done: %d IP address (%d alive) Time taken: %f sec\n",total_hosts, alive_hosts, time_taken);

  return 0;

}
