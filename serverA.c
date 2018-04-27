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

// Referred Code Begin: from Beej's Tutotial
int main(void){
	int sockfd;
	int rv;
	struct addrinfo hints; // the struct addrinfo have already been filled with relevant info
	struct addrinfo *servinfo;
	struct sockaddr_storage aws_addr;
	struct addrinfo *p; // tempoary point

	memset(&hints, 0, sizeof hints); // make sure the struct is empty
	hints.ai_family = AF_UNSPEC; // don't care IPv4 or IPv6
	hints.ai_socktype = SOCK_DGRAM; // UDP dgram sockets
	hints.ai_flags = AI_PASSIVE; // use my IP

	if((rv = getaddrinfo(LOCAL_HOST, SERVER_A_UDP_PORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 0;
	}

	// loop through all the results and bind to the first we can
	for (p = servinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror(SERVER_A_SOCKET_ERROR_MSG);
			continue;
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			perror(SERVER_A_BIND_ERROR_MSG);
			continue;
		}
		break;
	}
	if(p == NULL){
		fprintf(stderr, "%s\n", SERVER_A_BIND_ERROR_MSG);
		return 0;
	}
	freeaddrinfo(servinfo);
	printf("The Server %s is up and running using UDP on port %s.\n", SERVER_A_NAME, SERVER_A_UDP_PORT);
	// Referred Code End: from Beej's Tutorial

	while(1){
		socklen_t aws_addr_len = sizeof aws_addr;
		float input_num, result;

		recvfrom(sockfd, (char *)&input_num, sizeof input_num, 0, (struct sockaddr *)&aws_addr, &aws_addr_len);
		printf("The Server %s received input < %g >\n", SERVER_A_NAME, input_num);

		result = input_num * input_num;
		printf("The Server %s calculated square: < %g >\n", SERVER_A_NAME, result);

		sendto(sockfd, (char *)&result, sizeof result, 0, (struct sockaddr *)&aws_addr, aws_addr_len);
		printf("The Server %s finished sending the output to %s\n", SERVER_A_NAME, AWS_NAME);
	}
}