#include <stdio>

int Dijkstra(NodeStruct *Map,int MapSize,int Start,int Finish)
{
    int currentPosition = Start;
    int currentNeighbour = 0;

    while(currentPosition != Finish)
    {
        for(int i = 0; i < 4; i++)      //Neigbours 0-3: N,E,S,W
        {
            currentNeighbour = (Map + currentPosition)->Neighbours[i];
            if(currentNeighbour != -1)    //Check for neigbour
            {
                if((Map + currentNeighbour)->Visited == 'F')     //Check for already visited
                {
                   if((Map + currentNeighbour)->DV < ((Map + currentPosition)->DV + (Map + currentPosition)->Distance[i]))  //Found a shorter route
                   {
                        //Set new distance
                        (Map + currentNeighbour)->DV = ((Map + currentPosition)->DV + (Map + currentPosition)->Distance[i]);

                        //Set new previous node
                        (Map + currentNeighbour)->Previous = currentPosition;
                    }
                }
            }
        }

        //Set visited flag of current node
        (Map + currentPosition)->Visited = 'T';


}
