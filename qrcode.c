#include <stdio.h>

typedef enum
{
    false, true
} bool;

int QRCodeDecode(char *Data,int MaxDataLen)
{
    system ("espeak -ven+f2 -k5 -a50 -s150 \"Say cheese.\" --stdout | aplay");
    //Take image with front camera and save it to file /mnt/QRSnaps/QRSnap.jpg
    if(!system("raspistill -w 640 -h 480 -q 80 -rot 180 -o /mnt/QRSnaps/QRSnap.jpg"))
    {
        system ("espeak -ven+f2 -k5 -a50 -s150 \"Click click.\" --stdout | aplay");
    }
    else
    {
        system ("espeak -ven+f2 -k5 -a50 -s150 \"Error, no smile detected.\" --stdout | aplay");
    }

    FILE* pipe = popen("zbarimg /mnt/QRSnaps/QRSnap.jpg", "r");
    if(!pipe)
    {
        return 3;
    }
    char buffer[128];
    char readData[MaxDataLen];
    bool ready = false;

    while(!feof(pipe) || !ready)
    {
        if(fgets(buffer, 128, pipe) != NULL)
        {
            if(buffer[0] == 'Q')
            {
                int i = 0;
                while(buffer[8 + i] != '\n' && i < MaxDataLen - 1)
                {
                    readData[i] = buffer[8 + i];
                    ++i;
                }
                readData[i + 1] = '\0';
                ready = true;
            }
        }
    }

    pclose(pipe);

    if(ready)
    {
        Data = readData;
        return 0;
    }
    else
    {
        return 2;
    }
}
