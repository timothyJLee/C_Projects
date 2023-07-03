#ifndef __A1_MAP_H__
#define __A1_MAP_H__
#include <stdio.h>
typedef struct _map_entry map_entry_t;
struct _map_entry {
  char* key;
  char* value;
  map_entry_t* next;
};

typedef struct _map {
  map_entry_t* entry;
  int size;
} map_t;

// Part one functions.
void map_init(map_t*);
int map_put(map_t*, const char*, const char*);
const char* map_get(map_t*, const char*);
int map_remove(map_t*, const char*);
int map_size(map_t*);
void map_destroy(map_t*);

// Part two functions. 
int map_serialize(map_t*, FILE*);
int map_deserialize(map_t*, FILE*);

#endif
