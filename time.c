//Madhav A. Suresh
#include <time.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
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
	unsigned char * recv_buf = malloc(MAX_LEN);
	struct sockaddr_in * s;
	struct epoll_event ev, events[BACKLOG];
	socklen_t addrlen = sizeof(struct sockaddr *);
	size_t rlen;
	conn_sock = -1;

	setup(&listen_sock, &s);

	epollfd = setup_epoll();


	add_epollin(epollfd,listen_sock);
	//really should only happen after being connected to...
	add_epollin(epollfd,0);

	for(;;){
		nfds = epoll_wait(epollfd,events,BACKLOG,-1);
		fflush(0);
		if(nfds == -1){
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}
		for (n=0;n<nfds; ++n){
			if(events[n].data.fd == listen_sock){

				if(conn_sock != -1)
					continue;

				conn_sock = accept(listen_sock,(struct sockaddr *)s,&addrlen);

				if(conn_sock == -1){
					perror("accept");
					exit(EXIT_FAILURE);
				}

				add_epollin(epollfd,conn_sock);
				challenge_auth();

			}else if( events[n].data.fd == 0){
				packet_t * p;

				rlen = read(0,recv_buf,MAX_LEN);
				p = enc_msg(recv_buf,rlen);
				chksm_packet(p);

				send(conn_sock,p,sizeof(packet_t) + rlen,0);
				destroy_packet(p);


			}else{

				packet_t * p = get_message(events[n].data.fd,recv_buf);

				if( p != NULL){
					print_out_message(p);
					bzero(recv_buf,MAX_LEN);
				}
			}
		}
	}

	tear_down(listen_sock,epollfd,s);
}


