#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <unistd.h>
#include <signal.h>

#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "host_utils.h"
#include "targets.h"
#include "graph.h"
#include "dbg.h"

// Globals
short int sockfd = -1;
extern int VERBOSE; // verbosity
extern char *EXEC_NAME;
extern int GRAPH;
extern char *CLIENT_IP; // should this be a global var?


// A wrapper for scanner
int scan_wrapper(char *target, int *total_host, int scan_type, char *ports) {

  static int alive = 0;

  int hosts = -1;
  uint32_t ip, mask;

  if((hosts = cidr_to_ip(target, &ip, &mask)) != -1) {
    // cidr scan
    *total_host += hosts;
    uint32_t network = (ip & mask) + 1;

    for(int i = 0; i < hosts; i++){
      struct in_addr target_ip = {ntohl(network + i)};
      alive += scanner(inet_ntoa(target_ip), scan_type, ports);
    }

    
  }
  else if((hosts = range_to_ip(target , &ip)) != -1) {
    // Range scan
    *total_host += hosts;

    for(int i = 0; i < hosts; i++) {
      struct in_addr target_ip = {ntohl(ip + i)}; 
      alive += scanner(inet_ntoa(target_ip), scan_type, ports);
    }
  }
  else {
    // Normal scan
    *total_host += 1;
    alive += scanner(target, scan_type, ports);
  }

  return alive;

}


/*
 * The main scanner
 *
 * -1 - No port scan
 * 0 - list targets
 * 1 - tcp
 * 2 - udp
 * 3 - both
 *
 * returns No. of alive hosts
 */
int scanner(char *target, int scan_type, char *ports) {

  // -sL scan
  if(scan_type == 0) {
    printf("%s\n", target);
    return 0;
  }

  int alive = check_alive_ping(target);

  if(alive == 1) {
     
    // graph Macro
    graph_write(target, CLIENT_IP);

    // DNS
    char *ip_addr = dns(target);
    verbose("DNS: %s", ip_addr);

    // rDNS
    char HName[255]; // longest hostname size
    int ret = rdns(ip_addr, HName);
    if(ret == 0)
      verbose("rDNS: %s", HName);

    // port Scanning
    if(scan_type != -1) 
      port_scan(target, ports, scan_type);

    return 1; // host is alive
  }

  return 0; // host is not alive
}


/*
 * Checks if a host is a alive
 * 
 * returns 1 if alive
 */

int check_alive_ping(char *host_name) {

  char command[100];
  int com_ret = -1; // the return value of command

  snprintf(command, 99, "ping -c 1 \"%s\" >/dev/null 2>&1", host_name);
  com_ret = system(command);
  com_ret = com_ret/128; // since bash returns 127>>n values

  switch(com_ret) {
    
    case 0: //alive
      printf("\n%s (alive)\n",host_name);
      return 1;

    case 2: // Target seems to be down
      verbose("%s Taget seems to be down",host_name);
      return 0;

    case 4: // Name or service not known
      verbose("%s Name or service not known",host_name);
      return -1;

    default: // Target unreachable or some other error
      verbose("%s unable to reach target", host_name);
      return -2;
  }

}

/*
 * A wrapper for port scanning
 *
 */
