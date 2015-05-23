#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    NodeStruct mapLabo[MAPSIZE];
    uint8 startPosition, startSearchPosition, currentPosition, searchPosition, goalPosition;
    uint8 pickerRFChannel;
    uint8 attempts, respons;
    float driveSpeed = 80;
    char* packetQRCode;
    unsigned char messageSendBuffer[223];
    unsigned char messageReceiveBuffer[223];

    //Read arguments
    if(argc < 2)
    {
        printf("Insufficient arguments provided!\n");
        printf("arg 1: RF-channel of picker robot\n");
        printf("arg 2: QR code of searched packet\n");
        printf("Program will terminate now.\n");

        return;
    }

    //Get picker robot RF-channel
    pickerRFChannel = atoi(argv[1]);

    //Get QR-code of searched packet
    packetQRCode = argv[2];

    //Setup motordrivers
    DriveInit();

    //Initialize map
	initializeMap(mapLabo);

	//Initialize RF-module
    if(RfCommsInit(SEEKER_RFCHANNEL) != 0)
    {
        printf("Error while initialising RF-module\n");
        espeak("RF sender error!");

        return;
    }

    //Clear receive buffer
    clearReceiveBuffer();

    printf("Initialisation complete\n");
    espeak("Initialisation complete.");

    /**
        Check if the given pickerbot is available for picking up the package before moving out
        and search the packet.

        (Section needs to be implemented on the pickerbot)
    **/

    //Check if pickerbot is available
    respons = 0;
    attempts = 0;

    char pingMessage[] = "SEEKER PING";

    //Try three times before time-out
    while(respons == 0 && attempts < 3)
    {
        if(sendMessage((unsigned char*)pingMessage, pickerRFChannel) != 0)
        {
            printf("Error, could not send message to pickerbot: %d!\nProgram will terminate now.\n", pickerRFChannel);
            espeak("RF sender error!");

            return;
        }

        //Wait for pickerbot to respond
        _delay_us(2500000);

        //Poll for new message in receive buffer
        RfCommsReceivePoll(&respons);

        //Get and check the received message for ACK
        if(respons)
        {
            if(receiveMessage(messageReceiveBuffer) == 0)
            {
                //Check for acknowledge of the pickerbot
                if(strcmp((char*)messageReceiveBuffer, "PICKER ACK") != 0)
                {
                    respons = 0;
                    attempts++;
                }
            }
            else
            {
                respons = 0;
                attempts++;
            }
        }
        else
        {
            attempts++;
        }
    }

    if(attempts >= 3)
    {
        printf("Pickerbot: %d is not available or is not responding!\nProgram will terminate now.\n", pickerRFChannel);
        espeak("Pickerbot is not available!");

        return;
    }

    /**
        End of section
    **/

    //Seeck for first RF-tag to locate start location
    DriveLineFollow(driveSpeed);

    //Scan-tag
    int result;
    char tagData[24];

    result = TagReaderGetUID(tagData);

    if(result != 0)
    {
        printf("No RF-tag detected! Error code: %d\nProgram will terminalte now.\n", result);
        espeak("RF-tag not found!");

        return;
    }

    //Search locale position
    currentPosition = getNodeFromUID(tagData);

    if(currentPosition == -1)
    {
        printf("Current position is unknown!\nProgram will terminate now.\n");
        espeak("Current position not found!");

        return;
    }

    printf("Current position: %d\n", currentPosition);

    //Search start position
    startPosition = getStartNodeFromCrossingNode(mapLabo, currentPosition);

    if(startPosition == -1)
    {
        printf("Could not determin start position!\nProgram will terminate now.\n");
        espeak("Start position not found!");

        return;
    }

    printf("Start position: %d\n", startPosition);

    //Start search loop
    int found = 0;
    searchPosition = 0;
    startSearchPosition = startPosition;

    while(searchPosition < MAPSIZE && !found)
    {
        //Search for next endpoint where a package could be located (not located on the start position)
        if(nodeIsEndpoint(mapLabo, searchPosition) && searchPosition != startPosition)
        {
            printf("Running dijkstra pathplanning...\n");

            //Find route to next endpoint
            initializeMap(mapLabo);
            Dijkstra(mapLabo, MAPSIZE, startSearchPosition, searchPosition);

            printf("Route to point: %d found\n", searchPosition);

            //Set current position
            currentPosition = mapLabo[startSearchPosition].Next;

            //Travel to the next endpoint
            Travel(mapLabo, MAPSIZE, currentPosition, searchPosition, driveSpeed);

            //Search for QR-code on package if available
            char QRData[64];

            result = QRCodeDecode(QRData,64);

            if(result == 0)
            {
                //Check if package is the searched one
                if(strcmp(QRData, packetQRCode) == 0)
                {
                    printf("Packet: %s found!\n", packetQRCode);
                    found = 1;
                }
                else
                {
                    printf("Other packet detected with tag: %s\n", QRData);
                }
            }
            else if(result == 2)
            {
                printf("No QR-code detected!\n");
            }
            else
            {
                printf("Error using camera! Error code: %d\nProgram will terminate now.\n", result);
                espeak("Camera is malfunctioning!");

                return;
            }

            //Set new location
            startSearchPosition = searchPosition;

            //Make turn and go to the next node
            DriveRotateCenter(180.0, driveSpeed);
            DriveLineFollow(driveSpeed);
        }

        if(found)
        {
            goalPosition = searchPosition;
        }
        else
        {
            searchPosition++;
        }

    }

    if(!found)
    {
        printf("Packet not found on the endpoints!\nProgram will terminate now.\n");
        espeak("Packet not found!");

        return;
    }

    //Make way for pickerbot
    found = 0;

    //Find route to the next free endpoint (different from the package location and the start point)
    while(!found)
    {
        searchPosition++;

        if(searchPosition >= MAPSIZE)
        {
            searchPosition = 0;
        }

        if(nodeIsEndpoint(mapLabo, searchPosition) && searchPosition != startPosition && searchPosition != goalPosition)
        {
            found = 1;
        }
    }

    //Find route the the free endpoint
    initializeMap(mapLabo);
    Dijkstra(mapLabo, MAPSIZE, goalPosition, searchPosition);

    //Go to free endpoint
    Travel(mapLabo, MAPSIZE, mapLabo[goalPosition].Next, searchPosition, driveSpeed);

    //Send the pick up job to the pickerbot
    respons = 0;
    attempts = 0;

    //Command job Format: JOB:[START],[FINISH]
    sprintf((char*)messageSendBuffer, "JOB:%d,%d", startPosition, goalPosition);

    /**
        Check if the given pickerbot has received the pickup job and retry three
        times before time-out when no acknowledgment is received from pickerbot.

        (Section needs to be implemented on the pickerbot for successful termination)
    **/

    //Send three attempts with package location before time-out
    while(respons == 0 && attempts < 3)
    {
        if(sendMessage(messageSendBuffer, pickerRFChannel) != 0)
        {
            printf("Error, could not send message to pickerbot: %d!\nProgram will terminate now.\n", pickerRFChannel);
            espeak("RF sender error!");

            return;
        }

        //Wait for pickerbot to respond
        _delay_us(2500000);

        //Poll for new message in receive buffer
        RfCommsReceivePoll(&respons);

        //Get and check the received message for ACK
        if(respons)
        {
            if(receiveMessage(messageReceiveBuffer) == 0)
            {
                //Check for acknowledge of the pickerbot
                if(strcmp((char*)messageReceiveBuffer, "JOB ACK") != 0)
                {
                    respons = 0;
                    attempts++;
                }
            }
            else
            {
                respons = 0;
                attempts++;
            }
        }
        else
        {
            attempts++;
        }
    }

    if(attempts >= 3)
    {
        printf("Pickerbot: %d is not available or is not responding!\nProgram will terminate now.\n", pickerRFChannel);
        espeak("Pickerbot is not responding!");

        return;
    }

    /**
        End of section
    **/

    printf("Program successfull ended.\n");
    espeak("Program complete. Ready.");
}

