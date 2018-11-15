#include <stdio.h> // For printf
#include <stdlib.h> // For exit()
#include <string.h> // Memset
#include <sys/socket.h> // For socket
#include <netinet/tcp.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdint.h> // for u_intX_t etc.
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>




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


struct ETH_header
{
	u_int64_t dst_mac:48;
	u_int64_t src_mac:48;
	u_int16_t eth_type;
};

/* Leaving TCP for futher implementation. */

struct pseudo_header
{

	u_int32_t src_address;
	u_int32_t dst_address;
	u_int8_t reserved;
	u_int8_t protocol;
	u_int16_t tcp_seg_len;
};

struct TCP_header
{
	u_int16_t dst_port;
	u_int32_t seq_num;
	u_int32_t ack_num;
	u_int16_t off:4,
			  res:6,
			  cod:6;
	u_int16_t window;
	u_int16_t checksum;
	u_int16_t urgent;
	/* Options here if implemented */
};


/*

	-------------- DATA DUMP --------------
	int fd;

	if ((fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW)) < 0){
		perror("socket");
		close(fd);
	}

	struct ifreq if_index;

	memset(&if_index, 0, sizeof(struct ifreq));
	strncpy(if_index.ifr_name, "en0", IFNAMSIZ-1);

	if(ioctl(fd, SIOCGIFINDEX, &if_idx) < 0){
		perror("SIOCGIFINDEX");
	}

	struct ifreq if_mac;

	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, "en0", IFNAMSIZ-1);

	if(ioctl(fd, SIOCGIHWADDR, &if_mac) < 0){
		perror("SIOCGIHWADDR");
	}

	struct sockaddr_ll socket_address;

	socket_address.sll_ifindex = if_index.ifr_ifindex;

	//socket_address.sll_addr[0] = REC_MAC_ADDR;

	if (sendto(fd, iph, sizeof(iph), 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0){
		perror("Send failed");
	}

*/
