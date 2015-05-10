#include "drive.h"

float pi = 3.141592;
float omtrek = 173.0;
float onderling_wiel_afstand = 113.0;
uint16 Kp = 2400;
uint16 Kd = 3800;
uint16 Ki = 0;
uint16 imax = 0;
uint16 whiteLeft = 17670;
uint16 whiteRight = 11350;
uint16 greyLeft = 19220;
uint16 greyRight = 13540;
uint16 blackLeft = 22300;
uint16 blackRight = 16800;

// Initialiseren om te rijden
int DriveInit(void)
{
    uint8 res = 0;

    res = LegoMotorSetup(&LegoMotor,1,0,1);
    if(res != 0)
    {
        printf("DriveInit: Motor setup right failed\n");
        return res;
    }

    res = LegoMotorSetup(&LegoMotor,2,0,1);
    if(res != 0)
    {
        printf("DriveInit: Motor setup left failed\n");
        return res;
    }

    sint16 position = 0;

    res = LegoMotorSetPos(&LegoMotor,1,position);
    if(res != 0)
    {
        printf("DriveInit: Motor set position right failed\n");
        return res;
    }

    res = LegoMotorSetPos(&LegoMotor,2,position);
    if(res != 0)
    {
        printf("DriveInit: Motor set position left failed\n");
        return res;
    }

    res = LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
    if(res != 0)
    {
        printf("DriveInit: Motor PID control right failed\n");
        return res;
    }

    res = LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);
    if(res != 0)
    {
        printf("DriveInit: Motor PID control left failed\n");
        return res;
    }

    printf("Initializing wheels complete\n");
    return res;
}

