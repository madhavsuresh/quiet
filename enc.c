#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "enc.h"

char * RAND_PATH = "./rand";


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

void destroy_packet(packet_t * pak){
    free(pak);
}

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