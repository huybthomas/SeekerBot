#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);


    printf ("Initialisation complete\n");

    int result;
    char Data[64];

    result = QRCodeDecode(Data,64);

    if(result == 0)
    {
        printf("QR-code:\n");
        printf("%s\n", Data);
    }
    else
    {
        printf("No QR-code detected! Error code: %d\n", result);
    }

    printf ("Ready.\n");
}
