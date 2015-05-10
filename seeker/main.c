//Robotics lab
//Drive test program

#include "project.h"
#include "rs485client.h"
#include "lego-motor.h"
#include "lego-sensor.h"
#include "rf-cc1101.h"
#include "lin-delay.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include "robotapp.h"

RS485ClientStruct RS485Client;
LegoMotorStruct LegoMotor;
LegoSensorStruct LegoSensor;
RfCC1101Struct RfCC1101;

/* Signal Handler for SIGINT */
void sigintHandler(int sig_num)
{
    /* Reset handler to catch SIGINT next time.
       Refer http://en.cppreference.com/w/c/program/signal */
    signal(SIGINT, sigintHandler);

    printf("\nABORT. Shutting down motors.\n");

    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);
    LegoMotorSetup(&LegoMotor,3,0,0);
    LegoMotorDirectControl(&LegoMotor,1,0);
    LegoMotorDirectControl(&LegoMotor,2,0);
    LegoMotorDirectControl(&LegoMotor,3,0);

    printf ("ABORT complete\n");

    fflush(stdout);
    exit (-1);
}

void LogCsvSint16(char *FileName,sint16 *Data,uint16 Num);

int main(int argc, char *argv[])
{
    int res;

    //Initialize RS485Client
    strcpy(RS485Client.RS485Port,SER_PORT);
    RS485Client.BaudRate=512000;
    RS485Client.Timeout=20E6;
    res=RS485ClientInit(&RS485Client);
    if (res!=0)
    {
        printf ("RS485ClientInit error code %d\n",res);
        return 1;
    }

    //Lego motor init
    LegoMotor.RS485Client=&RS485Client;
    LegoMotor.Address=1;
    res=LegoMotorInit(&LegoMotor);
    if (res!=0)
    {
        printf ("LegoMotorInit error code %d\n",res);
        return 1;
    }

    //Lego sensor init
    LegoSensor.RS485Client=&RS485Client;
    LegoSensor.Address=2;
    res=LegoSensorInit(&LegoSensor);
    if (res!=0)
    {
        printf ("LegoSensorInit error code %d\n",res);
        return 1;
    }
    res=LegoSensorSetup(&LegoSensor,1,CFG_LSENSOR);
    if (res!=0)
    {
        printf ("LegoSensorSetup for CH1 error code %d\n",res);
        return 1;
    }
    res=LegoSensorSetup(&LegoSensor,2,CFG_LSENSOR);
    if (res!=0)
    {
        printf ("LegoSensorSetup for CH2 error code %d\n",res);
        return 1;
    }
    res=LegoSensorSetup(&LegoSensor,3,CFG_SWITCH);
    if (res!=0)
    {
        printf ("LegoSensorSetup for CH3 error code %d\n",res);
        return 1;
    }

    //RF-CC1101 module init
    RfCC1101.RS485Client=&RS485Client;
    RfCC1101.Address=4;
    RfCC1101.Channel=1;
    RfCC1101.Frequency=433920000;
    RfCC1101.ModType=CC1101_MOD_FSK;
    RfCC1101.Rate=CC1101_RATE_1200;
    res=RfCC1101Init(&RfCC1101);
    if (res!=0)
    {
        printf ("RfCC1101Init error code %d\n",res);
        return 1;
    }

    //CTRL-C handler
    signal(SIGINT, sigintHandler);

    //Mount ramdisk to cache picture file
    system("sudo mkdir /mnt/QRSnaps");
    system("sudo mount -o size=500k -t tmpfs none /mnt/QRSnaps");

    printf ("Init complete\n");

    //Main application code
    RobotApp(argc,argv);

    //Unmount ramdisk
    system("sudo umount /mnt/QRSnaps");
    system("sudo rmdir /mnt/QRSnaps");

    //Terminate RS485Client
    res=RS485ClientDeinit(&RS485Client);
    if (res!=0)
    {
        printf ("RS485ClientDeinit error code %d\n",res);
        return 1;
    }

    return 0;
}
