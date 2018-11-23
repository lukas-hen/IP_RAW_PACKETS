#include "packet_maker.h"

#define MAX_CMP_WORDS 15
#define DG_BUFFER 64
#define IFACE_IP "127.0.0.1"


int main(int argc, char * argv[]){


	// Make Buffer for datagram and cast to the IP_header struct (defined in packet_maker.h).
	char datagram[DG_BUFFER];
	// Following line assumes header is always 20 bytes. Note that i am referring to payload which in this scope would include headers from higher level protocols of the OSI stack.
	char payload_buffer[DG_BUFFER-20];
	struct IP_header * iph = (struct IP_header *) datagram;

	//Zero the buffer.
	memset(datagram, 0, DG_BUFFER);

	//Set number of words in header. 5 for standard 20 byte header.
	int nwords = 5;



	//Set values of IP header. See rfc791 for meaning of all values.
	//Also sets default fallback values if flags are not specified for a specific header field.
	iph->tos = 0;		// 0 is normal service. 
	iph->ihl = nwords;		// 5 is default header without options.
	iph->version = 4;		// 4 is ipv4, Ipv6 not implemented.
	iph->total_len = 20;		// 20 bytes if just header
	iph->identification = 65535;		// Max value for recognization
	iph->flags = 2;		// 3 bits 0b010 (Do not fragment, and no more fragments.)
	iph->offset = 0;		// No offset unless fragmentation.
	iph->ttl = 128;		// 8 bits 0x80 = 128
	iph->protocol = 4;		// 4 for ipv4 
	iph->src_address = inet_addr("127.0.0.1");		// 32 bits, fallback
	iph->dst_address = inet_addr("127.0.0.1");		// 32 bits, fallback to localhost

	//Update header based on options.
	iph = parse_options_and_modify_struct(argc, argv, iph);

	//Calculate and update checksum based on updated header (from options)
	//iph->checksum = csum(datagram, nwords);

	// Create socket.
	int sd;
	sd = socket(PF_INET, SOCK_RAW, IPPROTO_RAW);
	if(sd < 0){
		perror("Error creating socket.");
		exit(0);
	}

	// Set the socket to include a custom header.
	int on = 1;
	if(setsockopt(sd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0){
		perror("setsockopt");
		close(sd);
		exit(0);
	}

	//setup src socket details.
	struct sockaddr_in sin;
	sin.sin_family = PF_INET;
	sin.sin_addr.s_addr = inet_addr(IFACE_IP); // Can be set to your interface ip with src_address being somethin else if you want to forge packets.

	//Read data to be sent from stdin (Data will have to be wrapped in higher level protocol)
	read(STDIN_FILENO, payload_buffer, sizeof(payload_buffer));


	//Append to buffer after header.
	memcpy((datagram + 20), payload_buffer, sizeof(payload_buffer));

	

	if(sendto(sd, datagram, sizeof(datagram), 0, (struct sockaddr *)&sin, sizeof(sin)) < 0){
		perror("sendto");
		close(sd);
		exit(0);
	}
	
	close(sd);
	return 0;
}
