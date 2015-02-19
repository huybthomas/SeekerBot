#ifndef QRCODE_H
#define QRCODE_H

#include "robotapp.h"

/**
 * \brief Take still picture with raspberry PI camera, decode with zbar tool
 * \param Data :Location to store decoded QR code data payload (string)
 * \param MaxDataLen :Maximum data length that can be stored in 'Data' (string terminator NULL character included)
 * \return
 * 0:OK \n
 * 1:Camera error \n
 * 2:No QR code detected \n
 * 3:Other error \n
*/
int QRCodeDecode(char *Data,int MaxDataLen);


#endif
