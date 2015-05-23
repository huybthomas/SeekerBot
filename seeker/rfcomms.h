#ifndef RFCOMMS_H
#define RFCOMMS_H

#include "robotapp.h"
#include "cobs.h"

#define RFCOMMS_MAX_DATA_LEN 32

#pragma pack(push)
#pragma pack(1)

//Packet structure
typedef struct
{
    sint16 Rssi;    ///<Received signal strength
    uint8 Lqi;      ///<Link Quality Indicator
    uint8 DstRfAddr;///<Destination RF address

    //Transmitted OTA
    uint8 DataLen;  ///<Number of data bytes in packet (Data array)
    uint8 SrcRfAddr;///<Source RF address
    uint8 Cmd;      ///<Command byte
    uint8 Data[RFCOMMS_MAX_DATA_LEN];///<Data array. 'DataLen' bytes are used
} RfCommsPacket;
#pragma pack(pop)

/**
 * \brief Initialize RF communications system
 * \param Channel :RF channel to listen from
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsInit(int Channel);

/**
 * \brief Send data packet in RfCommsPacket struct form
 * \param Packet :Pointer to packet to be transmitted
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsSendPacket(RfCommsPacket *Packet);

/**
 * \brief Receive data packet in RfCommsPacket struct form. Blocks until packet is received.
 * \param Packet :Pointer to packet receive buffer
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsReceivePacket(RfCommsPacket *Packet);

/**
 * \brief Polls whether packet is ready in receive buffer
 * \param PollStatus :Pointer to poll status variable. 0: No packet ready 1: Packet ready
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int RfCommsReceivePoll(uint8 *PollStatus);
#endif
