#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "host_utils.h"
#include "dbg.h"

// Few Globals
extern int VERBOSE; // setup verbosity in this file
extern char* EXEC_NAME; // Ethe executable name

/*
 * Get Machines IP address
 */
int get_local_ip(char *buffer) {

  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  error_return((sock > 0), -1, "Failed opening socket, Unable to get the local client ip");

  const char* kGoogleDnsIp = "8.8.8.8";
  int dns_port = 53;

  struct sockaddr_in dns_serv;

  memset(&dns_serv, 0, sizeof(dns_serv));
  dns_serv.sin_family = AF_INET;
  dns_serv.sin_port = htons(dns_port);
  dns_serv.sin_addr.s_addr = inet_addr(kGoogleDnsIp);

  // connect to google dns server
  if(connect(sock, (const struct sockaddr*)&dns_serv, sizeof(dns_serv)) != 0)
    return -1;


  struct sockaddr_in name;
  socklen_t namelen = sizeof(name);

  // get socket details
  if(getsockname(sock, (struct sockaddr*)&name, &namelen) != 0)
    return -2;

  // save binary addr into buffer
  inet_ntop(AF_INET, &name.sin_addr, buffer, INET6_ADDRSTRLEN);

  close(sock);

  return 0;

}

/*
 * Resolve Hostname to IP
 */
char* dns(char* hostname) {

  struct hostent* host;
  struct in_addr** addr_list;

  host = gethostbyname(hostname);
  if((host == NULL) && (h_errno != 0)) {
    herror("[ERROR] DNS"); 
    return NULL; // Failed
  }

  if(h_errno == 0) 
    return hostname;
  
  addr_list = (struct in_addr**)host->h_addr_list;

  int a;
  for(a = 0; addr_list[a] != NULL; a++)
    return inet_ntoa(*addr_list[a]); // return the first host name

  return NULL; // Failed DNS request

}

/*
 * Resolve IP to domain name and store in buffer
 */
int rdns(const char* ipv4, char* buffer) {

  struct sockaddr_in dest;

  memset(&dest, 0, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_addr.s_addr = inet_addr(ipv4);
  dest.sin_port = 0;

  if(getnameinfo((struct sockaddr*)&dest, sizeof(dest), 
        buffer, NI_MAXHOST, NULL, 0,
        NI_NAMEREQD) != 0) {
    strcpy(buffer, "Hostname cannot be determined\n\0");
    return -1; // Failed
  }

  return 0; // Success


}
