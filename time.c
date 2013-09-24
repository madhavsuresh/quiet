//Madhav A. Suresh
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/ip.h>
#include <sys/epoll.h>
#include <string.h>
#include <arpa/inet.h>
#include "enc.h"

const char * const x[] = {"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
const char * MESSAGE = "HELLO WORLD";
static const int BACKLOG = 10;
static const int PORT = 8000;

void setup(int * fd, struct sockaddr_in ** s){

    int f,t;
    struct sockaddr_in * i = malloc(sizeof(struct sockaddr_in));

    if( (f = socket(AF_INET,SOCK_STREAM,0)) == -1){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    t = 1;
    setsockopt(f,SOL_SOCKET,SO_REUSEADDR,&t,sizeof(int));

    i->sin_family = AF_INET;
    i->sin_port = htons(PORT);
    i->sin_addr.s_addr = INADDR_ANY;
    fflush(0);

    if( bind(f,(const struct sockaddr *)i,sizeof(struct sockaddr_in)) == -1){
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if(listen(f,BACKLOG) == -1){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    *s = i;
    *fd = f;
    return;
} 

void tear_down(int socket,int epollfd,struct sockaddr_in * s){

    close(socket);
    close(epollfd);
    free(s);
}

void challenge_auth(){

}

int main(){


    RAND_PATH = "rand.1";
    int listen_sock,conn_sock,epollfd,nfds,n;
    char * recv_buf = malloc(MAX_LEN);
    struct sockaddr_in * s;
    struct epoll_event ev, events[BACKLOG];
    int addrlen = sizeof(struct sockaddr *);
    size_t rlen;
    setup(&listen_sock, &s);

    if( (epollfd = epoll_create1(0)) == -1){
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;
    ev.data.fd = listen_sock;
    if( epoll_ctl(epollfd,EPOLL_CTL_ADD,listen_sock,&ev) == -1){
        perror("epoll_ctl: listen_sock");
        exit(EXIT_FAILURE);
    }

    for(;;){
        nfds = epoll_wait(epollfd,events,BACKLOG,-1);
        if(nfds == -1){
            perror("epoll_wait");
            exit(EXIT_FAILURE);
        }
        fflush(0);

        for (n=0;n<nfds; ++n){
            if(events[n].data.fd == listen_sock){
                conn_sock = accept4(listen_sock,s,&addrlen,0);

                if(conn_sock == -1){
                    perror("accept");
                    exit(EXIT_FAILURE);
                }

		ev.events = EPOLLIN;
		ev.data.fd = conn_sock;
		epoll_ctl(epollfd,EPOLL_CTL_ADD,conn_sock,&ev);
		printf("got con");
		
		challenge_auth();
		


            }else{

		    //recieving from connection
		rlen = recv(events[n].data.fd,recv_buf,MAX_LEN,0);
		packet_t * p = enc_msg(recv_buf,rlen);
		//#printf("ENCRYPTED MESSAGE %s",p->buf);
		printf("%s",p->buf);

		//printf("unhandled");
		//exit(EXIT_FAILURE);

            }
        }
}
    

    /*
    struct tm tm;
    time_t t;

    t = time(&t);
    localtime_r(&t,&tm);
    printf("The Day %s",x[tm.tm_wday]);
    */
    tear_down(listen_sock,epollfd,s);
}


