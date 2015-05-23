#ifndef TAGREADER_H
#define TAGREADER_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "robotapp.h"

/**
 * \brief Scan for passive tag in range, return 7 byte UID
 * \param Data :Location to store tag UID. Buffer must be 21 bytes long at least. UID is stored as a NULL terminated string
 * \return
 * 0:OK \n
 * 1:Tag reader error \n
 * 2:No tag detected \n
 * 3:Other error \n
*/
int TagReaderGetUID(char *Data);

/**
 * \brief Write byte data to a passive tag in range
 * \param BlockNumber :The starting block to be updated
 * \param DataLength :The number of bytes to be updated
 * \param Data :New data to store on the tag.
 * \return
 * 0:OK \n
 * 1:Tag reader error \n
 * 2:No tag detected \n
 * 3:Other error \n
*/
int TagReaderWriteData(int BlockNumber, int DataLength, char* Data);

/**
 * \brief Read byte data from a passive tag in range
 * \param BlockNumber :The starting block to be readed from
 * \param Data :Location to store the data
 * \param DataLength :The number of bytes to read
 * 0:OK \n
 * 1:Tag reader error \n
 * 2:No tag detected \n
 * 3:Other error \n
*/
int TagReaderReadData(int BlockNumber, int DataLength, char* Data);

#endif
