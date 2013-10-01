#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <openssl/sha.h>
#include <sys/epoll.h>
#include "enc.h"

char * RAND_PATH = "./rand";

int setup_epoll(){

	int epollfd;
	if( (epollfd = epoll_create1(0)) == -1){
		perror("epoll_create1");
		exit(EXIT_FAILURE);
	}
	return epollfd;
}

void add_epollin(int epollfd,int fd){

	struct epoll_event ev;

	ev.events = EPOLLIN;
	ev.data.fd = fd;

	if( epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&ev) == -1){
		perror("epoll_ctl");
		exit(EXIT_FAILURE);
	}

}

void enc_packet(packet_t * pak, packet_t * rand){

    int i,len;
    len = pak->len;
    assert(pak->len <= MAX_LEN);
    assert(pak->len == rand->len);

    for(i=0;i<len;i++){
        pak->buf[i] = pak->buf[i] ^ rand->buf[i];
    }

}

packet_t * make_packet(char * input_buf,int len){

    packet_t * pak = malloc(sizeof(packet_t) + len + 1);
    pak->len = len;
    strncpy(pak->buf,input_buf,len);
    return pak;
}

int32_t read_header(int f){

    int32_t pos;
    int len = sizeof(int32_t);

    if( read(f, &pos, len) != len ){
        perror("unable to read first byte from rand");
        exit(0);
    }
    return pos;
}

void write_header(int f,int32_t pos){

    lseek(f,0,SEEK_SET);
    write(f,&pos,sizeof(int32_t));
    return;
}

packet_t * get_rand_pak(int len){

    int f = open(RAND_PATH,O_RDWR);
    int32_t pos = read_header(f);
    lseek(f,pos,SEEK_CUR);
    packet_t * pak = make_packet("",len);
    if( read(f,pak->buf,len) != len){
        perror("unable to read bytes from rand");
        exit(0);
    }
    pos += len;
    write_header(f,pos);
    close(f);
    return pak;
}

int ckcksum(packet_t * pak){

    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(pak->buf,pak->len-1,digest);
    if(strncmp(pak->digest,digest,SHA_DIGEST_LENGTH) != 0){
	    printf("error in transit");
	    return 1;
    }
    return 0;

}
void chksm_packet(packet_t * pak){

    SHA1(pak->buf,pak->len-1,pak->digest);
}

void destroy_packet(packet_t * pak){
    free(pak);
}

//this seems kinda dumb
packet_t * enc_msg(char * msg, int len){
    packet_t * p = make_packet(msg,len);
    packet_t * r = get_rand_pak(len);
    enc_packet(p,r);
    destroy_packet(r);
    return p;


}

void write_out(packet_t * pak){
    fwrite(pak,sizeof(packet_t),1,stdout);

}
