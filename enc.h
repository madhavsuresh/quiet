#include <stdint.h>

extern char * RAND_PATH; 
static const int MAX_LEN = 4096;

struct packet{
    int len;
    char buf[];
} typedef packet_t;

packet_t * enc_msg(char * msg, int len);
packet_t * make_packet(char * input_buf,int len);
void enc_packet(packet_t * pak, packet_t * rand);
void destroy_packet(packet_t * pak);
int32_t read_header(int f);
packet_t * get_rand_pak(int len);
