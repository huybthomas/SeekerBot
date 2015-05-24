#include "travel.h"

int Travel(NodeStruct* Map, int MapSize, int Start, int Finish, float Speed)
{
    bool hasNext = true;
    int previousPoint = Start;

    do
    {
        switch(Map[previousPoint].NextRelDir)
        {
            case 1:
                printf("Turn left\n");
                DriveRotateRWheel(90.0, Speed);         //Turn left
                break;
            case 2:
                printf("Going straight\n");
                DriveStraightDistance(120.0, Speed);    //Straight forward
                break;
            case 3:
                printf("Turn right\n");
                DriveRotateLWheel(90.0, Speed);         //Turn right
                break;
        }

        DriveLineFollow(Speed);                         //Drive to end of line

        previousPoint = Map[previousPoint].Next;        //Get next node

        if(Map[previousPoint].Next == -1)
        {
            hasNext = false;                            //End of route
        }

    } while(hasNext);

    return 0;
}
