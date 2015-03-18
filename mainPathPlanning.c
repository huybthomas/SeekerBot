#include <stdio.h>
#include "dijkstra.h"
#include "main.h"

int main(void)
{
	NodeStruct mapLabo[12];
	int Start = 6;
	int Finish = 8;

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

    //Run Dijkstra algorithm
    int routeSize = Dijkstra(mapLabo,12,Start,Finish);

    printf("Calculating route finished. Route size: %d\n", routeSize);
    printf("Starting from node: %d\n", Start);
    printf("Route: ");

    int i;
    int currentNode = Start;
    int prevNode;

    for(i = 0; i < routeSize; i++)
    {
        currentNode = (mapLabo + currentNode)->Next;

        printf("%d ", currentNode);
    }

    printf("\nRoute Finished!\n");

    printf("\nTable information:\n");
    for(i = 0; i < 12; i++)
    {
        printf("Node %d: NEXT: %d PREVIOUS: %d ABSNEXT: %d RELNEXT: %d\n", i, (mapLabo + i)->Next, (mapLabo + i)->Previous, (mapLabo + i)->NextAbsDir, (mapLabo + i)->NextRelDir);
    }

    printf("\nEnjoy your route\n");

	return 0;
}



