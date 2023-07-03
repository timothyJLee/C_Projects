

/*
 * TimothyLee     
 * 
 * A1_MAP:  Create a linked list data structure in C
 * SOURCES:    -All of Google
 *               -Countless searching of many different implementations helped me with the assignment
 *           
 */



#include <assert.h>
#include "map.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int size = 0;

void map_init(map_t* self)
{
  self = (map_t*)malloc(sizeof(map_t));
  self->entry = NULL;
  self->size = 0;
}

int map_put(map_t* self, const char* key, const char* val)
{
  assert(self != NULL);
 
   struct _map_entry *newNode;
   newNode =  (struct _map_entry*)malloc(sizeof(struct _map_entry));
   newNode->key = (char *)key;
   newNode->value = (char *)val;
   newNode->next = NULL;
   printf("(map_put) newNode->key:value %s:%s\n",newNode->key, newNode->value);
   
     struct _map_entry *currentNode = self->entry;
     bool KeyExist = false;
     
     
     if(self->entry == NULL)
     {
            currentNode = newNode;
            self->entry = newNode;
            size++;
            printf("(map_put) newNode successfully assigned to self->entry...\n");
	  }
	  else
	  {
		  if(currentNode->next == NULL )
		  {
			currentNode->next = newNode;
            self->entry->next = newNode;
            size++;
            printf("(map_put) newNode successfully assigned to currentNode->next...\n");
		  }
		  else
		  {
			   while(currentNode->next != NULL)
            {
				printf("(map_put) currentNode->key:value %s:%s\n",currentNode->key, currentNode->value);
                if(strcmp(currentNode->key,newNode->key)==0)
                {
                      KeyExist = true;
                      break;
                }
                currentNode = currentNode->next;
            }
            if(KeyExist == true)
            {
				printf("(map_put) key already exists...\n");
                return 1;
            }
            else
            {
				currentNode->next = newNode;
				printf("(map_put) Node successfully assigned to end of list...\n");
		        size++;
				return 0;
			}  
		  }
	  }          	 
     return 0;
}


const char* map_get(map_t* self, const char* key)
{
  assert(self != NULL);

  struct _map_entry *currentNode = self->entry;
   while(currentNode != NULL)
  {
	  if(currentNode->key == NULL)
	  {
		  printf("deleted node...\n");
	  }
	  else
	  {
		  printf("currentNode->key:key %s:%s\n",currentNode->key,key);
	  printf("currentNode->value %s\n",currentNode->value);
     if(strcmp(currentNode->key,key)==0)
       return currentNode->value;
	  }
	  
    currentNode = currentNode->next;     
  } 
    return "Key Not Found";
}

int map_size(map_t* self)
{
  assert(self != NULL);
  if(self==NULL)
    return 0;
  else
      self->size = size;
      return size; 
}

int map_remove(map_t* self, const char* key)
{
     assert(self != NULL);

      struct _map_entry *tempNode = self->entry;
      struct _map_entry *currentNode = tempNode->next;

      while(tempNode != NULL)
      {
         if(strcmp(tempNode->key,key)==0)
         {
     currentNode = tempNode->next;
     tempNode->key = NULL;
     tempNode->value = NULL;
     free(tempNode);
     tempNode = currentNode;
    
           size = size - 1;
           printf("Node removed from map...\n");
         }
         else
         {
			  currentNode = tempNode->next;
			  tempNode = currentNode;
			 printf("Node remove failed...\n");
		 }
      }
      return 0;
}


void map_destroy(map_t* self)
{
    assert(self != NULL);
 
  struct _map_entry *tempNode =   self->entry;
  struct _map_entry *currentNode =   NULL;
  while(tempNode != NULL)
  {
     currentNode = tempNode->next;
     free(tempNode);
     tempNode = currentNode;    
  }
  self->size = 0;
  size = 0;
  self->entry = NULL;
  free(self); 
   printf("Map destroyed...\n");
}


int map_deserialize(map_t* self, FILE* stream)
{
    assert(self != NULL);
    assert(stream != NULL); 

        char *line = malloc(1024);        
        while(fgets(line,1024,stream))
        {
            char *value = strstr(line,":");            
            int keylength = value - line;            
            char *key = malloc(sizeof(keylength));            
            int i;
            for(i = 0; i < keylength; i++)
            {
                key[i] = line[i];
            }
            key[keylength] = '\0';
            value++;
            map_put(self,key,value);
        }       
    fclose(stream);
    return 0;
}


int map_serialize(map_t* self, FILE* stream)
{
  assert(self != NULL);
  
  struct _map_entry *it = self->entry;

    while (it != NULL)
    {
        fwrite (it->key, sizeof (it->key), 1, stream);
        fwrite (":", 1, 1, stream);
        fwrite (it->value, sizeof (it->value), 1, stream);
        fwrite ("\r\n", 2, 1, stream);
        it = it->next;
    }
    return 0; 
}
