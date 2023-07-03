
/*
 * TimothyLee     
 * SHELL:  Recreate system calls cd, echo, exit, and implement execvp in a custom, BASH 
 * 				like shell
 * SOURCES:    -All of Google
 *               -Countless searching of many different implementations helped me with the assignment
 *             -Text for the Class
 *             -Student help
 */
 


#include "apue.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "shell.h"

#define READ_FD 0
#define WRITE_FD 1


int makeargv(char *, char *, char ***);
void cd(char *);
void echo(char **, int);
void execPiped(char*, int, int);

//char buf[MAXLINE]; /* from apue.h */	
char * prompt = ("TimsCustomShell:");
	
int status;	
int numArg;	
char* input = NULL;
size_t input_size = 0;
		
char ** ExecVectors;  
pid_t shell_pid;
pid_t pid;

int pipefd[2];
int i, line;
char s[1000];

int in, out, inputIO, outputIO, appendIO; // I/O redirection parameters
char *inputFile, *outputFile; // I/O input and output files
FILE *fp; //pointer to file for ouput file



int main(int argc, char *argv[]) 
{
	
		
	// Retrieve PID of the parent process. 
	shell_pid = getpid();
		
        // Print shell prompt. 
	printf("{%i}%s$ ", shell_pid, prompt);	
	while (getline(&input, &input_size, stdin) != -1 || input != NULL)
	{
		numArg = makeargv(input, "\t \n", &ExecVectors);
		ExecVectors[numArg] = NULL;
		
	if (pipe(pipefd) < 0) 
	{
          perror("pipe");
          exit(1);
        }

	if ((pid = fork()) != 0)  // Create a child process to handle user input. 
	{
		if (pid < 0) { perror("fork error"); exit(1); }
		
		if ((pid = waitpid(pid, &status, 0)) < 0)
		perror("waitpid error"); exit(1);
	} 
	else // Child process
	{
		// Handle user input here!
		if(!strcmp(ExecVectors[0], "cd"))
		{
			printf("cd %s\n", ExecVectors[1]);
   			cd(ExecVectors[1]);	
			
			 // Print shell prompt. 
	        	printf("{%i}%s$ ", shell_pid, prompt);	
			continue;
		}
		else if(!strcmp(ExecVectors[0], "echo"))
		{
			printf("echo\n");
				echo(ExecVectors, numArg);				
				
		        // Print shell prompt. 
		        printf("{%i}%s$ ", shell_pid, prompt);	
				continue;
		}
		else if(!strcmp(ExecVectors[0], "exit"))
		{
			printf("exit\n");
			break;
		}
		else
		{
			execvp(ExecVectors[numArg+1],&ExecVectors[numArg+1]);
			perror("exec failed\n");
		        exit(0);
		}	       
	}
  }
}


int makeargv(char *s, char *delimiters, char ***argvp)
{
   char *t;
   char *snew;
   int numtokens;
   int i;
    /* snew is real start of string after skipping leading delimiters */
   snew = s + strspn(s, delimiters);
                              /* create space for a copy of snew in t */
   if ((t = calloc(strlen(snew) + 1, sizeof(char))) == NULL) {
      *argvp = NULL;
      numtokens = -1;
   } else {                     /* count the number of tokens in snew */
      strcpy(t, snew);
      if (strtok(t, delimiters) == NULL)
         numtokens = 0;
      else
      for (numtokens = 1; strtok(NULL, delimiters) != NULL;
              numtokens++)
              ;  // THE work is in the For Construct
                /* create an argument array to contain ptrs to tokens */
      if ((*argvp = calloc(numtokens + 1, sizeof(char *))) == NULL) {
         free(t);
         numtokens = -1;
      } else {            /* insert pointers to tokens into the array */
         if (numtokens > 0) {
            strcpy(t, snew);
            **argvp = strtok(t, delimiters);
            for (i = 1; i < numtokens + 1; i++)
               *((*argvp) + i) = strtok(NULL, delimiters);
         } else {
           **argvp = NULL;
           free(t);
         }
      }
   }   
   return numtokens;
}


void cd(char *dir)
{	
  char * name = "PWD"; //string PWD
  char cwd[256]; // holder for current directory
  char * newCurrent = getcwd(cwd, sizeof(cwd)); //get the current dir and put it in cwd
  
	if(chdir(dir) == 0)// change the directory
	{
		printf("Directory Change Successful\n");
		printf("%s\n", newCurrent);
	}
	else 
	{
		printf("Directory Change Failure\n");
	}
  
	setenv(name, newCurrent,1); //set new pwd
}

void echo(char **c, int argNum)
{
	int i;
	for (i=1;i<argNum;i++) { fprintf(stderr,"%s ",c[i]); }
	printf(" \n");
}

void execPiped(char *path, int read_fd, int write_fd)
{
        int pid;

        if ( (pid = fork()) < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
        }

        if (pid) {
                /* parent (the shell) */
                if (read_fd) {
                        close(read_fd);
                }
                if (write_fd) {
                        close(write_fd);
                }
                return;
        }
        if (read_fd) {
                dup2(read_fd, STDIN_FILENO);
        }
        if (write_fd) {
                dup2(write_fd, STDOUT_FILENO);
        }

        execl(path, path, NULL);
}



// check the command for any I/O redirection
void redirectionHandle(char **userInputs){
  // reset input and output and append
  inputIO = 0;
  outputIO = 0;
  appendIO = 0;

  int i = 0;

  // loop through the command line input
  while(userInputs[i] != NULL)
  {
    if (!strcmp(userInputs[i], "<"))
    {
      strcpy(userInputs[i], "\0");
      inputFile = userInputs[i+1];
      inputIO = 1;
    }
    else if (!strcmp(userInputs[i], ">"))
    {
      outputFile = userInputs[i+1];
      userInputs[i] = NULL;
      outputIO = 1;
      break;
    }
    else if (!strcmp(userInputs[i], ">>"))
    { 
      outputFile = userInputs[i+1];
      userInputs[i] = NULL;
      appendIO = 1;
      break;
    }
    i++;
  }
}

