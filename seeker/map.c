#include "map.h"

const char* nodeUID[12] = {
    "",                             //0
    "",                             //1
    "",                             //2
    "04 47 E4 22 A9 34 80",         //3
    "04 EC D6 12 A9 34 80",         //4
    "04 63 13 22 A9 34 84",         //5
    "",                             //6
    "04 88 FE 22 A9 34 80",         //7
    "",                             //8
    "04 24 E6 22 A9 34 80",         //9
    "",                             //10
    "04 D0 04 12 A9 34 84"};        //11

void initializeMap(NodeStruct* map)
{
    /**
      Map :Array of nodes describing the map
      map :Index for the new node to be initialized
      n :Index of the neighbour in the north (-1: no neighbour)
      o :Index of the neighbour in the east (-1: no neighbour)
      z :Index of the neighbour in the south (-1: no neighbour)
      w :Index of the neighbour in the west (-1: no neighbour)
      nn :Cost of the route to the north
      oo :Cost of the route to the east
      zz :Cost of the route to the south
      ww :Cost of the route to the west
    **/

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
