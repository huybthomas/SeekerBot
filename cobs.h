#ifndef COBS
#define COBS

int encodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer);

int decodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer);

int encodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer);

int decodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer);

#endif
