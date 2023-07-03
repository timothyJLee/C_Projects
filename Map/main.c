#include <assert.h>
#include <stdio.h>
#include "map.h"
#include "map.c"

int main(){
map_t* newList = (map_t*)malloc(sizeof(map_t));
map_init(newList);
printf("Map Initialized...\n");

FILE *fp = fopen("c://serialize.bin", "wb");

char* firstString ="f";
char* secondString="2";
map_put(newList,"3","3");
map_put(newList,"7","34");
map_put(newList,"a","45");
map_put(newList,"f","45");
map_put(newList,"2","45");
map_put(newList,"1","45");
map_put(newList,"8","13");
map_put(newList,firstString,secondString);

printf("The size of the map is: %d\n", map_size(newList));
printf("\n");

printf("map_get(newList, firstString): %s\n", map_get(newList, firstString));
firstString = "7";
printf("map_get(newList, firstString): %s\n", map_get(newList, firstString));
firstString = "8";
printf("map_get(newList, firstString): %s\n", map_get(newList, firstString));
printf("\n");

printf("map_remove(newList,firstString)\n");
map_remove(newList, firstString);
printf("map_get(newList, firstString): %s\n", map_get(newList, firstString));
printf("The size of the map is: %d\n", map_size(newList));
printf("\n");

map_serialize(newList,fp);
fclose(fp);
fp = fopen("c://serialize.bin", "wb");

map_deserialize(newList,fp);
fp = fopen("c://serialize.bin", "wb");
fclose(fp);

printf("map_destroy(newList)\n");
map_destroy(newList);

return 1;
}
