#ifndef __HOST_UTILS_H
#define __HOST_UTILS_H

int get_local_ip(char *buffer);

char* dns(char* hostname);

int rdns(const char* ipv4, char* buffer);

#endif
