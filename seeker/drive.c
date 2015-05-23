/**
    Code developed by Quinten Van Hasselt & Timothy Verstraete
**/

#include "drive.h"
#include "math.h"
#include "timestep.h"

static float const MMPD = 0.46;     // mm per degree
static float const MMRD = 2;        // mm per degree to rotate
static uint16 const KP = 1800;      // first value was 2100, this one seems more stable
static uint16 const KD = 4200;      // first value was 5000, this one seems more stable
static uint16 const KI = 0;
static uint16 const IMAX = 0;
static int const MOTOR_R = 1;       // motor right
static int const MOTOR_L = 2;       // motor left
static int const SENSOR_R = 1;      // sensor right
static int const SENSOR_L = 2;      // sensor left
int cal;                            // calibration difference value
uint16 iLcal;                       // left sensor calibrated value on drive stroke
uint16 iRcal;                       // right sensor calibrated value on drive stroke


/**
 * \brief Initialize drive system
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int DriveInit(void)
{
    int error = 0;

    //Setup drive motors
    error += LegoMotorSetup(&LegoMotor,MOTOR_R,0,0);
    error += LegoMotorSetup(&LegoMotor,MOTOR_L,0,0);
    error += LegoMotorDirectControl(&LegoMotor, MOTOR_R, 0);
    error += LegoMotorDirectControl(&LegoMotor, MOTOR_L, 0);
    error += LegoMotorPosPIDControl(&LegoMotor, MOTOR_R, 0);
    error += LegoMotorPosPIDControl(&LegoMotor, MOTOR_L, 0);
    error += LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);
    error += LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);

    // Setup reflection sensors
    error += LegoSensorSetup(&LegoSensor, SENSOR_R, CFG_LSENSOR);
    error += LegoSensorSetup(&LegoSensor, SENSOR_L, CFG_LSENSOR);
    error += LegoSensorSetupLSensor(&LegoSensor, SENSOR_R, 1);
    error += LegoSensorSetupLSensor(&LegoSensor, SENSOR_L, 1);

    // calibrate
    cal = calibrate();
    if(error == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}


/**
 * \brief Drive robot straight forward over specified distance with specified speed. Returns after drive is complete.
 * \param Distance :Distance to travel in mm
 * \param Speed :Speed in mm/s
*/

// eventuele verbetering : wachten op elkaar met verhogen van setpoint zodat de wielen meer gelijktijdig rijden
void DriveStraightDistance(float Distance,float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    float totalangle = Distance/MMPD;               // total turning angle for the wheels
    float rotspeed = (fabs(Speed)/100)/MMPD;        // Speed /s --> /10ms is dividing by 100  == [degrees / 10 ms]
    float curangle = 0;

    // put in PID mode
    LegoMotorSetup(&LegoMotor, MOTOR_R, 1, 1);
    LegoMotorSetup(&LegoMotor, MOTOR_L, 1, 1);

    // reset angular motor position
    LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);
    LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    if(totalangle > 0)
    {
        // forward
        while(curangle < totalangle)
        {
            curangle += rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, curangle*2, KP, KD, KI, IMAX);  // KI and IMAX are 0
            TimeStep(0);
        }
    }
    else if(totalangle < 0)
    {
        // backward
        while(curangle > totalangle)
        {
            curangle -= rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, curangle*2, KP, KD, KI, IMAX);  // KI and IMAX are 0
            TimeStep(0);
        }
    }
}


