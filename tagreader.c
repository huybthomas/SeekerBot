#include <stdio.h>
#include <string.h>

typedef enum
{
    false, true
} bool;

FILE* scriptor;

int TagReaderGetUID(char *Data)
{
    char buffer[128];

    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;   //Other error
    }

    printf("Start RF-tag scan...\n");
    system("espeak -ven+f2 -k5 -a50 -s150 \"Scanning tag.\" --stdout | aplay 2>/dev/null");

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

int TagReaderWriteUID(char *Data)
{
    if(Data == NULL)
    {
        printf("Error, Data is NULL.\n");
        return 3;
    }

    printf("Write UID to RF-tag...\n");
    system("espeak -ven+f2 -k5 -a50 -s150 \"Writing tag.\" --stdout | aplay 2>dev/null");

    //Pipe the read tag info command to scriptor, drop the error stream to null device
    scriptor = popen("echo \"FF CA 00 00 00\" | scriptor 2>&1", "r");
    if(!scriptor)
    {
        printf("Error, cannot open scriptor\n");
        return 3;   //Other error
    }
/*
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
    }*/
}

