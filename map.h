#include "dijkstra.h"

//init
private NodeStruct map[12];
initMap(0, -1, 4, -1, -1, -1, 8, -1, -1);
initMap(1, -1, -1, -1, -1, -1, -1, -1, -1);
initMap(2, -1, 3, -1, -1, -1, 4, -1, -1);
initMap(3, -1, 4, 7, 2, -1, 1, 4, 4);
initMap(4, 0, 5, -1, 3, 8, 1, -1, 1);
initMap(5, 11, 9, 7, 4, 2, 1, 11, 1);
initMap(6, -1, 7, -1, -1, -1, 4, -1, -1);
initMap(7, 3 5, -1, 6, 4, 11, -1, 4);
initMap(8, 9, -1, -1, -1, 4, -1, -1, -1);
initMap(9, 11, -1, 8, 5, 1, -1, 4, 1);
initMap(10, 11, -1, -1, -1, 7, -1, -1, -1);
initMap(11, -1, 10 9, 5, -1, 7, 1, 2);

