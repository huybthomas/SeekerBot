#include "map.h"

void initializeMap(NodeStruct* map)
{
    //Initialize map
    InitMap(map, 0, -1, 4, -1, -1, -1, 5, -1, -1);
    InitMap(map, 1, -1, -1, -1, -1, -1, -1, -1, -1);
    InitMap(map, 2, -1, 3, -1, -1, -1, 2, -1, -1);
    InitMap(map, 3, -1, 4, 7, 2, -1, 2, 3, 2);
    InitMap(map, 4, 0, 5, -1, 3, 5, 2, -1, 2);
    InitMap(map, 5, 11, 9, 7, 4, 3, 2, 10, 2);
    InitMap(map, 6, -1, 7, -1, -1, -1, 2, -1, -1);
    InitMap(map, 7, 3, 5, -1, 6, 3, 10, -1, 2);
    InitMap(map, 8, 9, -1, -1, -1, 2, -1, -1, -1);
    InitMap(map, 9, 11, -1, 8, 5, 2, -1, 2, 2);
    InitMap(map, 10, 11, -1, -1, -1, 5, -1, -1, -1);
    InitMap(map, 11, -1, 10, 9, 5, -1, 5, 2, 3);
}
