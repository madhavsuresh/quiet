#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "enc.h"


void tst_case1(){
    char x[] = "hello world";
    packet_t * p = make_packet(x,strlen(x));
    packet_t * r = make_packet(x,strlen(x));
    enc_packet(p,r);
    assert(strcmp("",p->buf) == 0);
    enc_packet(p,r);
    assert(strcmp(x,p->buf) == 0);
    destroy_packet(p);
    destroy_packet(r);
    printf("1..");
}

void tst_case2(){
    char x[] = "hello world";
    packet_t * p = make_packet(x,strlen(x));
    packet_t * r = make_packet(x,strlen(x));
    enc_packet(p,r);
    assert(strcmp(x,p->buf) != 0);
    enc_packet(p,r);
    assert(strcmp("",p->buf) != 0);
    destroy_packet(p);
    destroy_packet(r);
    printf("2..");
}

void tst_case3(){
    char x[] = "hello world";
    packet_t * p = make_packet(x,strlen(x));
    packet_t * r = get_rand_pak(strlen(x));
    enc_packet(p,r);
    assert(strcmp(x,p->buf) != 0);
    enc_packet(p,r);
    assert(strcmp(x,p->buf) == 0);
    destroy_packet(p);
    destroy_packet(r);
    printf("3..");
}

void tst_case4(){
    char tst1[128];
    char tst2[256];
    int f = open(RAND_PATH,O_RDWR);
    int32_t pos = 0;
    assert(write(f,&pos,sizeof(int32_t)) == sizeof(int32_t));
    read(f,tst1,128);
    close(f);
    packet_t * pak = get_rand_pak(128);
    assert(strncmp(pak->buf,tst1,128) == 0);
    destroy_packet(pak);
    printf("4..");
}

void tst_case5(){
	char x[] = "1234567890";
	int32_t pos = 0;
	int f = open(RAND_PATH,O_RDWR);
	assert(write(f,&pos,sizeof(int32_t)) == sizeof(int32_t));
	close(f);
	enc_msg(x,strlen(x));
	f = open(RAND_PATH,O_RDWR);
	read(f,&pos,sizeof(int32_t));
	assert(pos == 10);
	close(f);
	printf("5..");
}

void run_tests(){

    tst_case1();
    tst_case2();
    tst_case3();
    tst_case4();
    tst_case5();
}
int main(){
	run_tests();
	return 0;

}
