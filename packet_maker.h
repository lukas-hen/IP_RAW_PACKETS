#ifndef BLAFOO_H_PM
#define BLAFOO_H_PM

#include <stdio.h> // For printf
#include <stdlib.h> // For exit()
#include <string.h> // Memset
#include <sys/socket.h> // For socket
#include <netinet/in.h> // For sockaddr_in on other platforms than osx.
#include <arpa/inet.h> // For inet_addr function
#include <unistd.h>
#include <stdint.h> // for u_intX_t etc.

//Defined here aswell for non implicit declaration. (Invalid in C99)
struct IP_header * parse_options_and_modify_struct(int argc, char * argv[], struct IP_header * iph);


struct IP_header
{
	u_int16_t ihl:4,
			  version:4,
			  tos:8;
	u_int16_t total_len;
	u_int16_t identification;
	u_int16_t offset:13,
			  flags:3;
	u_int8_t ttl;
	u_int8_t protocol;
	u_int16_t checksum;
	u_int32_t src_address;
	u_int32_t dst_address;
	/* Options here if implemented */
};


#endif


