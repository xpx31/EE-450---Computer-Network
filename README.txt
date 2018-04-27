a. Name: Pengxiang Xu

b. Student ID: 9730848105

c. What you have done in the assignment
	Programmed a mini client-server socket system that allows client to input a function (DIV or LOG) and a positive floating point number in range of (0,1) non-inclusive and returns the estimated output of the function through Taylor's expansion up to the 6th order. The system establishes a client-server through a TCP connection and server transmits and receivers among other servers through UDP connection. The backend servers performs the actual calculation for the key terms of Taylor's expansion, and the ASW server sums up the terms and returns the result to the client.

d. What your code files are and what each one of them does
	aws.c
		The aws server (serverD) is the server that takes inputs from client through TCP connection, ditributes tasks among all the other backend servers through UDP connection, sums up the calculated results from the backend servers, and return the final result to the client through TCP connection. 
	client.c
		The client takes in the command-line input from the user, validates the two inputs (a function name and a float point number between 0 and 1 non-inclusive), requests TCP connection to aws server, sends inputs to aws, and receives final calculated result from aws. 
	serverA.c
		Server A takes inputs from aws server, calculates and returns the square of the input number.
	serverB.c
		Server B takes inputs from aws server, calculates and returns the cube of the input number.
	serverC.c
		Server C takes inputs from aws server, calculates and returns the 5th power of the input number.
	socketconstants.h
		contains all constants for all execution files

e. The format of all the messages exchanged
	all DIV and LOG in the section below are referring to the constant that is included in socketconstants.h 
	file which are "DIV" and "LOG" respectively

	function: the function name (DIV or LOG), with a length of FUNCTION_NAME_SIZE which is 3 bytes long
	input_num: the float point input number, a float point number 

	command line input to client.c:
		through command line argument: argv[1]: function; argv[2]: input_num

	client to aws server:
		through TCP connection, funciton and input_num are passed through two connections

	aws server to server A, B, and C:
		through UDP connections, input_num is passed through a single connection to each server respectively

	server A, B, and C to aws server:
		through UDP connections, each exponential term result is passed through a single connection to aws respectively

	aws server to client:
		through TCP connection, the final combined result is passed through a TCP connection

	The message is formatted as the follows when entering ./client LOG 0.5 on the command line in the client terminal:

	aws server:
		The AWS is up and running.
		The AWS received input < 0.5 > and function=LOG from the client using TCP over port 2736
		The AWS sent < 0.5 > to Backend-Server A
		The AWS sent < 0.5 > to Backend-Server B
		The AWS sent < 0.5 > to Backend-Server C
		The AWS received < 0.25 > Backend-Server < A > using UDP over port < 21105 >
		The AWS received < 0.125 > Backend-Server < B > using UDP over port < 22105 >
		The AWS received < 0.03125 > Backend-Server < C > using UDP over port < 23105 >
		The AWS sent < 0.25 > to Backend-Server A
		The AWS sent < 0.25 > to Backend-Server B
		The AWS received < 0.0625 > Backend-Server < A > using UDP over port < 21105 >
		The AWS received < 0.015625 > Backend-Server < B > using UDP over port < 22105 >
		Values of powers received by AWS: < 0.5, 0.25, 0.125, 0.0625, 0.03125, 0.015625 >
		AWS calculated LOG on < 0.5 >: < -0.691146 >
		The AWS sent < -0.691146 > to client

	client:
		The client is up and running.
		The client sent < 0.5 > and LOG to AWS
		According to AWS, LOG on < 0.5 >: < -0.691146 >

	serverA:
		The Server A is up and running using UDP on port 21105.
		The Server A received input < 0.5 >
		The Server A calculated square: < 0.25 >
		The Server A finished sending the output to AWS
		The Server A received input < 0.25 >
		The Server A calculated square: < 0.0625 >
		The Server A finished sending the output to AWS

	serverB:
		The Server B is up and running using UDP on port 22105.
		The Server B received input < 0.5 >
		The Server B calculated cube: < 0.125 >
		The Server B finished sending the output to AWS
		The Server B received input < 0.25 >
		The Server B calculated cube: < 0.015625 >
		The Server B finished sending the output to AWS

	serverC:
		The Server C is up and running using UDP on port 23105.
		The Server C received input < 0.5 >
		The Server C calculated 5th power: < 0.03125 >
		The Server C finished sending the output to AWS

g. Any idiosyncrasy of your project. It should say under what conditions the project fails, if any. 
	The project does not contain idiosyncrasy. 
	However, due to the precision of a computer and mathematical terms defination of the number 1 and 0.99..., the client may not accept 0.99... term upto some extend, and treats the number as 1 essentially which is an invalid input for this project. In Ubuntu UNIX system, the largest float number that the computer treats smaller than 1 is around 0.9999999.

h. Resused Code: Did you use code from anywhere for you project? If not, say so. If so, say what functions and where they're from. (Also identify this with a comment in the source code.)
	Code from Beej's Guide to Network Programming was referred and commented throughout the project.
		Setting up TCP, UDP connections
		All socket operations (accept, bind, send, recv, sendto, recvfrom, etc.)