#ifndef MAP_H
#define MAP_H

#include "dijkstra.h"

#define MAPSIZE 12

//init map structure
NodeStruct map[MAPSIZE];

 //init tag UID's
extern const char* nodeUID[MAPSIZE];

/**
 * \brief Initialize the map struct for the paths in the labo
 * \param map :Pointer the the map struct where to initialize the map
 */
void initializeMap(NodeStruct* map);

#endif
