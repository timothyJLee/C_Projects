///Timothy Lee
///WebServer
///
///http://www.ibm.com/developerworks/systems/library/es-nweb/index.html?ca=dat  Was given this as a reference from a classmember, received a
///bit of help as I was having a few problems with certain parts
///
///Was also shown Anjuta which helps my programming world immensely

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFERSIZE 8196

int main(int argc, char *argv[])
{	
	assert (argc == 4);
	
	char *host = argv[1];
	int port = atoi (argv[2]);
	int sockfd;	
	char get[BUFFERSIZE];
	char recieved[BUFFERSIZE];
	

	static struct sockaddr_in serv_addr;
	int i;
	
	//Turn Command Line Argument into a Get Request("GET /filename HTTP/1.0 ")
	strcpy(get, "GET /");
	strcat(get,argv[3]);
	strcat(get, " HTTP/1.0 \r\n\r\n");

	printf("client trying to connect to %s and port %d\n",host,port);
	 
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0)
	{
		perror("socket() failed");
        exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(host);
	serv_addr.sin_port = htons(port); // Setup connections and ports to get ready

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) //  Connect to the socket
	{ 
		perror("connect() failed");
        exit(1);
	}
	else
	{
	printf("Send bytes=%d %s\n",strlen(get), get);	 
	write(sockfd, get, strlen(get));

	while((i=read(sockfd,recieved,BUFFERSIZE)) > 0)
	{		
	write(1,recieved,i);
        printf("\n");	 
	}
  }	
}
