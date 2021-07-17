#include <stdio.h>
#include <time.h>
#include "helper.h"

extern char *EXEC_NAME;
extern char *VERSION;

/*
 * Prints help
 */
void print_help() {

  printf("SokS version %s\n", VERSION);
  printf("Usage: %s [Scan Types(s)] [Options] {Targets}\n",EXEC_NAME);

  printf("TARGET:\n\
      can pass hostnames, IP, CIDR addresses, IP ranges(Only to the last octet)\n\
      Example: google.com, 192.168.0.1, 10.10.10.10/24, 1.1.1.1-10\n");

  printf("SCAN TECHNIQUES:\n\
      -sU: UDP Scan (Slow/Unreliable)\n\
      -sT: TCP Scan\n\
      -sL: List the hosts\n");

  printf("PORT SPECIFICATIONS:\n\
      -p <comma seperated ports>\n\
      Example -p22; -p22,80,443\n");

  printf("GRAPH:\n\
      -g <GraphName>.jpg\n\
      Note: The extension name doesn't matter\n");

  printf("MISC:\n\
      -V: Prints version\n\
      -v: Increases Verbosity\n\
      -d: Enables Debugging\n\
      -h: Prints This help Summary\n");
  
  printf("EXAMPLES:\n\
      %s -v google.com\n\
      %s -sT google.com -p 80,443\n\
      %s -sU 192.168.0.1 -p 21,23\n\
      %s 192.168.0.2/24 -g graph.jpg\n", EXEC_NAME, EXEC_NAME, EXEC_NAME, EXEC_NAME);



}

/*
 * print version and other info
 */
void print_version() {

  printf("SokS version %s\n", VERSION);

  // COPYRIGHT and author here

}

/*
 * Prints Current Time
 */
void print_curr_time() {

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  tzset();
  printf(" %d-%02d-%02d %02d:%02d:%02d %s\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec, tzname[1]);

}