/**
 * \brief Rotate robot around right wheel over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/

void DriveRotateRWheel(float Angle,float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    float rotspeed = (fabs(Speed)/100)/MMPD;    // Speed /s --> /10ms is dividing by 100  == [degrees / 10 ms]
    float totalangle = Angle*MMRD/MMPD;         // total turning distance for wheel
    float curangle = 0;

    // enable R/disable L
    LegoMotorSetup(&LegoMotor,MOTOR_R,1,1);
    LegoMotorSetup(&LegoMotor,MOTOR_L,0,0);

    // reset angular motor position right wheel
    LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    if(totalangle > 0)
    {
        // forward
        while(curangle < totalangle)
        {
            curangle += rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            TimeStep(0);
        }
    }
    else if(totalangle < 0)
    {
        // backward
        while(curangle > totalangle)
        {
            curangle -= rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            TimeStep(0);
        }
    }
}


/**
 * \brief Rotate robot around left wheel over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/

void DriveRotateLWheel(float Angle,float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    float rotspeed = (fabs(Speed)/100)/MMPD;    // Speed /s --> /10ms is dividing by 100  == [degrees / 10 ms]
    float totalangle = Angle*MMRD/MMPD;         // total turning distance for wheel
    float curangle = 0;

    // enable L/disable R
    LegoMotorSetup(&LegoMotor,MOTOR_L,1,1);
    LegoMotorSetup(&LegoMotor,MOTOR_R,0,0);

    // reset motor angular pos L wheel
    LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    if(totalangle > 0)
    {
        // forward
        while(curangle < totalangle)
        {
            curangle += rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            TimeStep(0);
        }
    }
    else if(totalangle < 0)
    {
        // backward
        while(curangle > totalangle)
        {
            curangle -= rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
            TimeStep(0);
        }
    }
}


/**
 * \brief Rotate robot around center of wheelbase over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/
void DriveRotateCenter(float Angle,float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    float rotspeed = (fabs(Speed)/100)/MMPD;    // Speed /s --> /10ms is dividing by 100  == [degrees / 10 ms]
    float totalangle = Angle*MMRD/MMPD;         // total turning distance for wheel
    float curangle = 0;

    // enable R and L motor
    LegoMotorSetup(&LegoMotor,MOTOR_L,1,1);
    LegoMotorSetup(&LegoMotor,MOTOR_R,1,1);

    // reset pos of wheels
    LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);
    LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    if(totalangle > 0)
    {
        while(curangle < totalangle)
        {
            curangle += rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle, KP, KD, KI, IMAX);
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, -curangle, KP, KD, KI, IMAX);
            TimeStep(0);
        }
    }
    else if(totalangle < 0)
    {
        while(curangle > totalangle)
        {
            curangle -= rotspeed;
            LegoMotorPIDControl(&LegoMotor,MOTOR_L, curangle, KP, KD, KI, IMAX);
            LegoMotorPIDControl(&LegoMotor,MOTOR_R, -curangle, KP, KD, KI, IMAX);
            TimeStep(0);
        }
    }
}


/**
 * \brief Get 300 sensor samples and calculate the average for each sensor as calibration.
 * \return The average difference between the 2 sensors
*/

int calibrate()
{
    // Delay of 50ms to stabilize after sensor setup in Init function
    _delay_ms(50);

    uint16 iL = 0;      // variable to get left sensor intensity
    uint16 iR = 0;      // variable to get right sensor intensity
    int arr[300];
    int iLarr[300];
    int iRarr[300];
    int i;

    // Get 300 intensities
    for(i = 0; i < 300; i++)
    {
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_L, &iL);
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_R, &iR);
        iLarr[i] = iL;
        iRarr[i] = iR;
        arr[i] = iL-iR;
    }

    int som = 0;
    int somIL = 0;
    int somIR = 0;
    for(i = 0; i < 300; i++)
    {
        som += arr[i];
        somIL += iLarr[i];
        somIR += iRarr[i];
    }
    iLcal = somIL/300;
    iRcal = somIR/300;
    return som/300;
}


/**
 * \brief Follow line until specified distance has been travelled
 * \param Distance :Distance in mm
 * \param Speed :Speed in mm/s
*/

