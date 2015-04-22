#include "dijkstra.h"

//init map structure
private NodeStruct map[12];

//Initialise map
InitMap(mapLabo, 0, -1, 4, -1, -1, -1, 8, -1, -1);
InitMap(mapLabo, 1, -1, -1, -1, -1, -1, -1, -1, -1);
InitMap(mapLabo, 2, -1, 3, -1, -1, -1, 4, -1, -1);
InitMap(mapLabo, 3, -1, 4, 7, 2, -1, 1, 4, 4);
InitMap(mapLabo, 4, 0, 5, -1, 3, 8, 1, -1, 1);
InitMap(mapLabo, 5, 11, 9, 7, 4, 2, 1, 11, 1);
InitMap(mapLabo, 6, -1, 7, -1, -1, -1, 4, -1, -1);
InitMap(mapLabo, 7, 3, 5, -1, 6, 4, 11, -1, 4);
InitMap(mapLabo, 8, 9, -1, -1, -1, 4, -1, -1, -1);
InitMap(mapLabo, 9, 11, -1, 8, 5, 1, -1, 4, 1);
InitMap(mapLabo, 10, 11, -1, -1, -1, 7, -1, -1, -1);
InitMap(mapLabo, 11, -1, 10, 9, 5, -1, 7, 1, 2);

