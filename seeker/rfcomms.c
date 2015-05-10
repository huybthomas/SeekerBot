#include "rfcomms.h"

int initialized = 0;

int RfCommsInit(int Channel)
{
    int res;

    //Initialize RF-CC1101 module
    RfCC1101.RS485Client = &RS485Client;
    RfCC1101.Frequency = 433920000;
    RfCC1101.ModType = CC1101_MOD_FSK;
    RfCC1101.Rate = CC1101_RATE_1200;
    RfCC1101.RFAddr = Channel;

    res = RfCC1101Init(&RfCC1101);
    if(res != 0)
    {
        printf("RfCC1101Init error code %d\n", res);
        return 1;
    }
    else
    {
        initialized = 1;
    }

    return 0;
}

int RfCommsSendPacket(RfCommsPacket *Packet)
{
    int res;

    if(!initialized)
    {
        return 1;
    }

    if(Packet->DataLen >= 59)
    {
        return 1;
    }

    res = RfCC1101FIFOSendData(&RfCC1101, Packet->Data, Packet->DstRfAddr);

    if(res != 0)
    {
        printf("RF packet could not be sended. Error code: %d\n", res);
        return 1;
    }

    return 0;
}

int RfCommsReceivePacket(RfCommsPacket *Packet)
{
    int res;

    if(!initialized)
    {
        return 1;
    }

    res = RfCC1101FIFOReceiveData(&RfCC1101, Packet->Data, &Packet->Rssi, &Packet->Lqi);

    if(res != 0)
    {
        printf("RF packet could not be received. Error code: %d\n", res);
        return 1;
    }

    return 0;
}

int RfCommsReceivePoll(uint8 *PollStatus)
{
    int res;

    if(!initialized)
    {
        return 1;
    }

    res = RfCC1101FIFOReceivePoll(&RfCC1101, PollStatus);

    if(res != 0)
    {
        printf("RF Receive FIFO could not be polled. Error code: %d\n", res);
        return 1;
    }

    return 0;
}
