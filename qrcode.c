#include <stdio.h>

typedef enum
{
    false, true
} bool;

FILE* camStat;
FILE* zbarStat;

int QRCodeDecode(char *Data,int MaxDataLen)
{
    printf("Start QR code scan...\n");
    system("espeak -ven+f2 -k5 -a50 -s150 \"Scannig package.\" --stdout | aplay");

    //Take image with front camera, save it to file /mnt/QRSnaps/QRSnap.jpg and print output+error stream to file
    system("raspistill -w 640 -h 480 -q 80 -rot 180 -o /mnt/QRSnaps/QRSnap.jpg 2>&1>/mnt/QRSnaps/camStat");

    //Read Raspicam logs
    camStat = fopen("/mnt/QRSnaps/camStat", "r");

    if(camStat == NULL)
    {
        printf("Error while reading camera logs!\n");
        return 3;   //Other error
    }

    //LOGIC TO INTERPRET CAMERALOG

    if(1)
    {
        printf("Scan complete.\nAnalysing...\n");
        system("espeak -ven+f2 -k5 -a50 -s150 \"Scan complete. Analysing.\" --stdout | aplay");
    }
    else
    {
        printf("Error while taking picture!\n");
        system("espeak -ven+f2 -k5 -a50 -s150 \"Error.\" --stdout | aplay");
        return 1;   //Camera error
    }

    fclose(camStat);

    //Analysing taken picture for QRcode and print result to file
    system("zbarimg /mnt/QRSnaps/QRSnap.jpg 2>&1>/mnt/QRSnaps/zbarStat");

    //Read zbar logs
    zbarStat = fopen("/mnt/QRSnaps/zbarStat", "r");

    if(zbarStat == NULL)
    {
        printf("Error while reading zbar logs!\n");
        return 3;   //Other error
    }

    char buffer[128];
    bool found = false;

    while(fgets(buffer, 128, zbarStat) != NULL && !found)
    {
        if(buffer[0] == 'Q')
        {
            int i = 0;
            while(buffer[8 + i] != '\n' && i < MaxDataLen - 1)
            {
                Data[i] = buffer[8 + i];
                i++;
            }
            Data[i + 1] = '\0';
            found = true;
        }
    }

    fclose(zbarStat);

    if(found)
    {
        return 0;   //QRcode OK
    }
    else
    {
        return 2;   //No QRcode detected
    }
}
