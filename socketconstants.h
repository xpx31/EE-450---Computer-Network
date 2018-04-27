

#ifndef SOCKET_PROGRAMMING_SOCKETCONSTANTS_H
#define SOCKET_PROGRAMMING_SOCKETCONSTANTS_H

// Names
#define SERVER_A_NAME "A"
#define SERVER_B_NAME "B"
#define SERVER_C_NAME "C"
#define AWS_NAME 	  "AWS"
#define CLIENT_NAME   "client"
#define LOCAL_HOST	  "localhost"
#define DIV "DIV"
#define LOG "LOG"

// TCP/UDP Port Numbers
#define SERVER_A_UDP_PORT "21105"
#define SERVER_B_UDP_PORT "22105"
#define SERVER_C_UDP_PORT "23105"
#define SERVER_D_UDP_PORT "24105"
#define SERVER_D_TCP_PORT "25105"

// Limitations
#define FUNCTION_NAME_SIZE 3
#define NUMS_OF_BACK_SERVERS 3
#define BACKLOG 10
#define MAXDATASIZE 100 // in bytes

// Error Message Codes
#define SOCKET_ACCEPT_ERROR 701
#define SERVER_A_SOCKET_ERROR 702
#define SERVER_A_BIND_ERROR 703
#define SERVER_B_SOCKET_ERROR 702
#define SERVER_B_BIND_ERROR 703
#define SERVER_C_SOCKET_ERROR 702
#define SERVER_C_BIND_ERROR 703
#define INPUT_ERROR 801
#define FUNCTION_INPUT_ERROR 802
#define UDP_RECEIVE_ERROR 803


// Error Messages
#define SOCKET_ACCEPT_ERROR_MSG "accept error"
#define SERVER_A_SOCKET_ERROR_MSG "server A error: socket"
#define SERVER_A_BIND_ERROR_MSG "server A error: failed to bind"
#define SERVER_B_SOCKET_ERROR_MSG "server B error: socket"
#define SERVER_B_BIND_ERROR_MSG "server B error: failed to bind"
#define SERVER_C_SOCKET_ERROR_MSG "server C error: socket"
#define SERVER_C_BIND_ERROR_MSG "server C error: failed to bind"
#define INPUT_ERROR_MSG "input error: function can either be DIV or LOG, and number must be in range of (0, 1) non-inclusive."
#define FUNCTION_INPUT_ERROR_MSG "function error: please enter ""DIV"" for 1/(1 - x) and ""LOG"" for log(1 - x)."
#define UDP_RECEIVE_ERROR_MSG "UDP receive error."

#endif //SOCKET_PROGRAMMING_SOCKETCONSTANTS_H