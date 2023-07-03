/*
 * TimothyLee     
 * 02/04/14
 * 
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


// Included for the `size_t` type. 
#include <string.h>

typedef struct _heap_block heap_block;  // came from the book and figured that's the data structure we had to use
struct _heap_block {                    // I now think I may have been mistaken
  int size;
  int free;
  heap_block* next;
};

static heap_block heapBlockHead = { 0, 0, NULL }; // a static heap initialized to null for use in malloc.c


void* nu_malloc(size_t);             // prototypes for the nu implementations of our system funtions
void* nu_calloc(size_t, size_t);
void* nu_realloc(void*, size_t);
void nu_free(void*);
