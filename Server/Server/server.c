///Timothy Lee
///WebServer
///
///http://www.ibm.com/developerworks/systems/library/es-nweb/index.html?ca=dat  Was given this as a reference from a classmember, received a
///bit of help as I was having a few problems with certain parts
///
///Been using Anjuta which helps my programming life...

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFLEN 10000

struct 
{	
	char *extention;
	char *fileDescription;	
}extensions [] = 
{
	{"gif", "image/gif" },  
	{"jpg", "image/jpg" }, 
	{"jpeg","image/jpeg"},
	{"png", "image/png" },  
	{"ico", "image/ico" },  
	{"zip", "image/zip" },  
	{"gz",  "image/gz"  },  
	{"tar", "image/tar" },  
	{"htm", "text/html" },  
	{"html","text/html" },  
	{0,0}
};


void ForkServer(int, int);
void Writer(int, char *, char *, int);

int main(int argc, char **argv)
{
	int i;
	int port = atoi(argv[1]);; 
	int pid;
	int listenfd;
	int socketfd;
	int target;
	
	socklen_t length;
	
	static struct sockaddr_in cli_addr; //made all zeros
	static struct sockaddr_in serv_addr; 
	
	
	if(fork() != 0)
	{		
		return 0; //return zero to show everything is fine
	}
		
	(void)signal(SIGCLD, SIG_IGN); 
	(void)signal(SIGHUP, SIG_IGN); 	
	for(i=0;i<32;i++)
	{		
		(void)close(i); //for loop to go over all open files and close them
	}	
	(void)setpgrp();		//set the pgrp to break from current group
	
	
	if((listenfd = socket(AF_INET, SOCK_STREAM,0)) <0) // listening fd socket, if less than zero than 500 error
	{		
		Writer(500,"SOCKET ERROR","bind",0);	
	}	
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port); // connecting ports and sockets
	
	if(bind(listenfd, (struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0) // Binding sockets and returning error if wrong
	{
		Writer(500,"BIND ERROR","bind",0);		
	}
	
	if(listen(listenfd,64) <0)  // Label socket as a passive socket that will accept incoming connection requests
	{		
		Writer(500,"LISTEN ERROR","listen",0);		
	}
	
	for(target = 1;;) 
	{		
		length = sizeof(cli_addr);		
		socketfd = accept(listenfd, (struct sockaddr *)&cli_addr, &length); //Accepting incoming connection after listening
		if((pid = fork()) < 0) //Forking processes
		{			
			Writer(500,"FORK ERROR","fork",0);			
		} 
		else 
		{			
			if(pid == 0) 
			{
				(void)close(listenfd);	//child
				ForkServer(socketfd,target); 			
			} 
			else 
			{ 			
				(void)close(socketfd); //parent				
			}
		}
		target++;
	}
}


void Writer(int error, char *s1, char *s2, int socket_fd)
{	
	char log[BUFFLEN*2];
	switch (error) 
	{		
	case 500:			
		(void)sprintf(log,"HTTP/1.1 500 ERROR"); 		//Write 500 ERROR
		break;			
	case 400:			//Write 400 Error
		(void)write(socket_fd, "HTTP/1.1 404 Not Found\nContent-Length: 136\nConnection: close\n\n<html><head>\n<title>404 Not Found</title>\n</head><body>\n<h1>Not Found</h1>\nThe requested URL was not found on this server.\n</body></html>\n",224); 
		break;			
	}		
	exit(3);	
}

void ForkServer(int fd, int target)
{		
	static char buffer[BUFFLEN+1]; 
	int file_fd;
	int bufferLength;
	long i;
	long returnVal;
	long length;
	char * string;

	returnVal = read(fd,buffer,BUFFLEN); 	//reading request
	
	for(i=4;i<BUFFLEN;i++) 
	{	
		if(buffer[i] == ' ') 
		{ 		
			buffer[i] = 0;			
		break;			
		}
	}

	bufferLength = strlen(buffer);
	string = (char *)0;
	
	for(i=0;extensions[i].extention != 0;i++) //Setting extentions for returning files
	{		
		length = strlen(extensions[i].extention);		
		if( !strncmp(&buffer[bufferLength-length], extensions[i].extention, length)) 
		{			
			string = extensions[i].fileDescription;			
			break;			
		}		
	}

	if(( file_fd = open(&buffer[5],O_RDONLY)) == -1) //Reading the file
	{		
		Writer(400, "FILE ERROR \n",&buffer[5],fd);		
	}
	else
	{
		  length = (long)lseek(file_fd, (off_t)0, SEEK_END); //Set file length
	      (void)lseek(file_fd, (off_t)0, SEEK_SET); //Seek to read File
	      (void)sprintf(buffer,"HTTP/1.1 200 OK\nServer: WEB***/\nContent-Length: %ld\nConnection: close\nContent-Type: %s\n\n", length, string); 
		  (void)write(fd,buffer,strlen(buffer));
		  
	while ((returnVal = read(file_fd, buffer, BUFFLEN)) > 0 )
	{		
		(void)write(fd,buffer,returnVal);		
	}
  }
  sleep(1);	
  close(fd);
  exit(1);	
}

