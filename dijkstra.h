#ifndef DIJKSTRA
#define DIJKSTRA

#include <stdio.h>
#include <stdbool.h>

#define NON -1
typedef struct
{
	int Neighbours[4];		//Index of neighbouring nodes, in order N,E,S,W   -1= no connection
	int Distance[4];		//Distance to neighbouring nodes, in order N,E,S,W

	//Solver variables
	char Visited;			//flag: visited or not
	int DV;                 //Distance value
	int Previous;           //Previous node
	int Next;               //Next node
	int NextAbsDir;         //Absolute direction to next node (0=N, 1=E, 2=S, 3=W)
	int NextRelDir;         //Relative direction to next node (-1:no direction, 1=turn left, 2=forward, 3=turn right)
} NodeStruct;

//Linked list for nodes
typedef struct Node
{
    int ID;
    NodeStruct* Node;
    struct Node* Next;
} Node;

/**
 * \brief Calculate path using dijkstra method
 * \param Map :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
 * \return
 * Path length expressed in number of nodes (not including start node)
*/
int Dijkstra(NodeStruct *Map,int MapSize,int Start,int Finish);

/**
 * \brief First step in the Dijkstra algorithm to calculate the shortest path from start to finish
 * \param Map :Array of nodes describing the map
 * \param MapSize :Number of nodes in Map
 * \param Start :Start node index for the path to be calculated
 * \param Finish :Finish node index for the path to be calculated
 */
void VisitNode(NodeStruct *Map, int MapSize, int Start, int Finish);

/**
 * \brief Second step in the Dijkstra algorithm to find the shortest path from finish to start
 * \param Map :Array of nodes describing the map
 * \param Finish :Finish node index for the path to be calculated
 * \return Path length expressed in number of nodes (not including start node)
 */
int TraceRoute(NodeStruct* Map, int Finish);

/**
 * \brief Get the direction to the next node relative to the previous node
 * \param PrevAbsDir :Absolute direction of the previous node entry point to the current node (0-3: N,E,S,W)
 * \param NextAbsDir :Absolute direction of the next node entry point to the current node (0-3: N,E,S,W)
 * \return Relative direction to the next node in respect to the previous node (0-3: N,E,S,W)
 */
int GetRelDirection(int PrevAbsDir, int NextAbsDir);

/**
 * \brief Add a new node to the NodeStruct for initialisation of the map for the Dijkstra algorithm
 * \param Map :Array of nodes describing the map
 * \param map :Index for the new node to be initialized
 * \param n :Index of the neighbour in the north (-1: no neighbour)
 * \param o :Index of the neighbour in the east (-1: no neighbour)
 * \param z :Index of the neighbour in the south (-1: no neighbour)
 * \param w :Index of the neighbour in the west (-1: no neighbour)
 * \param nn :Cost of the route to the north
 * \param oo :Cost of the route to the east
 * \param zz :Cost of the route to the south
 * \param ww :Cost of the route to the west
 */
void InitMap(NodeStruct* Map, int map, int n,  int o, int z, int w, int nn, int oo, int zz, int ww);

#endif
