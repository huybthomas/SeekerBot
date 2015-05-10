#ifndef PROJECT_H
#define PROJECT_H

#define PI
//#define IGEP

#ifdef IGEP
#define SER_PORT "/dev/ttyO1"
#endif

#ifdef PI
#define SER_PORT "/dev/ttyAMA0"
#endif

#define RS485_DATABUFFER_SIZE 256
#define RS485_TXRXBUFFER_SIZE RS485_DATABUFFER_SIZE+64
#define RS485CLIENT_MAX_BLOCK_SIZE 64

#endif
