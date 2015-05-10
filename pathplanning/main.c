#include "main.h"

int main(void)
{
	NodeStruct mapLabo[12];
	int Start = 10;
	int Finish = 4;

    //Initialise map
	initializeMap(mapLabo);

    //Run Dijkstra algorithm
    int routeSize = Dijkstra(mapLabo,12,Start,Finish);

    printf("Calculating route finished. Route size: %d\n", routeSize);
    printf("Starting from node: %d\n", Start);
    printf("Route: ");

    int i;
    int currentNode = Start;

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



