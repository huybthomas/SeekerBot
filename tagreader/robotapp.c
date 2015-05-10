#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);


    printf("Initialisation complete\n");

    int result;
    char Data[24];

    //Scan UID of tag
    result = TagReaderGetUID(Data);

    if(result == 0)
    {
        printf("Tag UID:\n");
        printf("%s\n", Data);
    }
    else
    {
        printf("No RF-tag detected! Error code: %d\n", result);
    }

    int DataBlock = 4;
    int DataLength = 4;
    char TestData[12] = "04 03 02 01";

    //Write data to tag
    result = TagReaderWriteData(DataBlock, DataLength, TestData);

    if(result == 0)
    {
        printf("Write %d bytes from \"%s\" to datablock %d.\n", DataLength, TestData, DataBlock);
    }
    else
    {
        printf("Data not written to RF-tag! Error code: %d\n", result);
    }

    char ReceivedData[12];

    //Read data from tag
    result = TagReaderReadData(DataBlock, DataLength, ReceivedData);

    if(result == 0)
    {
        printf("Reading %d bytes from datablock %d: \"%s\"\n", DataLength, DataBlock, ReceivedData);
    }
    else
    {
        printf("Cannot read data from RF-tag! Error code: %d\n", result);
    }

    printf ("Ready.\n");
}
