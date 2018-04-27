#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<netdb.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include"socketconstants.h"

// Partially Referred from Beej's Tutorial
void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[]){
	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct  addrinfo hints, *servinfo, *p;
	int rv;

	// Own Code
	char *function = (char *)malloc(FUNCTION_NAME_SIZE);
	float input_num, result;

	if(argc < 3){
		perror(INPUT_ERROR_MSG);
		exit(INPUT_ERROR);
	}
	else{
		function = argv[1];
		input_num = strtof(argv[2], 0);

		if(((strcmp(function, DIV) != 0) && (strcmp(function, LOG) != 0)) || input_num >= 1.0 || input_num <= 0.0){
			perror(INPUT_ERROR_MSG);
			exit(INPUT_ERROR);
		}
	}

	// from Beej's Tutorial
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(LOCAL_HOST, SERVER_D_TCP_PORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("client: socket");
			continue;
		}

		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			perror("client: connect");
			continue;
		}
		break;
	}

	if(p == NULL){
		fprintf(stderr, "clinent: failed to connect.\n");
		exit(0);
		return 2;
	}
	freeaddrinfo(servinfo); // all done with this structure

	// Own Code
	printf("The %s is up and running.\n", CLIENT_NAME);

	send(sockfd, function, sizeof function, 0);
	send(sockfd, (char *)& input_num, sizeof input_num, 0);

	printf("The %s sent < %g > and %s to %s\n", CLIENT_NAME, input_num, function, AWS_NAME);

	recv(sockfd, (char *)& result, sizeof result, 0);
	printf("According to %s, %s on < %g >: < %g >\n", AWS_NAME, function, input_num, result);
}