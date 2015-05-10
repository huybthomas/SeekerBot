#include <stdio.h>
#include "main.h"
#include "cobs.h"

int main(void)
{
    int i;
    int length = 12;
    int newLength;

    unsigned char message[12] = {0x45, 0x00, 0x00, 0x2C, 0x4C, 0x79, 0x00, 0x00, 0x40, 0x06, 0x4F, 0x37};
    unsigned char encoded[13];
    unsigned char decoded[12];
    unsigned char encodedZPE[11];
    unsigned char decodedZPE[12];

    printf("COBS encoding\nOriginal message:\n");

    for(i = 0; i < length; i++)
    {
        printf("%02X ", message[i]);
    }

    //Encode message with COBS
    newLength = encodeCOBS(message, length, encoded);

    printf("\nEncoded message:\n");

    for(i = 0; i < newLength; i++)
    {
        printf("%02X ", encoded[i]);
    }

    //Decode message with COBS
    newLength = decodeCOBS(encoded, newLength, decoded);

    printf("\nDecoded message:\n");

    for(i = 0; i < newLength; i++)
    {
        printf("%02X ", decoded[i]);
    }

    //Encode message with COBS with ZPE
    newLength = encodeCOBSZPE(message, length, encodedZPE);

    printf("\nEncoded message with ZPE:\n");

    for(i = 0; i < newLength; i++)
    {
        printf("%02X ", encodedZPE[i]);
    }

    //Decode message with COBS with ZPE
    newLength = decodeCOBSZPE(encodedZPE, newLength, decodedZPE);

    printf("\nDecoded message with ZPE:\n");

    for(i = 0; i < newLength; i++)
    {
        printf("%02X ", decodedZPE[i]);
    }

	return 0;
}



