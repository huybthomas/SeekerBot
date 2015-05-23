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
    int res = 0;
    unsigned char encodeMessageBuffer[59];
    unsigned char encodeBuffer[2];
    uint8 dataLength;

    if(!initialized)
    {
        return 1;
    }

    if(Packet->DataLen >= 59)
    {
        return 1;
    }

    //Encode Data
    dataLength = encodeCOBSZPE(Packet->Data, Packet->DataLen, encodeMessageBuffer);

    //Encode and send Data length
    encodeCOBSZPE(&dataLength, 1, encodeBuffer);
    res =+ RfCC1101FIFOSendData(&RfCC1101, encodeBuffer, Packet->DstRfAddr);

    //Encode and send Source RF address
    encodeCOBSZPE(&Packet->SrcRfAddr, 1, encodeBuffer);
    res =+ RfCC1101FIFOSendData(&RfCC1101, encodeBuffer, Packet->DstRfAddr);

    //Encode and send Cmd
    encodeCOBSZPE(&Packet->Cmd, 1, encodeBuffer);
    res =+ RfCC1101FIFOSendData(&RfCC1101, encodeBuffer, Packet->DstRfAddr);

    //Send Data
    res =+ RfCC1101FIFOSendData(&RfCC1101, encodeMessageBuffer, Packet->DstRfAddr);

    if(res != 0)
    {
        printf("RF packet could not be sended. Error code: %d\n", res);
        return 1;
    }

    return 0;
}

int RfCommsReceivePacket(RfCommsPacket *Packet)
{
    int res = 0;
    uint8 decodeBuffer[64];
    uint8 receivedData[64];
    int decodeLength;

    if(!initialized)
    {
        return 1;
    }

    //Receive and decode Data length
    res =+ RfCC1101FIFOReceiveData(&RfCC1101, decodeBuffer, &Packet->Rssi, &Packet->Lqi);
    if(res == 0)
    {
        decodeCOBSZPE(decodeBuffer, 2, &Packet->DataLen);
    }

    //Receive and decode Source RF address
    res =+ RfCC1101FIFOReceiveData(&RfCC1101, decodeBuffer, &Packet->Rssi, &Packet->Lqi);
    if(res == 0)
    {
        decodeCOBSZPE(decodeBuffer, 2, &Packet->SrcRfAddr);
    }

    //Receive and decode Cmd
    res =+ RfCC1101FIFOReceiveData(&RfCC1101, decodeBuffer, &Packet->Rssi, &Packet->Lqi);
    if(res == 0)
    {
        decodeCOBSZPE(decodeBuffer, 2, &Packet->Cmd);
    }

    //Receive and decode data
    res =+ RfCC1101FIFOReceiveData(&RfCC1101, receivedData, &Packet->Rssi, &Packet->Lqi);
    if(res == 0)
    {
        decodeLength = decodeCOBSZPE(receivedData, Packet->DataLen, Packet->Data);
        Packet->Data[decodeLength] = '\0';
    }

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
