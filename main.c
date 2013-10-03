#include "enc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>

static const int BACKLOG = 10;

int main(int argc, char ** argv){

    unsigned char * recv_buf = malloc(MAX_LEN); 
    int x = 0;
    int f,sfd,s,epollfd,n,nfds;
    struct addrinfo hints;
    struct addrinfo *result,*rp;
    struct epoll_event events[BACKLOG];
    packet_t * p;
    RAND_PATH = argv[1];


    //networking client setup
    memset(&hints,0,sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    if ( (s = getaddrinfo(argv[2],argv[3],&hints,&result)) != 0){
	    fprintf(stderr,"getaddrinfo: %s \n",gai_strerror(s));
	    exit(EXIT_FAILURE);
    }
    
    for(rp = result; rp != NULL; rp = rp->ai_next){

	    sfd = socket(rp->ai_family,rp->ai_socktype,
			    rp->ai_protocol);
	    if(sfd == -1)
		    continue;

	    if(connect(sfd,rp->ai_addr, rp->ai_addrlen) != -1){
		    break;
	    }
	    printf("wat");
	    close(sfd);
    }

    if ( rp == NULL){
	    perror("Could not connect\n");
	    exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    epollfd = setup_epoll();
    add_epollin(epollfd,sfd);
    add_epollin(epollfd,0);

    for(;;){
	    int rlen;
	    nfds = epoll_wait(epollfd,events,BACKLOG,-1);
	    if(nfds == -1){
		perror("epoll_wait");
		exit(EXIT_FAILURE);
	    }
	for( n = 0;n<nfds;++n){
		if(events[n].data.fd == sfd){

				packet_t * p = get_message(events[n].data.fd,recv_buf);

				if( p != NULL){
					print_out_message(p);
					bzero(recv_buf,MAX_LEN);
				}

		}else if(events[n].data.fd == 0){
			packet_t * p;

			rlen = read(0,recv_buf,MAX_LEN);
			p = enc_msg(recv_buf,rlen);
			chksm_packet(p);
			
			if (send(sfd,p,sizeof(packet_t) + rlen,0) < 0){
				perror("send error!");

			}
			destroy_packet(p);

		}
	}

    }

    /*
    while( 1 ) {

	fgets(buf,MAX_LEN,stdin);
	x = strlen(buf);

	p = enc_msg(buf,x);
	chksm_packet(p);
	send(sfd,p,sizeof(packet_t) + p->len,0);

	destroy_packet(p);
    }
    */
    close(sfd);

}