int sendMessage(unsigned char* message, int destRFAddress)
{
    RfCommsPacket rfPacket;

    rfPacket.DataLen = strlen((const char*)message);
    rfPacket.DstRfAddr = destRFAddress;
    rfPacket.SrcRfAddr = SEEKER_RFCHANNEL;
    rfPacket.Cmd = 0;
    rfPacket.Lqi = 0;
    rfPacket.Rssi = 0;

    strcpy((char*)rfPacket.Data, (const char*)message);

    if(RfCommsSendPacket(&rfPacket) != 0)
    {
        printf("Error while sending message to address: %d\n", destRFAddress);

        return 1;
    }

    return 0;
}

int receiveMessage(unsigned char* message)
{
    RfCommsPacket rfPacket;

    if(RfCommsReceivePacket(&rfPacket) == 0)
    {
        strcpy((char*)message, (const char*)rfPacket.Data);
    }
    else
    {
        printf("Error while reading new message!\n");

        return 1;
    }

    return 0;
}

int clearReceiveBuffer(void)
{
    uint8 receivePollFailed, receiveStatus;
    RfCommsPacket rfPacket;

    receivePollFailed = RfCommsReceivePoll(&receiveStatus);

    while(!receivePollFailed && receiveStatus)
    {
        //Remove remaining package from receivestack
        if(RfCommsReceivePacket(&rfPacket) != 0)
        {
            receivePollFailed = 1;
        }
        else
        {
            receivePollFailed = RfCommsReceivePoll(&receiveStatus);
        }
    }

    if(receivePollFailed)
    {
        printf("Error while clearing receive buffer!\n");

        return 1;
    }
    else
    {
        return 0;
    }
}

