#include "qrcode.h"

FILE* camStat;
FILE* zbarStat;

int QRCodeDecode(char *Data,int MaxDataLen)
{
    char buffer[128];

    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;   //Other error
    }

    printf("Start QR code scan...\n");

    //Take image with front camera, save it to file /mnt/QRSnaps/QRSnap.jpg and print output+error stream to file
    system("sudo raspistill -w 640 -h 480 -q 80 -rot 180 -o /mnt/QRSnaps/QRSnap.jpg > /mnt/QRSnaps/camStat 2>&1");

    //Read Raspicam logs
    camStat = fopen("/mnt/QRSnaps/camStat", "r");

    if(camStat == NULL)
    {
        printf("Error while reading camera logs!\n");
        return 3;   //Other error
    }

    if(fgets(buffer, 128, camStat) == NULL)
    {
        printf("Scan complete.\nAnalysing...\n");
    }
    else
    {
        printf("Error while taking picture!\n");
        return 1;   //Camera error
    }

    //Close Raspicam logs
    fclose(camStat);

    //Analysing taken picture for QRcode and print result to file
    system("zbarimg /mnt/QRSnaps/QRSnap.jpg > /mnt/QRSnaps/zbarStat 2>&1");

    //Read zbar logs
    zbarStat = fopen("/mnt/QRSnaps/zbarStat", "r");

    if(zbarStat == NULL)
    {
        printf("Error while reading zbar logs!\n");
        return 3;   //Other error
    }

    char* token;
    int QRSize = 0;
    bool found = false;

    while(fgets(buffer, 128, zbarStat) != NULL && !found)
    {
        //Check logs for "QR-Code:"
        if(strstr(buffer, "QR-Code:") != NULL)
        {
            //Get the QR-code
            token = strrchr(buffer, ':');

            if(token != NULL)
            {
                QRSize = strlen(token + 1);     //Get string length without '\0'
                if(QRSize + 1 < MaxDataLen)
                {
                    strcpy(Data, token + 1);
                    Data[QRSize - 1] = '\0';    //String terminator
                }
                else
                {
                    strncpy(Data, token + 1, MaxDataLen - 1);
                    Data[MaxDataLen - 1] = '\0';	//String terminator
                }
                found = true;
            }
        }
    }

    //Close zbar logs
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

/**
    Uitbreiding 1: De QR-code met '*' heeft een ingebouwde error correction: Reed-Solomon. Hiermee kan de QR-code scanner de QR-code interpreteren indien
    er een deel van de code is bedekt. De QR-code zonder '*' moet volledig zichtbaar zijn om deze correct te kunnen interpreteren.
**/
