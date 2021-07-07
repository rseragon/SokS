#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <time.h>

#include "host_utils.h"
#include "dbg.h"
#include "main.h"
#include "targets.h" 
#include "helper.h"
#include "graph.h"

// Hated Globals
int VERBOSE = FALSE;
int DEBUG = FALSE;
char *EXEC_NAME;

char *VERSION = "0.1";

int main(int argc, char *argv[])
{


  EXEC_NAME = strdup(argv[0]);

  if(argc == 1) {
    print_help();
    exit(EXIT_SUCCESS);  
  } 

  int arg;
  int option_index;

  char *ports_str;
  bool scan_ports = FALSE;
  bool tcp_scan = FALSE; // default scan type
  bool udp_scan = FALSE;

  int total_hosts = 0;
  int alive_hosts = 0;
  bool GRAPH = FALSE; // for graph

  static struct option long_options[] = {

    {"scanflag", required_argument, 0, 's'},
    {"verbose", no_argument, &VERBOSE, 'v'},
    {"version", no_argument, 0, 'V'},
    {"debug", no_argument, 0, 'd'},
    {"graph", no_argument, 0, 'g'},
    {"port", required_argument, 0, 'p'},
    {"help", no_argument, 0, 'h'},
    {0,0,0,0}

  };


  while((arg = getopt_long_only(argc, argv, "s:p:vVg", long_options, &option_index)) != EOF) {

    switch(arg) {

      case 0:
        if(long_options[option_index].flag != 0)
          break;
        printf("options %s",long_options[option_index].name);
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
        break;

      case 'p':
        if(!optarg) {
          error_exit(0, "Ports not provided");
        }
        ports_str = strdup(optarg);
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
  clock_t start = clock();  // Start time
  printf("Starting scan, Time:"); 
  print_curr_time();


  if(optind < argc) {
    
    // graph macro
    graph_create();
    char client_ip[20];
    get_local_ip(client_ip); // get the machine's local ip

    while (optind < argc) {
      total_hosts++; 

      char *h = argv[optind++];
      int alive = check_alive_ping(h); // check if host is alive
      if(alive == 1) 
      {
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
      else {
        verbose("%s (down)", h);
      }
    }
    //printf("\n");
    
    // end graph macro
    graph_end();
    graph_build(); // build the graph


  }

  // Warn if total hosts given = 0
  warn((total_hosts != 0), "No targets given, so 0 hosts scanned");
  // End scan
  clock_t end = clock();
  // Fix time taken
  printf("Scan done: %d IP address (%d alive) Time taken: %f\n",total_hosts, alive_hosts, (double)(end-start)/CLOCKS_PER_SEC);

  return 0;

}