// Rechtdoor rijden
// Twee foutverhelpers:
// 1. Wanneer verschil tussen beide motoren te groot wordt, stopt wagen (faulttolerance)
void DriveStraightDistance(float Distance, float Speed)
{
    if(Speed > 0)
    {
        float encSteps = 720.0/omtrek * Distance;
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        sint16 currentPosition = 0;
        sint16 positionRight = 0;
        sint16 positionLeft = 0;
        sint16 difference = 0;
        sint16 faultTolerance = 30;      // Fouttolerantie (verschil tussen twee wielen in encoderposities)
        TimeStepInit(10000);

        if(Distance > 0)
        {
            do
            {
                currentPosition = currentPosition + (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPosition);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPosition);

                // Veiligheid
                LegoMotorGetPos(&LegoMotor, 1, &positionRight);
                LegoMotorGetPos(&LegoMotor, 2, &positionLeft);
                difference = abs(positionRight - positionLeft);

                TimeStep(0);
            } while((currentPosition < ((sint16) encSteps)) && (difference < faultTolerance));
        }
        else if(Distance < 0)
        {
            do
            {
                currentPosition = currentPosition - (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPosition);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPosition);

                // Veiligheid
                LegoMotorGetPos(&LegoMotor, 1, &positionRight);
                LegoMotorGetPos(&LegoMotor, 2, &positionLeft);
                difference = abs(positionRight - positionLeft);

                TimeStep(0);
            } while((currentPosition > ((sint16) encSteps)) && (difference < faultTolerance));
        }
        else
        {
            // Stilstaan (Distance == 0)
        }

        LegoMotorSetup(&LegoMotor,1,1,1);
        LegoMotorSetup(&LegoMotor,2,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

// Rechter wiel laten draaien
void DriveRotateRWheel(float Angle,float Speed)
{
    if(Speed > 0)
    {
        float radians = pi/180.0 * Angle;
        float encSteps = 720.0/omtrek * (onderling_wiel_afstand * radians);
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        LegoMotorSetup(&LegoMotor,2,1,1);

        sint16 currentPosition = 0;
        TimeStepInit(10000);

        if(Angle > 0)
        {
            do
            {
                currentPosition = currentPosition + (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPosition);

                TimeStep(0);
            } while((currentPosition < ((sint16) encSteps)));
        }
        else if(Angle < 0)
        {
            do
            {
                currentPosition = currentPosition - (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPosition);

                TimeStep(0);
            } while((currentPosition > ((sint16) encSteps)));
        }
        else
        {
            // Stilstaan (Angle == 0)
        }

        LegoMotorSetup(&LegoMotor,1,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

// Linker wiel laten draaien
void DriveRotateLWheel(float Angle,float Speed)
{
    if(Speed > 0)
    {
        float radians = pi/180.0 * Angle;
        float encSteps = 720.0/omtrek * (onderling_wiel_afstand * radians);
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        LegoMotorSetup(&LegoMotor,1,1,1);

        sint16 currentPosition = 0;
        TimeStepInit(10000);

        if(Angle > 0)
        {
            do
            {
                currentPosition = currentPosition + (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPosition);

                TimeStep(0);
            } while((currentPosition < ((sint16) encSteps)));
        }
        else if(Angle < 0)
        {
            do
            {
                currentPosition = currentPosition - (sint16) encStepsPerTenMS;
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPosition);

                TimeStep(0);
            } while((currentPosition > ((sint16) encSteps)));
        }
        else
        {
            // Stilstaan (Angle == 0)
        }

        LegoMotorSetup(&LegoMotor,2,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

// Wagen laten draaien rond center
void DriveRotateCenter(float Angle,float Speed)
{
    if(Speed > 0)
    {
        float radians = pi/180.0 * Angle;
        float encSteps = 720.0/omtrek * (onderling_wiel_afstand/2.0 * radians);
        float encStepsPos = fabs(encSteps);
        float encStepsNeg = encStepsPos*(-1.0);
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        sint16 currentPositionR = 0;
        sint16 currentPositionL = 0;
        TimeStepInit(10000);

        if(Angle > 0)
        {
            do
            {
                currentPositionL = currentPositionL - (sint16) encStepsPerTenMS;
                currentPositionR = currentPositionR + (sint16) encStepsPerTenMS;

                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionL);
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionR);

                TimeStep(0);
            } while((currentPositionL > ((sint16) encStepsNeg)) || (currentPositionR < ((sint16) encStepsPos)));
        }
        else if(Angle < 0)
        {
            do
            {
                currentPositionL = currentPositionL + (sint16) encStepsPerTenMS;
                currentPositionR = currentPositionR - (sint16) encStepsPerTenMS;

                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionL);
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionR);

                TimeStep(0);
            } while((currentPositionL < ((sint16) encStepsPos)) || (currentPositionR > ((sint16) encStepsNeg)));
        }
        else
        {
            // Stilstaan (Angle == 0)
        }

        LegoMotorSetup(&LegoMotor,1,1,1);
        LegoMotorSetup(&LegoMotor,2,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

//De wagen een lijn laten volgen over een bepaalde afstand
void DriveLineFollowDistance(int Distance, float Speed)
{
    uint16 manualCorrection = 6240;

    if(Speed > 0)
    {
        float encSteps = 720.0/omtrek * Distance;
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        LegoSensorSetupLSensor(&LegoSensor, 1, 1);
        LegoSensorSetupLSensor(&LegoSensor, 2, 1);

        sint16 currentPosition = 0;
        sint16 currentPositionLcorrection = 0;
        sint16 currentPositionRcorrection = 0;
        uint16 intensityL = 0;
        uint16 intensityR = 0;
        sint16 difference = 0;
        uint16 tolerace = 500;
        TimeStepInit(10000);

        while(currentPosition < encSteps)
        {
            LegoSensorGetLSensorData(&LegoSensor, 1, &intensityR);
            LegoSensorGetLSensorData(&LegoSensor, 2, &intensityL);
            intensityR = intensityR + manualCorrection;
            difference = intensityR - intensityL;

            currentPosition = currentPosition + (sint16) encStepsPerTenMS;

            if(difference > tolerace)                            //Robot links van de lijn
            {
                currentPositionRcorrection = currentPositionRcorrection + (sint16) encStepsPerTenMS;
                LegoMotorSetup(&LegoMotor,1,1,1);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionRcorrection);
            }
            else if(difference < ((-1)*tolerace))               //Robot rechts van de lijn
            {
                currentPositionLcorrection = currentPositionLcorrection + (sint16) encStepsPerTenMS;
                LegoMotorSetup(&LegoMotor,2,1,1);
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionLcorrection);
            }
            else
            {
                currentPositionLcorrection = currentPositionLcorrection + (sint16) encStepsPerTenMS;
                currentPositionRcorrection = currentPositionRcorrection + (sint16) encStepsPerTenMS;

                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionLcorrection);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionRcorrection);
            }
            TimeStep(0);
        }

        LegoMotorSetup(&LegoMotor,1,1,1);
        LegoMotorSetup(&LegoMotor,2,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

// De wagen voor onbepaalde afstand de lijn laten volgen
void DriveLineFollow(float Speed)
{
    uint16 manualCorrection = 6240;

    if(Speed > 0)
    {
        float encStepsPerSec = 720.0/omtrek * Speed;
        float encStepsPerTenMS = encStepsPerSec/100.0;

        LegoSensorSetupLSensor(&LegoSensor, 1, 1);
        LegoSensorSetupLSensor(&LegoSensor, 2, 1);

        sint16 currentPosition = 0;
        sint16 currentPositionLcorrection = 0;
        sint16 currentPositionRcorrection = 0;

        uint16 intensityL = 0;
        uint16 intensityR = 0;
        sint16 difference = 0;

        uint16 whiteRightFixed = 12000;
        uint16 whiteLeftFixed = 18000;
        int debounce = 5;
        int counter = 0;
        int run = 1;

        uint16 tolerance = 500;

        TimeStepInit(10000);

        LegoSensorGetLSensorData(&LegoSensor, 1, &intensityR);
        LegoSensorGetLSensorData(&LegoSensor, 2, &intensityL);

        while(run == 1)
        {
            LegoSensorGetLSensorData(&LegoSensor, 1, &intensityR);
            LegoSensorGetLSensorData(&LegoSensor, 2, &intensityL);
            intensityR = intensityR + manualCorrection;
            difference = intensityR - intensityL;

            currentPosition = currentPosition + (sint16) encStepsPerTenMS;

            if(difference > tolerance)                           // Robot links van de lijn
            {
                currentPositionRcorrection = currentPositionRcorrection + (sint16) encStepsPerTenMS;
                LegoMotorSetup(&LegoMotor,1,1,1);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionRcorrection);
            }
            else if(difference < ((-1)*tolerance))               // Robot rechts van de lijn
            {
                currentPositionLcorrection = currentPositionLcorrection + (sint16) encStepsPerTenMS;
                LegoMotorSetup(&LegoMotor,2,1,1);
                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionLcorrection);
            }
            else
            {
                currentPositionLcorrection = currentPositionLcorrection + (sint16) encStepsPerTenMS;
                currentPositionRcorrection = currentPositionRcorrection + (sint16) encStepsPerTenMS;

                LegoMotorPosPIDControl(&LegoMotor, 1, currentPositionLcorrection);
                LegoMotorPosPIDControl(&LegoMotor, 2, currentPositionRcorrection);
            }
            TimeStep(0);

            if((intensityR < (whiteRightFixed + manualCorrection)) && (intensityL < whiteLeftFixed))
            {
                counter = counter + 1;
                if(counter == debounce)
                {
                    run = 0;
                }
            }
            else
            {
                counter = 0;
            }
        }

        LegoMotorSetup(&LegoMotor,1,1,1);
        LegoMotorSetup(&LegoMotor,2,1,1);

        LegoMotorSetPos(&LegoMotor,1,0);
        LegoMotorSetPos(&LegoMotor,2,0);

        LegoMotorPIDControl(&LegoMotor,1,0,Kp,Kd,Ki,imax);
        LegoMotorPIDControl(&LegoMotor,2,0,Kp,Kd,Ki,imax);

        LegoMotorSetup(&LegoMotor,1,0,1);
        LegoMotorSetup(&LegoMotor,2,0,1);
    }
    else
    {
        printf("Specified speed is not allowed\n");
    }
}

