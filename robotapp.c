#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);


    printf ("Initialisation complete\n");

    int result;
    char* Data;


    result = QRCodeDecode(Data,64);

    if(result == 0)
    {
        printf("QR-code: \n");
        printf(Data);
    }

    printf ("Ready.\n");
}
