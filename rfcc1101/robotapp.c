#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);

    int srcRFAddress = 6;
    int destRFAddress = 2;
    uint8 receiveStatus = 0;
    uint8 receivePollFailed = 0;
    unsigned char message[60] = "Hello from robot6!";
    RfCommsPacket rfPacket;

    printf("Initialisation complete\n");

    if(RfCommsInit(srcRFAddress) != 0)
    {
        printf("Error while initialising RF-module\n");
        system("espeak -ven+f2 -k5 -a50 -s150 \"Error.\" --stdout | aplay 2>/dev/null");
        return;
    }

    //Clear receive buffer
    receivePollFailed = RfCommsReceivePoll(&receiveStatus);

    while(!receivePollFailed && receiveStatus)
    {
        if(RfCommsReceivePacket(&rfPacket) != 0)
        {
            receivePollFailed = 1;
        }
        else
        {
            receivePollFailed = RfCommsReceivePoll(&receiveStatus);
        }
    }

    if(receivePollFailed)
    {
        printf("Error while clearing receive buffer!\n");
    }

    printf("Sending message '%s' to address: %d\n", message, destRFAddress);
    system("espeak -ven+f2 -k5 -a50 -s150 \"Sending message.\" --stdout | aplay 2>/dev/null");

    //rfPacket.DataLen = encodeLength;
    rfPacket.DataLen = strlen((const char*)message);
    rfPacket.DstRfAddr = destRFAddress;
    rfPacket.SrcRfAddr = srcRFAddress;
    rfPacket.Cmd = 0;
    rfPacket.Lqi = 0;
    rfPacket.Rssi = 0;

    strncpy((char*)rfPacket.Data, (const char*)message, strlen((const char*)message));

    //Send message to destination
    if(RfCommsSendPacket(&rfPacket) != 0)
    {
        printf("Error while sending message to address: %d\n", destRFAddress);
        system("espeak -ven+f2 -k5 -a50 -s150 \"Error.\" --stdout | aplay 2>/dev/null");
    }

    printf("Waiting for acknownledge message...\n");

    receiveStatus = 0;
    receivePollFailed = 0;

    while(!receivePollFailed && !receiveStatus)
    {
        receivePollFailed = RfCommsReceivePoll(&receiveStatus);
    }

    if(receivePollFailed)
    {
        printf("Error while polling for new message!\n");
    }
    else
    {
        if(RfCommsReceivePacket(&rfPacket) == 0)
        {
            printf("Message: %s\n", rfPacket.Data);
        }
        else
        {
            printf("Error while reading new message!\n");
        }
    }

    printf("Ready.\n");
}
