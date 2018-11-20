#ifndef BLAFOO_H_IH
#define BLAFOO_H_IH
#include <ctype.h>
int check_input_validity(int input, char * hfield);
struct IP_header * parse_options_and_modify_struct(int argc, char * argv[], struct IP_header * iph);
#endif
