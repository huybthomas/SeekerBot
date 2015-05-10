#include "cobs.h"

int encodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer)
{
    int i = 0;
    unsigned char nonZeroValueByte = 0x01;
    unsigned char* nonZeroValuePointer;

    if(length >= 255)
    {
        //Message is to long to encode with COBS
        return -1;
    }

    //Set code pointer to first byte of the output buffer
    nonZeroValuePointer = outBuffer;

    //Encode the whole message
    while(i < length)
    {
        //Check for zero byte
        if(*(inBuffer + i) == 0x00)
        {
            //Set encoded byte on the zero byte
            *nonZeroValuePointer = nonZeroValueByte;

            //Set new code pointer on the zero byte
            nonZeroValuePointer = (outBuffer + i + 1);

            //Reset counter to search for new encoded value
            nonZeroValueByte = 0x01;
        }
        else
        {
            //Copy byte to output buffer
            *(outBuffer + i + 1) = *(inBuffer + i);

            nonZeroValueByte++;
        }

        i++;
    }

    //Fill the last encoded byte on the zero byte
    *nonZeroValuePointer = nonZeroValueByte;

    //New encoded length
    return length + 1;
}

int decodeCOBS(unsigned char* inBuffer, int length, unsigned char* outBuffer)
{
    int i = 1;
    unsigned char zeroPosition;

    if(length > 255)
    {
        //Message is to long to decode with COBS
        return -1;
    }

    //Get first code byte
    zeroPosition = *inBuffer;

    //Decode the whole message
    while(i < length)
    {
        //Check for code byte
        if(i == zeroPosition)
        {
            //Get next zero position
            zeroPosition += *(inBuffer + i);

            //Set code byte to zero
            *(outBuffer + i - 1) = 0x00;
        }
        else
        {
            //Copy byte to output buffer
            *(outBuffer + i - 1) = *(inBuffer + i);
        }

        i++;
    }

    //New decoded length
    return length - 1;
}

int encodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer)
{
    int i = 0;
    int zeroPairs = 0;
    unsigned char nonZeroValueByte = 0x01;
    unsigned char* nonZeroValuePointer;
    const unsigned char zeroPairByte = 0XE0;

    if(length >= 224)
    {
        //Message is to long to encode with COBS/Zero Pair Elimination
        return -1;
    }

    //Set code pointer to first byte of the output buffer
    nonZeroValuePointer = outBuffer;

    //Encode the whole message
    while(i < length)
    {
        //Check for zero byte
        if(*(inBuffer + i) == 0x00)
        {
            //Zero pair elimination is limited for 30 non-zero bytes
            if(nonZeroValueByte <= 30)
            {
                //Check for zero pair
                if(*(inBuffer + i + 1) == 0x00)
                {
                    //Add zero pair byte to the non-zero byte counter
                    nonZeroValueByte += zeroPairByte;

                    //Increment inputbuffer pointer and zero pairs counters
                    i++;
                    zeroPairs++;
                }
            }

            //Set encoded byte on the zero byte
            *nonZeroValuePointer = nonZeroValueByte;

            //Set new code pointer on the zero byte
            nonZeroValuePointer = (outBuffer + i - zeroPairs + 1);

            //Reset counter to search for new encoded value
            nonZeroValueByte = 0x01;
        }
        else
        {
            //Copy byte to output buffer
            *(outBuffer + i - zeroPairs + 1) = *(inBuffer + i);

            nonZeroValueByte++;
        }

        i++;
    }

    //Fill the last encoded byte on the zero byte
    *nonZeroValuePointer = nonZeroValueByte;

    //New encoded length
    return length - zeroPairs + 1;
}

int decodeCOBSZPE(unsigned char* inBuffer, int length, unsigned char* outBuffer)
{
    int i = 1;
    int isZeroPair = 0;
    int zeroPairs = 0;
    unsigned char zeroPosition;
    const unsigned char zeroPairByte = 0XE0;

    if(length > 224)
    {
        //Message is to long to decode with COBS
        return -1;
    }

    //Get first code byte
    zeroPosition = *inBuffer;

    //Check if code is zero pair
    if(zeroPosition > zeroPairByte)
    {
        //Correct zero position
        zeroPosition -= zeroPairByte;

        //Set zero pair flag
        isZeroPair = 1;
    }

    //Decode the whole message
    while(i < length)
    {
        //Check for code byte
        if(i == zeroPosition)
        {
            //Get next zero position
            zeroPosition += *(inBuffer + i);

            //Set code byte to zero
            *(outBuffer + i + zeroPairs - 1) = 0x00;

            //Check the zero pair flag
            if(isZeroPair)
            {
                //Add second zero byte to output buffer
                *(outBuffer + i + zeroPairs) = 0x00;

                //Add to zero pairs counter
                zeroPairs++;
            }

            //Check if next zero position is zero pair
            if(zeroPosition > zeroPairByte)
            {
                //Correct zero position
                zeroPosition -= zeroPairByte;

                //Set zero pair flag
                isZeroPair = 1;
            }
            else
            {
                //Unset zero pair flag
                isZeroPair = 0;
            }
        }
        else
        {
            //Copy byte to output buffer
            *(outBuffer + i + zeroPairs - 1) = *(inBuffer + i);
        }

        i++;
    }

    //New decoded length
    return length + zeroPairs - 1;
}
