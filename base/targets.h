#ifndef __TARGETS_H
#define __TARGETS_H

int check_alive_ping(char *ipv4);

int port_scan(char *host, char *ports, int type);

int tcp_scan(char *ipv4, int port);

int udp_scan(char *ipv4, int port);

#endif