int port_scan(char *host, char *ports, int type) {

  struct hostent *target;
  char *ipv4;

  target = gethostbyname(host);

  if(target == NULL) {
    herror("Error getting host: "); 
    exit(-__LINE__);
  }

  ipv4 = inet_ntoa( *(struct in_addr *) target->h_addr);

  printf("Scanning ports for: %s\n", ipv4); 
  printf("    PORT\t\tSTATE\n");
  if(type == 1) {

    //verbose("Starting TCP port scan");
    char *token = strtok(ports, ",");

    while(token != NULL) {
      int port = atoi(token);
      token = strtok(NULL ,",");
      //error_exit(, "Port range should be 1-65535 given: %d", port);
      if((port < 1) || (port > 65535)){ 
        error("Port range should be 1-65535 given: %d\n",port);
        continue;
      }
      verbose("Scanning port: %d", port);

      tcp_scan(ipv4, port); 
    }
  }
  else if(type == 2) {

    verbose("Starting UDP port scan");
    char *token = strtok(ports, ",");

    while(token != NULL) {
      int port = atoi(token);
      token = strtok(NULL ,",");
      //error_exit(, "Port range should be 1-65535 given: %d", port);
      if((port < 1) || (port > 65535)){ 
        error("Port range should be 1-65535 given: %d\n",port);
        continue;
      }
      verbose("Scanning port: %d", port);

      udp_scan(ipv4, port);
    }
  }
  else if(type == 3) {

    verbose("Starting TCP & UDP port scan");
    char *token = strtok(ports, ",");

    while(token != NULL) {
      int port = atoi(token);
      token = strtok(NULL ,",");
      //error_exit(, "Port range should be 1-65535 given: %d", port);
      if((port < 1) || (port > 65535)){ 
        error("Port range should be 1-65535 given: %d\n",port);
        continue;
      }
      verbose("Scanning port: %d", port);

      tcp_scan(ipv4, port);
      udp_scan(ipv4, port);
    }
  }


  return 0;
}


int tcp_scan(char *ipv4, int port)  {

  struct sockaddr_in address;
  fd_set fdset;
  struct timeval tv;

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = inet_addr(ipv4);
  address.sin_port = htons(port);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  // error return macro here
  fcntl(sockfd, F_SETFL, O_NONBLOCK);

  connect(sockfd, (struct sockaddr *) &address, (socklen_t) sizeof(address));
  FD_ZERO(&fdset);
  FD_SET(sockfd, &fdset);
  tv.tv_sec = 2;        // timeout
  tv.tv_usec = 0;

  int sel = select(sockfd +1, NULL, &fdset, NULL, &tv);

  if(sel == 1) {
    int so_error;
    socklen_t len = sizeof(so_error);

    getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &so_error, &len);

    if(so_error == 0) {
      printf("%6d/tcp\t\topen\n",port);
    }
    else {
      printf("%6d/tcp\t\tclosed\n",port);
    }
  }
  else {
    printf("%6d/tcp\t\tfiltered/closed\n",port);
  }

  close(sockfd);
  return 0;
}


void close_sock() {
  close(sockfd);
}

// Switch to select() if possible
int udp_scan(char *ipv4, int port) {

  int connfd;
  struct sockaddr_in saddr;
  struct hostent *target;

  sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  // error return macro here

  target = gethostbyname(ipv4);

  bzero((char *) &saddr, sizeof(saddr));
  saddr.sin_family = AF_INET;
  bcopy((char *) target->h_addr, (char *)&saddr.sin_addr.s_addr, target->h_length);
  saddr.sin_port = htons(port);

  size_t sendto_ret, recv_ret;
  char *msg = "NULL\0"; // NULL? 

  connfd = connect(sockfd, (struct sockaddr *)&saddr, sizeof(saddr));

  error_return((connfd == 0),-1, "Failed to Connect to host");
  // error return macro here

  sendto_ret = sendto(sockfd, msg, strlen(msg), 0,
      (struct sockaddr*)&saddr, sizeof(saddr));


  signal(SIGALRM, close_sock);

  char buff[2]; // just some random string buff to hold the incomming data

  alarm(5); // alarm to triggers close_sock for the hax
  recv_ret = recvfrom(sockfd, buff, 0, MSG_PEEK, // probably change MSG_PEEK
      (struct sockaddr*)&saddr, (socklen_t *) sizeof(saddr));

  // This is hax, since I can't find any other way
  if(errno == 111) {
    printf("%6d/udp\t\tclosed\n",port);
  }
  else if(errno == 14) {
    printf("%6d/udp\t\topen\n",port);
  }
  else if(errno == 9) {
    printf("%6d/udp\t\tfiltered/closed\n",port);
  }
  else {
    printf("%6d/udp\t\tUnable to Determine(Probably closed)\n",port);
  }

  return 0; 

}


