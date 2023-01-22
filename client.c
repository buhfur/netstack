#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
//client socket for the server
int main(int argc , const char* argv[]){
	int sock = 0 , read_data , client_fd;

	struct sockaddr_in serv_addr;
	char* data = "hello from client ";
	char buffer[1024]  = { 0 }; 

	// initialize socket file descriptor 
	if (( sock = socket(AF_INET, SOCK_STREAM, 0 )) < 0 ){
		perror("socket failed\n");
		exit(EXIT_FAILURE);
	}

	
	//configure server socket 
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	//serialize IpV4 and IpV6 addresses  , AF_INET => 32 bits long 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0 ){
		perror("Address not supported\n");
		exit(EXIT_FAILURE);
	}

	//client connection to server 
	if((client_fd = connect(sock , (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0 ){
		perror("connection failed\n");
	}


	//send data to the server through the sock socket 
	send(sock, data, strlen(data), 0);

	//receive data from the server 
	read_data = read(sock, buffer, 1024);

	printf("%s\n", buffer); // print out the message sent from the server

	//close the client connection 
	close(client_fd);


	
}
