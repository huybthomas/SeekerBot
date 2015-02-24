#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);


    printf ("Initialisation complete\n");

    int result;
    char Data[24];

    result = TagReaderGetUID(Data);

    if(result==0)
    {
        printf("Tag UID:\n");
        printf("%s\n", Data);
    }
    else
    {
        printf("No RF-tag detected! Error code: %d\n", result);
    }

    printf ("Ready.\n");
}
