#include "packet_maker.h"


unsigned short csum(unsigned short *buf, int nwords)
//FORKED: https://www.tenouk.com/Module43a.html
{
        unsigned long sum;
        for(sum=0; nwords>0; nwords--)
                sum += *buf++;
        sum = (sum >> 16) + (sum &0xffff);
        sum += (sum >> 16);
        return (unsigned short)(~sum);
}


int main(){


	char datagram[64];

	//struct ETH_header * eth = (struct ETH_header *) datagram;
	struct IP_header * iph = (struct IP_header *) datagram; //+ sizeof(struct ETH_header);
	int iph_size = sizeof(struct IP_header);
	int eth_size = sizeof(struct ETH_header);


	//zero buf.
	memset(datagram, 0, 64);

	//IP values
	iph->tos = 0b0000;			// 8 bits 0b0000 is normal service
	iph->ihl = 0x5;					// 4 bits 0x5 is hlen = 20
	iph->version = 0x4;				// 4 bits 0x4 is IPv4
	iph->total_len = 0x0014;		// 16 bits 0x14 is 20 bytes.
	iph->identification = 0xFFFF;	// 16 bits id only ones.
	iph->flags = 0b010;				// 3 bits 0b010 (Do not fragment, and no more fragments.) GCC specific
	iph->offset = 0b0000000000000;	// 13 bits 13 zeros, GCC specific
	iph->ttl = 0x80;				// 8 bits 0x80 = 128
	iph->protocol = 0x06;			// 8 bits
	iph->checksum = 0xFFFF;			// 16 bits forked csum can be used for valid csum.
	iph->src_address = inet_addr("123.123.123.123");	// 32 bits spoofed. 123.123.123.123
	iph->dst_address = inet_addr("127.0.0.1");	// 32 bits localhost: 127.0.0.1

	/* For linux with af_packet 
	//ETH values
	eth->dst_mac = 0xFFFFFFFFFFFF;
	eth->src_mac = 0x000000000000;
	eth->eth_type = 0x0800; //IPv4
	*/


	int sd;
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
	if(sd < 0){
		perror("Error creating socket.");
		exit(0);
	}

	int on = 1;
	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
		perror("setsockopt");
		close(sd);
		exit(0);
	}

	struct sockaddr_in sin;
	sin.sin_family = PF_INET;
	sin.sin_port = htons(9999);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	

	if(sendto(sd, datagram, sizeof(datagram), 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("sendto");
		close(sd);
		exit(0);
	}
	
	close(sd);
	return 0;
}