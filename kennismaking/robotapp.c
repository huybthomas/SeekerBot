#include "robotapp.h"

void RobotApp(int argc, char *argv[])
{
    //Set motors on channels 1 and 2 on 'Direct control mode' without braking on stop
    LegoMotorSetup(&LegoMotor,1,0,0);
    LegoMotorSetup(&LegoMotor,2,0,0);

    //Print on debug terminal
    printf ("Initialisation complete\n");

    //eSpeak voice say a text
    system ("espeak -ven+f2 -k5 -a50 -s150 \"Hello my name is bay max, your personal healthcare companion.\" --stdout | aplay 2>/dev/null");

    //Let motors on channels 1 and 2 turn with a PWM duty cycle of 61% (32767 ==> 100%)
    LegoMotorDirectControl(&LegoMotor,1,20000);
    LegoMotorDirectControl(&LegoMotor,2,20000);

    //Pause code execution for 4 seconds, motors keeps turning
    _delay_ms(4000);

    //Let motor on channel 1 turn backwards with a PWM duty cycle of 92%
    //Stop motor on channel 2 without braking
    LegoMotorDirectControl(&LegoMotor,1,-30000);
    LegoMotorDirectControl(&LegoMotor,2,0);

    //Pause for 2 seconds, motor 1 keeps turning
    _delay_ms(2000);

    //Let motor on channel 1 turn forward with a PWM duty cycle of 92%
    LegoMotorDirectControl(&LegoMotor,1,30000);

    //Pause for 2 seconds, motor 1 keeps turning
    _delay_ms(2000);

    //Stop motor on channel 2 wihtout braking
    LegoMotorDirectControl(&LegoMotor,1,0);

    //eSpeak voice say a text
    system ("espeak -ven+f2 -k5 -a50 -s150 \"I can not deactivate until you say you are satisfied with your care.\" --stdout | aplay 2>/dev/null");

    //Let motor on channel 2 turn backwards with a PWM duty cycle of 92%
    //No change for motor on channel 1
    LegoMotorDirectControl(&LegoMotor,1,-30000);
    LegoMotorDirectControl(&LegoMotor,2,-30000);

    //Pause for 2 seconds, motors keeps turning
    _delay_ms(2000);

    //Set both motors on 'Direct control mode' with active braking on stop
    LegoMotorSetup(&LegoMotor,1,1,0);
    LegoMotorSetup(&LegoMotor,2,1,0);

    //Stop both motors with active braking
    LegoMotorDirectControl(&LegoMotor,1,0);
    LegoMotorDirectControl(&LegoMotor,2,0);

    //eSpeak voice say 'Ready'
    system ("espeak -ven+f2 -k5 -a50 -s150 \"Ready.\" --stdout | aplay 2>/dev/null");

    //Print on debug terminal
    printf ("Ready.\n");
}
