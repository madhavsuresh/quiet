#include "enc.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>


int main(int argc, char ** argv){

    char * buf = malloc(MAX_LEN); 
    int x = 0;
    int f,sfd,s;
    struct addrinfo hints;
    struct addrinfo *result,*rp;
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

    while( 1 ) {

	fgets(buf,MAX_LEN,stdin);
	x = strlen(buf);

	p = enc_msg(buf,x);
	chksm_packet(p);
	send(sfd,p,sizeof(packet_t) + p->len,0);

	destroy_packet(p);
    }
    free(buf);
    close(sfd);

}
