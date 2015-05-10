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
                DriveRotateRWheel(90.0, Speed);         //Turn left
                printf("Turn left\n");
                break;
            case 2:
                DriveStraightDistance(120.0, Speed);    //Straight forward
                printf("Going straight\n");
                break;
            case 3:
                DriveRotateLWheel(90.0, Speed);         //Turn right
                printf("Turn right\n");
                break;
        }

        DriveLineFollow(Speed);                         //Drive to end of line

        previousPoint = Map[previousPoint].Next;        //Get next node

        if(previousPoint == -1)
        {
            hasNext = false;                            //End of route
        }
    } while(hasNext);

    return 0;
}
