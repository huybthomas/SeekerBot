#ifndef ROBOT_APP
#define ROBOT_APP

#include "project.h"
#include "rs485client.h"
#include "lego-motor.h"
#include "rf-cc1101.h"
#include "lin-lego-motor-log.h"
#include "lego-sensor.h"
#include "lin-delay.h"
#include "TimeSupport.h"
#include <stdlib.h>


extern RS485ClientStruct RS485Client;
extern LegoMotorStruct LegoMotor;
extern LegoSensorStruct LegoSensor;
extern RfCC1101Struct RfCC1101;

void RobotApp(int argc, char *argv[]);


#endif
