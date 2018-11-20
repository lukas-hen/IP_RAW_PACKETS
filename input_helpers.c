#include "input_helpers.h"
#include "packet_maker.h"

#define MAX_CMP_WORDS 15



int check_input_validity(int input, char * hfield)
//Checks validity of header values.
{
	if((strncmp(hfield, "identification", MAX_CMP_WORDS) == 0)){
		if(input >= 0 && input <= 65535){
			return 1;
		}else{
			printf("Error: invalid header field \'identification\'.\n");
			exit(0);
		}
	}

	if((strncmp(hfield, "version", MAX_CMP_WORDS) == 0)){
		if(input >= 4 && input <= 9){
			return 1;
		}else{
			printf("Error: invalid header field \'version\'.\n");
			exit(0);
		}
	}

	if((strncmp(hfield, "flags", MAX_CMP_WORDS) == 0)){
		if(input >= 0 && input <= 3){
			return 1;
		}else{
			printf("Error: invalid header field \'flags\'.\n");
			exit(0);
		}
	}

	if((strncmp(hfield, "offset", MAX_CMP_WORDS) == 0)){
		if(input >= 0 && input <= 8191){
			return 1;
		}else{
			printf("Error: invalid header field \'offset\'.\n");
			exit(0);
		}
	}


	if((strncmp(hfield, "ttl", MAX_CMP_WORDS) == 0)){
		if(input >= 0 && input <= 255){
			return 1;
		}else{
			printf("Error: invalid header field \'ttl\'.\n");
			exit(0);
		}
	}


	if((strncmp(hfield, "protocol", MAX_CMP_WORDS) == 0)){
		if(input >= 0 && input <= 254){
			return 1;
		}else{
			printf("Error: invalid header field \'protocol\'.\n");
			exit(0);
		}
	}

	perror("Error: invalid header field name.");
	exit(0);
	return 0; //For compiler to stop whining.
}


struct IP_header * parse_options_and_modify_struct(int argc, char * argv[], struct IP_header * iph)
{

	int c;
	opterr = 0;
	while ((c = getopt (argc, argv, "d:s:i:v:f:o:t:p:")) != -1){
		int optarg_num = atoi(optarg);
		switch (c)
		{
			case 'd':
				iph->dst_address = inet_addr(optarg);
				break;
			case 's':
				iph->src_address = inet_addr(optarg);
				break;
			case 'i':
				if(check_input_validity(optarg_num, "identification") == 1){iph->identification = optarg_num;}
				break;
			case 'v':
				if(check_input_validity(optarg_num, "version") == 1){iph->version = optarg_num;}
				break;
			case 'f':
				if(check_input_validity(optarg_num, "flags") == 1){iph->flags = optarg_num;}
				break;
			case 'o':
				if(check_input_validity(optarg_num, "offset") == 1){iph->offset = optarg_num;}
				break;
			case 't':
				if(check_input_validity(optarg_num, "ttl") == 1){iph->ttl = optarg_num;}
				break;
			case 'p':
				if(check_input_validity(optarg_num, "protocol") == 1){iph->protocol = optarg_num;}
				break;
			case '?':
				if (optopt == 'c')
				{
				fprintf(stderr, "Option -%c requires an argument.\n", optopt);
				}
				else if (isprint (optopt)){
				fprintf(stderr, "Unknown option `-%c'.\n", optopt);
				}
				else{
				fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
				}
			default:
				abort ();
		}
	}
	return iph;
}


