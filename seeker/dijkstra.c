#include "dijkstra.h"

int Dijkstra(NodeStruct *Map,int MapSize,int Start,int Finish)
{
    //Start Dijkstra algorithm
    VisitNode(Map, MapSize, Start, Finish);

    //Trace found route of Dijkstra algorithm
    return TraceRoute(Map, Finish);
}

void VisitNode(NodeStruct *Map, int MapSize, int Start, int Finish)
{
    int currentPosition = Start;
    int currentNeighbour = 0;
    int i;
    Node* nodeList;
    Node* nodePointer;
    Node* currentNode;

    //Set distance value of start position
    (Map + Start)->DV = 0;

    //Initialise list of nodes on the map to visit
    nodeList = (Node*) malloc (sizeof(Node));
    nodeList->Node = &Map[Start];
    nodeList->ID = Start;
    nodeList->Next = NULL;

    while(currentPosition != Finish && nodeList != NULL)
    {
        //Select a node from the list with the shortest distance value and not visited yet.
        currentNode = nodeList;
        nodePointer = nodeList;

        while(nodePointer->Next != NULL)
        {
            if(nodePointer->Next->Node->DV < currentNode->Node->DV) //Node has a shorter distance value
            {
                currentNode = nodePointer->Next;
            }
            nodePointer = nodePointer->Next;
        }

        currentPosition = currentNode->ID;

        //Check for not visited yet, else delete from list
        if(currentNode->Node->Visited == 0)
        {
            //Checking neighbours
            for(i = 0; i < 4; i++)      //Neigbours 0-3: N,E,S,W
            {
                currentNeighbour = (Map + currentPosition)->Neighbours[i];
                if(currentNeighbour != -1)    //Check for neighbour
                {
                    if((Map + currentNeighbour)->Visited == 0)     //Check for already visited
                    {
                       if((Map + currentNeighbour)->DV > ((Map + currentPosition)->DV + (Map + currentPosition)->Distance[i]))  //Found a shorter route
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
            (Map + currentPosition)->Visited = 1;

            //Add neighbours to list for visiting
            for(i = 0; i < 4; i++)      //Neighbours 0-3: N,E,S,W
            {
                currentNeighbour = (Map + currentPosition)->Neighbours[i];
                if(currentNeighbour != -1)      //Check for neighbour
                {
                    if((Map + currentNeighbour)->Visited == 0)     //Check for already visited
                    {
                        //Add neighbour to start of list
                        nodePointer = (struct Node*) malloc (sizeof(struct Node));
                        nodePointer->Next = nodeList;
                        nodePointer->Node = (Map + currentNeighbour);
                        nodePointer->ID = currentNeighbour;

                        nodeList = nodePointer;
                    }
                }
            }
        }

        //Delete current node out of the list
        bool found = false;
        nodePointer = nodeList;

        //Current node is the first node of the list
        if(nodeList == currentNode)
        {
            nodeList = currentNode->Next;
            found = true;
        }

        //Check for node in the list if not found
        while(nodePointer->Next != NULL && !found)
        {
            if(nodePointer->Next == currentNode)
            {
                nodePointer->Next = currentNode->Next;
                found = true;
            }

            if(nodePointer->Next != NULL)
            {
                nodePointer = nodePointer->Next;
            }
        }

        //Free allocated memory
        free(currentNode);
    }

    //Free remaining nodes
    while(nodeList != NULL)
    {
        nodePointer = nodeList;
        nodeList = nodeList->Next;
        free(nodePointer);      //Release allocated memory
    }
}

int TraceRoute(NodeStruct* Map, int Finish)
{
    int currentPosition = Finish;
    int nextNode;
    int prevNode;
    bool found;
    int i;
    int steps = 0;

    //Trace route from finish to start
    while((Map + currentPosition)->Previous != -1)
    {
        prevNode = (Map + currentPosition)->Previous;

        (Map + prevNode)->Next = currentPosition;

        currentPosition = prevNode;

        //Count number of nodes without starting node
        steps++;
    }

    //Set relative direction of start node (2: Forward)
    (Map + currentPosition)->NextRelDir = 2;

    //Calculate absolute and relative directions for the route
    while((Map + currentPosition)->Next != -1)
    {
        nextNode = (Map + currentPosition)->Next;
        prevNode = (Map + currentPosition)->Previous;

        i = 0;
        found = false;

        //Calculate next absolute direction
        while(!found && i < 4)      //Neighbours 0-3: N,E,S,W
        {
            if((Map + currentPosition)->Neighbours[i] == nextNode)
            {
                (Map + currentPosition)->NextAbsDir = i;        //NextAbsDir 0-3: N,E,S,W
                found = true;
            }
            i++;
        }

        i = 0;
        found = false;

        //Calculate previous absolute direction for next relative direction
        if(prevNode != -1)
        {
            while(!found && i < 4)      //Neighbours 0-3: N,E,S,W
            {
                if((Map + currentPosition)->Neighbours[i] == prevNode)
                {
                    int prevAbsDirection = i;       //PrevAbsDir 0-3: N,E,S,W

                    //Calculate next relative direction
                    (Map + currentPosition)->NextRelDir = GetRelDirection(prevAbsDirection, ((Map + currentPosition)->NextAbsDir));
                    found = true;
                }
                i++;
            }
        }

        currentPosition = nextNode;
    }

    //Set relative direction of end node (-1: No direction)
    (Map + currentPosition)->NextRelDir = -1;

    return steps;
}

int GetRelDirection(int PrevAbsDir, int NextAbsDir)
{
    //Calculate relative direction
    switch(PrevAbsDir)
    {
        //From NORTH
        case 0:
            switch(NextAbsDir)
            {
                //Go EAST
                case 1:
                    return 1;   //Turn LEFT
                    break;
                //Go SOUTH
                case 2:
                    return 2;   //Go STRAIGHT
                    break;
                //Go WEST
                case 3:
                    return 3;   //Turn RIGHT
                    break;
            }
            break;
        //From EAST
        case 1:
            switch(NextAbsDir)
            {
                //Go NORTH
                case 0:
                    return 3;   //Turn RIGHT
                    break;
                //Go SOUTH
                case 2:
                    return 1;   //Turn LEFT
                    break;
                //Go WEST
                case 3:
                    return 2;   //Go STRAIGHT
                    break;
            }
            break;
        //From SOUTH
        case 2:
            switch(NextAbsDir)
            {
                //Go NORTH
                case 0:
                    return 2;   //Go STRAIGHT
                    break;
                //Go EAST
                case 1:
                    return 3;   //Turn RIGHT
                    break;
                //Go WEST
                case 3:
                    return 1;   //Turn LEFT
                    break;
            }
            break;
        //From WEST
        case 3:
            switch(NextAbsDir)
            {
                //Go NORTH
                case 0:
                    return 1;   //Turn LEFT
                    break;
                //Go EAST
                case 1:
                    return 2;   //Go STRAIGHT
                    break;
                //Go SOUTH
                case 2:
                    return 3;   //Turn RIGHT
                    break;
            }
            break;
    }

    //Invalid direction
    return -1;
}

void InitMap(NodeStruct* Map, int map, int n,  int o, int z, int w, int nn, int oo, int zz, int ww)
{
	Map[map].Neighbours[0] = n;
	Map[map].Neighbours[1] = o;
	Map[map].Neighbours[2] = z;
	Map[map].Neighbours[3] = w;

	Map[map].Distance[0] = nn;
	Map[map].Distance[1] = oo;
	Map[map].Distance[2] = zz;
	Map[map].Distance[3] = ww;

	Map[map].Visited = 0;

	Map[map].DV = 120;
	Map[map].Previous = -1;
	Map[map].Next = -1;
	Map[map].NextAbsDir = -1;
	Map[map].NextRelDir = -1;
}

