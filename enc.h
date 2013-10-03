#include <stdint.h>
#include <openssl/sha.h>

extern char * RAND_PATH; 
static const int MAX_LEN = 4096;

struct packet{
    int len;
    unsigned char digest[SHA_DIGEST_LENGTH];
    unsigned char buf[];
} typedef packet_t;

packet_t * enc_msg(unsigned char * msg, int len);
packet_t * make_packet(unsigned char * input_buf,int len);
void enc_packet(packet_t * pak, packet_t * rand);
void destroy_packet(packet_t * pak);
int32_t read_header(int f);
packet_t * get_rand_pak(int len);
int ckcksum(packet_t * pak);
int setup_epoll();
void add_epollin(int epollfd,int fd);
packet_t * get_message(int fd, unsigned char * recv_buf);
void print_out_message(packet_t * p);
void chksm_packet(packet_t * pak);
