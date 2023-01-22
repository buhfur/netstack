#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>


#define PORT 8080


/* what is a file descriptor ? 
	- integer that represents something about a file 

	socket_fd -> listening socket, listens for any incoming connections from a client 
	client_socket -> connection socket from the client , 
*/
int main( int argc, char const* argv[]){

	int server_fd, client_socket, read_data; 
	struct sockaddr_in address; // IPv4 Strcut 
	int opt = 1; // I have no idea honestly 
	int addrlen = sizeof(address); // return length of the address for the server 
	char buffer[1024] = { 0 }; // buffer to store info received from the client 
	char* message = "Hello from server."; // message sent to incoming clients 


	//initialize listening socket 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	//forces socket to bind to port 8080
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}


	//configure
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT); // converts the address to network byte order 

	//tries to bind an address to the listening socket 
	// TODO : find out casting the address to a sockaddr pointer , fills out the structs fields
	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
		perror("bind failed ");
		exit(EXIT_FAILURE);
	}

	//make sure the listening socket can listen for data being sent from the client 
	if( listen(server_fd, 3) < 0 ){
		perror("listen failed");
		exit(EXIT_FAILURE);
	}


	// accept incoming socket connection from client 
	if ((client_socket = accept(server_fd,  (struct sockaddr*) &address, (socklen_t*) &addrlen)) < 0){
		perror("accept failed");
		exit(EXIT_FAILURE);
	}

	//read data incoming from the client socket into the buffer
	read_data = read(client_socket, buffer, 1024);
	printf("%s\n" , buffer); // print the data sent from the client socket 
	//send data to the client socket connection 
	send(client_socket, message, strlen(message), 0);
	printf("message sent to client socket\n");

	//close the client_socket connection
	close(client_socket);
	//close the listening socket 
	shutdown(server_fd, SHUT_RDWR);

	

}
