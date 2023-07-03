
/*****************************************
 * Name: <Timothy Lee>
 *
 * Description:
Simulate a Printer Spooler with POSIX pthreads using a producer-consumer model.  Get user input for number of printing threads.  Use mutexes to synchronize spooling and printing threads in order to siumulate a printing speed of 100KB/sec.  
 ***********************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define BUFFSIZE 2048
#define PRINTSPD 100
#define PRINTMAX 1000
#define PRINTMIN 50


pthread_mutex_t produce_mutex = PTHREAD_MUTEX_INITIALIZER; // Spooler
pthread_mutex_t consume_mutex = PTHREAD_MUTEX_INITIALIZER; // Printer

void *Simulate_Spooler ( void* );   
void *Simulate_Printer ( void* );   
char* get_timestamp ();

char* string;

int bufferUsage = 0;
int NumJobs = 0;

struct job {
	int jobNum;
	int jobSize;
};

pthread_t pro_tid;       //spoolers Thread ID
pthread_t con_tid;       //printers Thread ID

/**
 * function main
 * parameter: int argc, char *argv[]
 * Generates threads to simulate a printer spooler
 * param: command line arguments
 * Returns: 
 */
int main(int argc, char *argv[])
{    
	NumJobs = strtol(argv[1], NULL, 10);

	printf("%s", get_timestamp());

	printf("Simulation has started...\n");
	printf("Free buffer size %dKB\n", (BUFFSIZE - bufferUsage));

        pthread_attr_t pro_attr;  //thread object
        pthread_attr_init(&pro_attr); //initialize
	pthread_attr_t con_attr;  //thread object
        pthread_attr_init(&con_attr); //initialize

	struct job newJob[NumJobs];  //Struct array for holding jobs

	printf("-------------------------\n");
	printf("Printer has started...\n");

	int i = 0;
	for(i=0;i<NumJobs;i++)// for loop for spinning threads
	{
		newJob[i].jobNum = i;

		pthread_create(&con_tid, &con_attr, Simulate_Printer, &newJob[i]);
         	pthread_create(&pro_tid,&pro_attr,Simulate_Spooler, &newJob[i]); //spin n 											 //threads 
         	
		pthread_join(pro_tid, NULL);   // join spoolers
	}
	pthread_join(con_tid, NULL);   // join printers
	pthread_exit(0); //exit printers
}


/**
 * function Simulate_Spooler
 * parameter: void * param
 * Creates jobs and adds them to the buffer, waits while buffer is full, acts as producer
 * param: newjob[i]
 * Returns: none
 */
void *Simulate_Spooler ( void * param )   // Producer
{	
	struct job *newJob = (struct job *) param;
	newJob->jobSize = random_number(PRINTMIN,PRINTMAX,pro_tid);  // random job size

	string = get_timestamp();
	string[strlen(string)-1] = '\0';
	printf("%s - Job %d of size %d KB arrived.\n",string, newJob->jobNum, newJob->jobSize);

	while((bufferUsage + newJob->jobSize) > BUFFSIZE) // wait while buffer full
	{
		pthread_mutex_lock(&produce_mutex); // lock spoolers because full buffer
		string = get_timestamp();
		string[strlen(string)-1] = '\0';
		printf("%s - Insufficient buffer space for Job %d.\n", string,  newJob->jobNum);
		sleep(newJob->jobSize / PRINTSPD);

	}

	bufferUsage += newJob->jobSize; // add to buffer

	string = get_timestamp();
	string[strlen(string)-1] = '\0';
	printf("%s - Job %d stored in buffer.\n" , string, newJob->jobNum);

	pthread_exit(0); //exit thread 
}


/**
 * function Simulate_Printer
 * parameter: void * param
 * takes job from buffer and prints as a speed of PRINTSPD, 100KB/sec, acts as consumer
 * param: newjob[i]
 * Returns: none
 */
void *Simulate_Printer ( void * param )   // Consumer
{	
	struct job *newJob = (struct job *) param;
  	pthread_mutex_lock(&consume_mutex);    // lock consumers

	string = get_timestamp();
	string[strlen(string)-1] = '\0';
	printf("%s - Job %d starts printing.\n",string, newJob->jobNum);

	sleep(newJob->jobSize / PRINTSPD);
	bufferUsage = bufferUsage - newJob->jobSize; // remove item from buffer
	pthread_mutex_unlock(&produce_mutex);  // unlock producer now that buffer free

	string = get_timestamp();
	string[strlen(string)-1] = '\0';
	printf("%s - Job %d completed printing.\n",string, newJob->jobNum);

	pthread_mutex_unlock(&consume_mutex);  // unlock for other consumers
}

/**
 * function random_number
 * parameter: int min_num, int max_num, pthread_t tid
 * Takes a min and max, and generates a random number using time and thread ID as seed
 * param: PRINTMIN, PRINTMAX, pro_tid
 * Returns: result
 */
int random_number(int min_num, int max_num, pthread_t tid)
{
       int result=0,low_num=0,hi_num=0;
       if(min_num<max_num)
       {
           low_num=min_num;
           hi_num=max_num+1; // inclusive
       }else{
           low_num=max_num+1;// inclusive
           hi_num=min_num;
       }
       srand((time(NULL) & 0xFFFF) | (getpid() << 16)*tid); // hopefully more random seed
       result = (rand()%(hi_num-low_num))+low_num;
       return result;
}

/**
 * function get_timestamp
 * parameter: none
 * Generates a time object and makes it a null terminated string
 * param: none
 * Returns: timestamp
 */
char* get_timestamp ()
{
  time_t now = time (NULL);
  return asctime (localtime (&now));
}
