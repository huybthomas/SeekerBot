#include "tagreader.h"

FILE* scriptor;

int TagReaderGetUID(char *Data)
{
    char buffer[128];

    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;   //Other error
    }

    printf("Start RF-tag UID scan...\n");

    //Pipe the read tag info command to scriptor, drop the error stream to null device
    scriptor = popen("echo \"FF CA 00 00 00\" | scriptor 2>&1", "r");
    if(!scriptor)
    {
        printf("Error, cannot open scriptor\n");
        return 3;   //Other error
    }

    char* token;
    bool found = false;

    while(fgets(buffer, 128, scriptor) != NULL && !found)
    {
        //Check for "Can't get readers list"
        if(strstr(buffer, "Can't get readers list") != NULL)
        {
            //Reader is not connected
            printf("Error, connot contact RF-tag tag reader\n");
            return 1;   //Tag reader error
        }
        else
        {
            //Get the UID number
            token = strrchr(buffer, '<');

            if(token != NULL)
            {
                strncpy(Data, token + 2, 20);   //Copy UID
                Data[20] = '\0';    //String terminator
                found = true;
            }
        }
    }

    pclose(scriptor);

    if(found)
    {
        return 0;   //Tag OK
    }
    else
    {
        return 2;   //No tag detected
    }
}

int TagReaderWriteData(int BlockNumber, int DataLength, char* Data)
{
    char buffer[128];
    char command[40];

    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;
    }

    printf("Write data to RF-tag...\n");

    //Create scriptor command
    //Byte 4: Block number, Byte 5: Data length
    sprintf(command, "echo \"FF D6 00 %02X %02X %s\" | scriptor 2>&1", BlockNumber, DataLength, Data);

    //Pipe the read tag info command to scriptor, drop the error stream to null device
    scriptor = popen(command, "r");
    if(!scriptor)
    {
        printf("Error, cannot open scriptor\n");
        return 3;   //Other error
    }

    bool found = false;

    while(fgets(buffer, 128, scriptor) != NULL && !found)
    {
        //Check for "Can't get readers list"
        if(strstr(buffer, "Can't get readers list") != NULL)
        {
            //Reader is not connected
            printf("Error, connot contact RF-tag tag reader\n");
            return 1;   //Tag reader error
        }
        else
        {
            //Check for "Normal processing"
            if(strstr(buffer, "Normal processing."))
            {
                found = true;
            }
        }
    }

    pclose(scriptor);

    if(found)
    {
        return 0;   //Tag OK
    }
    else
    {
        return 2;   //No tag detected
    }
}

int TagReaderReadData(int BlockNumber, int DataLength, char* Data)
{
    char buffer[128];
    char command[40];

    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;   //Other error
    }

    printf("Start RF-tag data scan...\n");

    //Create scriptor command
    //Byte 4: Block number, Byte 5: Data length
    sprintf(command, "echo \"FF B0 00 %02X %02X\" | scriptor 2>&1", BlockNumber, DataLength);

    //Pipe the read tag info command to scriptor, drop the error stream to null device
    scriptor = popen(command, "r");
    if(!scriptor)
    {
        printf("Error, cannot open scriptor\n");
        return 3;   //Other error
    }

    char* token;
    bool found = false;

    while(fgets(buffer, 128, scriptor) != NULL && !found)
    {
        //Check for "Can't get readers list"
        if(strstr(buffer, "Can't get readers list") != NULL)
        {
            //Reader is not connected
            printf("Error, connot contact RF-tag tag reader\n");
            return 1;   //Tag reader error
        }
        else
        {
            //Get the data
            token = strrchr(buffer, '<');

            if(token != NULL)
            {
                strncpy(Data, token + 2, (DataLength*3) - 1);   //Copy UID
                Data[(DataLength*3) - 1] = '\0';    //String terminator
                found = true;
            }
        }
    }

    pclose(scriptor);

    if(found)
    {
        return 0;   //Tag OK
    }
    else
    {
        return 2;   //No tag detected
    }
}
