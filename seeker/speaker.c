#include "speaker.h"

void espeak(char* message)
{
    char command[255];

    //Create espeak command
    sprintf(command, "espeak -ven+f2 -k5 -a50 -s150 \"%s\" --stdout | aplay 2>/dev/null", message);

    system(command);
}
