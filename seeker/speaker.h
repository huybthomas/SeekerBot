#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * \brief Use the computer voice to say a message trough the build-in speaker.
 * \param message :Pointer to the message string to be said
*/
void espeak(char* message);

#endif