void DriveLineFollowDistance(int Distance, float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    // init motors
    // put in PID mode
    LegoMotorSetup(&LegoMotor,MOTOR_R,1,1);
    LegoMotorSetup(&LegoMotor,MOTOR_L,1,1);

    // reset angular motor position
    LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);
    LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);

    // variables
    int totaldist = 0;
    uint16 iL = 0;
    uint16 iR = 0;
    int diff = 0;
    int delta = 0;
    float speedL = 0;
    float speedR = 0;
    float KPlight = 0.01;
    float incL = 0;
    float incR = 0;
    float nextL = 0;
    float nextR = 0;
    float rotSpeed = Speed/100/MMPD;

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    // while distance, nu negeren
    while(totaldist < Distance)
    {
        // Twee aparte angles ( eentje per wiel )
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_L, &iL);
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_R, &iR);
        diff = iL - iR - cal;

        delta = diff * KPlight;

        // calc speed for both wheels
        if((diff > 1000) || (diff < -1000))
        {
            speedL = Speed * ( 1 - delta );
            speedR = Speed * ( 1 + delta );
        }
        else
        {
            speedL = Speed;
            speedR = Speed;
        }

        // Check if increase angle is above/under max/min
        incL = speedL/100/MMPD;
        if(incL < 0)
        {
            incL = 0;
        }
        else if (incL > rotSpeed)
        {
            incL = rotSpeed;
        }

        // Check if increase angle is above/under max/min
        incR = speedR/100/MMPD;
        if(incR < 0)
        {
            incR = 0;
        }
        else if (incR > rotSpeed)
        {
            incR = rotSpeed;
        }

        nextL += incL;
        nextR += incR;

        LegoMotorPIDControl(&LegoMotor,MOTOR_L, (nextL)*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
        LegoMotorPIDControl(&LegoMotor,MOTOR_R, (nextR)*2, KP, KD, KI, IMAX);  // KI and IMAX are 0
        TimeStep(0);

        totaldist = (nextL+nextR)*MMPD /2;
    }
}


/**
 * \brief Follow line until end of line segment
 * \param Speed :Speed in mm/s
*/

void DriveLineFollow(float Speed)
{
    // Delay of 50ms to stabilize sensors and motors
    _delay_ms(50);

    // Safety mechanism: speed limited to maximum 100 mm/s.
    if(Speed > 100)
    {
        Speed = 100;
    }

    // put in PID mode
    LegoMotorSetup(&LegoMotor,MOTOR_R,1,1);
    LegoMotorSetup(&LegoMotor,MOTOR_L,1,1);

    // reset pos
    LegoMotorSetPos(&LegoMotor, MOTOR_R, 0);
    LegoMotorSetPos(&LegoMotor, MOTOR_L, 0);

    // variables
    uint16 iL = 0;
    uint16 iR = 0;
    int diff = 0;
    int delta = 0;
    float speedL = 0;
    float speedR = 0;
    float KPlight = 0.01;
    float incL = 0;
    float incR = 0;
    float nextL = 0;
    float nextR = 0;
    float rotSpeed = Speed/100/MMPD;
    int stop = 0;

    // Setup timestep module with time step of 10ms
    TimeStepInit(10000);

    // while distance, nu negeren
    while(stop == 0)
    {
        // Twee aparte angles ( eentje per wiel )
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_L, &iL);
        LegoSensorGetLSensorData(&LegoSensor, SENSOR_R, &iR);
        diff = iL - iR - cal;

        delta = diff * KPlight;

        // calc speed for both wheels
        if((diff > 1000) || (diff < -1000))
        {
            speedL = Speed * ( 1 - delta );
            speedR = Speed * ( 1 + delta );
        }
        else
        {
            speedL = Speed;
            speedR = Speed;
        }

        // Check if increase angle is above/under max/min
        incL = speedL/100/MMPD;
        if(incL < 0)
        {
            incL = 0;
        }
        else if (incL > rotSpeed)
        {
            incL = rotSpeed;
        }

        // Check if increase angle is above/under max/min
        incR = speedR/100/MMPD;
        if(incR < 0)
        {
            incR = 0;
        }
        else if (incR > rotSpeed)
        {
            incR = rotSpeed;
        }

        nextL += incL;
        nextR += incR;

        LegoMotorPIDControl(&LegoMotor,MOTOR_L, (nextL)*2, KP, KD, KI, IMAX);  // *2 because PID controller works with half degrees
        LegoMotorPIDControl(&LegoMotor,MOTOR_R, (nextR)*2, KP, KD, KI, IMAX);  // KI and IMAX are 0
        TimeStep(0);

        // 8% daling in intensiteit om wit te detecteren
        if((iL < (iLcal*0.92)) && (iR < (iRcal*0.92)))
        {
            stop = 1;
        }
    }
}
