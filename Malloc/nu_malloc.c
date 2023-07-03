
/*
 * TimothyLee     
 * 02/04/14
 * A2_Malloc:  Recreate system calls malloc, calloc, realloc, and free without using
 *             original system calls.
 * 
 * SOURCES:    -All of Google
 *               -Countless searching of many different implementations helped me with the assignment
 *             -Text for the Class
 *               -Helped with the typedef and header file as well as malloc.  The first assignment
 *                also helped
 *             -http://stackoverflow.com/questions/5422061/malloc-implementation
 *               -Website that provided the most guidance and help on the subject. Parts
 *                of program are modeled off of the examples here as well as examples from
 *                other google pages.
 */
 

#include "nu_malloc.h"
#include <unistd.h> // included for sbrk()


void* nu_malloc(size_t size)
{
	size = (size + sizeof(int) + sizeof(heap_block));
    heap_block* block = heapBlockHead.next;
    heap_block** current = &(heapBlockHead.next);
    
    while (block != NULL && block->free != 0) //find next free space
    { 
        if (block->size >= size)   // if size of block is equal or greater
        {
			block->next->free = 1;  // set free var to allocated
            *current = block->next; // set current
            return ((char*)block) + sizeof(heap_block);
        }
        current = &(block->next); // set current
        block = block->next; // set block for loop
    }

    block = (heap_block*)sbrk(size); // call sbrk to allocate memory
    block->size = size;  // set the size to accurately reflect the heapBlocks size

    return ((char*)block) + sizeof(heap_block); // return
  //return sbrk(size);
}


void* nu_calloc(size_t count, size_t size) 
{
	size = count + size;
	heap_block* block = nu_malloc(size); // malloc a new block
	
	if(block != NULL) // initialize it to zeros (free is false when one)
	{
		block->size = 0;
		block->free = 1;
		block->next = NULL;
		return block; // return calloced block
	}
  return NULL;  // return NULL 
}


void* nu_realloc(void* ptr, size_t size) // a lot like malloc but a bit trickier
{
	if(ptr == NULL) // equivalent to malloc
	{		
		size = (size + sizeof(int) + sizeof(heap_block));
		heap_block* block = nu_malloc(size);
      return block; 
	}
	if(size == 0) // equivalent to free
	{
		nu_free(ptr);
		return NULL;
	}
	
	size = (size + sizeof(int) + sizeof(heap_block));
	heap_block* block = (heap_block*)(((char*)ptr) - sizeof(heap_block));
    heap_block** current = &(block->next);
    
    while (block != NULL && block->free != 0) // start while loop like malloc
    {
        if (block->size >= size) 
        {
			block->next->free = 1;
            *current = block->next;
            return ((char*)block) + sizeof(heap_block);
        }
        current = &(block->next);
        block = block->next;
    }

    block = (heap_block*)sbrk(size); // realloc size
    block->size = size; // set relloced size

    return ((char*)block) + sizeof(heap_block); //mostly like malloc all around
}


void nu_free(void* ptr) //free memory associated with heapBlock
{
  heap_block* block = (heap_block*)(((char*)ptr) - sizeof(heap_block));
    block->next = heapBlockHead.next; // set next to the next of empty block
    heapBlockHead.next = block; //set empty block to memory being freed
    heapBlockHead.next->free = 0; //set free variable to 9 so we know its available
}