int getNodeFromUID(char* UIDData)
{
    int i = 0;
    int found = 0;

    while(i < MAPSIZE && !found)
    {
        if(strcmp(UIDData, nodeUID[i]) == 0)
        {
            found = 1;
        }
        else
        {
            i++;
        }
    }

    if(i >= MAPSIZE)
    {
        //UID not found
        return -1;
    }
    else
    {
        return i;
    }
}

int getStartNodeFromCrossingNode(NodeStruct* map, int nodeID)
{
    int i = 0;
    int j = 0;
    int neighbours = 0;
    int found = 0;

    while(i < MAPSIZE && !found)
    {
        //Check if node is an endpoint (neighbours == 1)
        neighbours = 0;
        found = 0;

        for(j = 0; j < 4; j++)
        {
            //Check if node has the crossing node as a neighbour
            if(map[i].Neighbours[j] == nodeID)
            {
                found = 1;
                neighbours++;
            }
            else if(map[i].Neighbours[j] != -1)
            {
                neighbours++;
            }
        }

        //Node is not an endpoint
        if(neighbours != 1)
        {
            found = 0;
        }

        //Check the next node on the map
        if(!found)
        {
            i++;
        }
    }

    if(i >= MAPSIZE)
    {
        //No start node found
        return -1;
    }
    else
    {
        return i;
    }
}

int nodeIsEndpoint(NodeStruct* map, int nodeID)
{
    int i;
    int neighbours = 0;

    for(i = 0; i < 4; i++)
    {
        //Check neighbours of node  (-1 = no neighbour in searched direction)
        if(map[nodeID].Neighbours[i] != -1)
        {
            neighbours++;
        }
    }

    if(neighbours == 1)
    {
        //Node is an endpoint (only one neighbour)
        return 1;
    }
    else
    {
        //Node is not an endpoint
        return 0;
    }
}
