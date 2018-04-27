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

// Referred Code Begin: from Beej's Tutorial
void sigchld_handler(int s){
	//waitpid() might overwrite errno, so we save and restore it:
	int saved_errno = errno;
	while(waitpid(-1, NULL, WNOHANG) > 0);
	errno = saved_errno;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
	if(sa->sa_family == AF_INET){
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
// Referred Code End: from Beej's Tutorial

// Get Indiviual Results from each server
float getPartialTaylor(float input_num, char *server){
	int server_socket;
	float term_result;
	int rv;
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr from_addr;
	socklen_t from_addr_len;
	char *server_port;

	// Associate server names with server ports 
	if(strcmp(server, SERVER_A_NAME) == 0){
		server_port = SERVER_A_UDP_PORT;
	}
	else if(strcmp(server, SERVER_B_NAME) == 0){
		server_port = SERVER_B_UDP_PORT;
	}
	else if(strcmp(server, SERVER_C_NAME) == 0){
		server_port = SERVER_C_UDP_PORT;
	}

	// Set up UDP connection
	// Referred Code Begin: from Beej's Tutorial
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if((rv = getaddrinfo(LOCAL_HOST, server_port, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: on server %s: %s\n", server, gai_strerror(rv));
		return 1;
	}

	for(p = servinfo; p != NULL; p = p->ai_next){
		if((server_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("talker: socket");
			continue;
		}
		break;
	}
	if(p == NULL){
		fprintf(stderr, "talker: failed to bind socket\n");
		return 2;
	}
	// Referred Code End: from Beej's Tutorial

	sendto(server_socket, (char *)&input_num, sizeof input_num, 0, p->ai_addr, p->ai_addrlen);
	printf("The %s sent < %g > to Backend-Server %s\n", AWS_NAME, input_num, server);

	from_addr_len = sizeof hints;
	recvfrom(server_socket, (char *)&term_result, sizeof term_result, 0, &from_addr, &from_addr_len);

	return term_result;
}

// Prints Server Receive Message
void printServerReceiveMsg(float pt, char * server, char * serverPort){
	printf("The %s received < %g > Backend-Server < %s > using UDP over port < %s >\n", 
			AWS_NAME, pt, server, serverPort);
}

// Combine Results
float combineResult(char * function, float input_num, float p2, float p3, float p4, float p5, float p6){
	float result;

	if(strcmp(function, DIV) == 0){
		result = 1.0 + input_num + p2 + p3 + p4 + p5 + p6;
	}
	else if(strcmp(function, LOG) == 0){
		result = -1.0 * (input_num + p2 / 2.0 + p3 / 3.0 + 
					p4 / 4.0 + p5 / 5.0 + p6 / 6.0);
	}
	else{
		perror(FUNCTION_INPUT_ERROR_MSG);
		exit(FUNCTION_INPUT_ERROR);
	}

	return result;
}

// Get Result
float getResult(char * function, float input_num){
	float power_2_term, power_3_term, power_4_term, power_5_term, power_6_term;
	
	// Send to Backended Servers for calculation
	power_2_term = getPartialTaylor(input_num, SERVER_A_NAME);
	power_3_term = getPartialTaylor(input_num, SERVER_B_NAME);
	power_5_term = getPartialTaylor(input_num, SERVER_C_NAME);

	printServerReceiveMsg(power_2_term, SERVER_A_NAME, SERVER_A_UDP_PORT);
	printServerReceiveMsg(power_3_term, SERVER_B_NAME, SERVER_B_UDP_PORT);
	printServerReceiveMsg(power_5_term, SERVER_C_NAME, SERVER_C_UDP_PORT);

	power_4_term = getPartialTaylor(power_2_term, SERVER_A_NAME);
	power_6_term = getPartialTaylor(power_2_term, SERVER_B_NAME);

	printServerReceiveMsg(power_4_term, SERVER_A_NAME, SERVER_A_UDP_PORT);
	printServerReceiveMsg(power_6_term, SERVER_B_NAME, SERVER_B_UDP_PORT);

	printf("Values of powers received by %s: < %g, %g, %g, %g, %g, %g >\n", 
		AWS_NAME, input_num, power_2_term, power_3_term, power_4_term, power_5_term, power_6_term);

	// Combine Results
	return combineResult(function, input_num, power_2_term, power_3_term, power_4_term, power_5_term, power_6_term);
}

// Referred Code Begin: from Beej's Tutorial
int main(){
	int sockfd, client_fd; // listen on sock_fd, new connection on client_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	int yes = 1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if((rv = getaddrinfo(LOCAL_HOST, SERVER_D_TCP_PORT, &hints, &servinfo)) != 0){
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next){
		if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1){
			perror("server: socket");
			continue;
		}

		if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){
			perror("setsockopt");
			exit(1);
		}

		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1){
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if(p == NULL){
		fprintf(stderr, "server: failed to bind\n");
		exit(1);
	}

	if(listen(sockfd, BACKLOG) == -1){
		perror("listen");
		exit(1);
	}

	// Referred Code End: from Beej's Tutorial

	printf("The %s is up and running.\n", AWS_NAME);

	// Patially referred from Beej's Tutorial
	// While loop
	while(1){
		struct sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		int client_port;
		float result;
		char *function = (char *)malloc(FUNCTION_NAME_SIZE);
		float input_num; // a float between (0, 1)

		// Referred from Beej's Tutorial
		sin_size = sizeof their_addr;
		client_fd = accept(sockfd, (struct sockaddr *) &their_addr, &sin_size);

		if(client_fd == -1){
			perror(SOCKET_ACCEPT_ERROR_MSG);
			exit(SOCKET_ACCEPT_ERROR);
		}

		inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *) &their_addr), s, sizeof s);

		// Get Client Port
		memset(&client_addr, 0, sizeof(client_addr));
		getpeername(client_fd, (struct sockaddr *) &client_addr, (socklen_t *) &client_addr_len);
		client_port = client_addr.sin_port;
		// Referred from Beej's Tutorial End

		// Get Inputs
		recv(client_fd, function, sizeof function, 0);
		recv(client_fd, (char *)&input_num, sizeof input_num, 0);

		printf("The %s received input < %g > and function=%s from the client using TCP over port %d\n",
			AWS_NAME, input_num, function, client_port);

		// Get Result
		result = getResult(function, input_num);

		printf("%s calculated %s on < %g >: < %g >\n", AWS_NAME, function, input_num, result);

		send(client_fd, (char *)&result, sizeof(result), 0);
		printf("The %s sent < %g > to client\n", AWS_NAME, result);

		close(client_fd);
	}
}

