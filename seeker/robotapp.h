#ifndef ROBOT_APP
#define ROBOT_APP

#include <stdlib.h>

#include "project.h"
#include "rs485client.h"
#include "lego-motor.h"
#include "rf-cc1101.h"
#include "lin-lego-motor-log.h"
#include "lego-sensor.h"
#include "lin-delay.h"
#include "TimeSupport.h"
#include "qrcode.h"
#include "tagreader.h"
#include "dijkstra.h"
#include "map.h"
#include "drive.h"
#include "speaker.h"
#include "rfcomms.h"
#include "travel.h"

extern RS485ClientStruct RS485Client;
extern LegoMotorStruct LegoMotor;
extern LegoSensorStruct LegoSensor;
extern RfCC1101Struct RfCC1101;

/**
 * \brief The seeker robot application
 * \arg 1: Picker robot RF-channel
 * \arg 2: QR-code of wanted packet
**/
void RobotApp(int argc, char *argv[]);

/**
 * \brief Send data packet with the RF-CC1101
 * \param message :Pointer to the message string to be sended
 * \param destRFAddress :Destination RF-channel to address to
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int sendMessage(unsigned char* message, int destRFAddress);

/**
 * \brief Get data packet from the receivestack of the RF-CC1101 if available (Check with RfCommsReceivePoll first)
 * \param message :Pointer where to write the received message to
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int receiveMessage(unsigned char* message);

/**
 * \brief Clear the receivestack of the RF-CC1101
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int clearReceiveBuffer(void);

/**
 * \brief Get the node index of the map corresponding to the tag-UID
 * \param UIDData :Pointer to the UID of the scanned tag
 * \return
 * -1: No node corrispond with the given UID \n
 * else: The index of the node that corrisponds with the UID \n
*/
int getNodeFromUID(char* UIDData);

/**
 * \brief Get the start node (endpoint) determined from the given cross node
 * \param map :Pointer to the nodestruct of the map
 * \param nodeID :Index of the cross node where to calculate the start node from
 * \return
 * -1: No start node found with the given cross node \n
 * else: The index of the node that is the start node (endpoint) corrisponding with the given cross node \n
*/
int getStartNodeFromCrossingNode(NodeStruct* map, int nodeID);

/**
 * \brief Determine if given node is an endpoint
 * \param map :Pointer to the nodestruct of the map
 * \param nodeID :Index of the node to check
 * \return
 * 0: Is not an endpoint on the map \n
 * 1: Is an endpoint on the map \n
*/
int nodeIsEndpoint(NodeStruct* map, int nodeID);

#endif
