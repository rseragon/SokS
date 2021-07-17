#ifndef __HOST_UTILS_H
#define __HOST_UTILS_H

#include <stdint.h>

int get_local_ip(char *buffer);

char* dns(char* hostname);

int rdns(const char* ipv4, char* buffer);

int cidr_to_ip(const char *cidr, uint32_t *ip, uint32_t *mask);

int range_to_ip(const char *range, uint32_t *ip);

#endif
