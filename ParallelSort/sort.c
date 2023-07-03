
/*Timothy Lee
 * Parallel Sort (pthread)
 *
 * */

#include <dirent.h> 
#include <string.h>
#include <unistd.h>
#include <pthread.h> 
#include <stdio.h> 
#include <stdlib.h>
#include <syscall.h>

void WriteFile(int);
void *DealWithFile(void*);
int FindingFile(char*);
int ReadFile(char*);
void CreateThread(int);
int Compare (const void *, const void *);
void JoinThread(int);
int MergeSort(int);

typedef struct Record // struct for holding individual fields
{
	char* userName;
	char* password;
	char* bloodType;
	char* domain;
	int index;
}Record;

char fileName[10][100];  // holding names of files
char *directory;          
pthread_t thread[256];   
pthread_mutex_t block;   // synchro
int totalN;
	
struct Record records[100][1000];
struct Record sorted[10000];


int main(int argc, char *argv[])
{	
	char f[] = "./data"; // path for finding files
	pthread_mutex_init(&block, NULL);	// synch
	int numFiles;	 // for holding the number of files
	int N = 0;
	
	numFiles = FindingFile(f);  // Finding file returns the number of files

	if(numFiles == -1)  //  if not valid
	{		
		 printf("No files found in  %s.\n", f);		
		return 1;		
	}
	else // else create a thread to properly handle
	{
		printf("Path(%s)\n    Unsorted(%d)\n", f,numFiles);
		
		CreateThread(numFiles);		
		JoinThread(numFiles);		
	    printf("Sorted: (%d)\n" ,totalN );
		
		WriteFile(N);	
  	}
	printf("-----------------------------------------\n" );	
return 0;	
}


int Compare (const void * a, const void * b)
{
	Record *a1 = (Record *)a;
	Record *a2 = (Record *)b;
	return a1->index-a2->index;
}


int ReadFile(char* FilePath) // Reading records from file so that they may be sorted
{
	pthread_mutex_lock(&block);
	int i = 0;	
	int fileNumber = FilePath[0] - '0';	
    char line[256];
	char buf[256];	
	sprintf(buf,"%s%d",directory,fileNumber);
	FILE *fp ;	
    fp = fopen(buf, "r");

		if( fp== NULL ) // if file does not exist
		{		
			return	-1;
		}	
	
	while(fgets(line,256,fp)) 
	{
		   struct Record *record = malloc(sizeof(struct Record));		
		    records[fileNumber][i] = *record;				  
            char *pch = strtok (line,",");		   
		    records[fileNumber][i].userName =  malloc(sizeof(Record));
		    strcpy(records[fileNumber][i].userName, pch);		   
		   	pch = strtok (NULL, ",");
 			records[fileNumber][i].password = malloc(sizeof(Record));
		    strcpy(records[fileNumber][i].password, pch);	
		    pch = strtok (NULL, ",");
	 		records[fileNumber][i].bloodType = malloc(sizeof(Record));
		  	strcpy(records[fileNumber][i].bloodType, pch);		   
			pch = strtok (NULL, ",");
		    records[fileNumber][i].domain = malloc(sizeof(Record));
		    strcpy(records[fileNumber][i].domain, pch);
		    pch = strtok (NULL, ",");
		    records[fileNumber][i].index = malloc(sizeof(Record));
			records[fileNumber][i].index = atoi(pch);
		
		   totalN++;
		   i++;
 	}	
    pthread_mutex_unlock(&block);
	printf("Reading File: %d\n->Records: %d\n", fileNumber, i);	
 	fclose(fp);   
    return i; // return recnum
}


void WriteFile(int recordsLength) // Write to sorted.yay
{
    FILE *f;	
    f = fopen("sorted.yay", "w");	
	if(f==NULL)
	{	
		printf("ERROR OPENING FILE");		
	}
	
	char buf[10];
	int i = 0;	
	for(i = 0; i < recordsLength; i++)
	{
		fputs(sorted[i].userName,f);
		fputs(",",f);

		fputs(sorted[i].password,f);
		fputs(",",f);

		fputs(sorted[i].bloodType,f);
		fputs(",",f);

		fputs(sorted[i].domain,f);
		fputs(",",f);
		
		snprintf(buf, 10, "%d", sorted[i].index);
	    fputs(buf,f);

		fputs(",",f);
		fputs("\r\n",f);		
	}	
	fclose(f);	
	 	printf("Write was Successful\n");	
}


int FindingFile(char* filePath)  // Find the File and increment totalFiles
{
 DIR *d;	
 struct dirent *dir;	
 directory = filePath;	
 strcat(directory, "/");	
 d = opendir(directory);	
 int totalFiles = 0;	
  if (d)
  {	  
    while ((dir = readdir(d)) != NULL)
    {		
		if (dir->d_type == DT_REG)
		{		
    	  strcpy(fileName[totalFiles],dir->d_name);
            totalFiles++;     	 
		}  
    }
    closedir(d);   
  }
  else
  {	  
	  return -1;
  }
	return totalFiles;	
}


void *DealWithFile(void *ID)// ReadFile and MergeSort
{
	int *fileNum = ID;
	int totalRecords = ReadFile(ID);	
	totalRecords = MergeSort(*fileNum);
}


void CreateThread(int numberOfThreads) // Create a thread for dealing with a file
{
	int i=0;
	for(i = 0; i < numberOfThreads; i++)
	{
		 pthread_create(&thread[i], NULL, DealWithFile,  (void *) &fileName[i]);
	}	
	 printf("Number of Threads: (%d)\n" ,i );
}

void JoinThread(numFiles) // Join a thread
{	
	int i = 0;	
	for(i = 0; i < numFiles; i++)
	{
		  pthread_join(thread[i], NULL);
	}		
	  	printf("Successfully Joined [%d] Threads \n", i);
}

int MergeSort(numFiles) // Merge sorce the records with qsort
{
	int success = 0;	
	int i = 0;	
	int j = 0;	
	for(i = 0; i<10; i++)
	{		
      for(j = 0; j<1000; j++)
      {
	      if(records[i][j].userName == NULL)
	      {
			   break;
		  }			
            sorted[success++] = records[i][j];
	  }			
	}	
	qsort(sorted, success, sizeof(Record), Compare);	
	return success;
}
	
		

