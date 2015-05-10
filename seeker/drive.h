#ifndef DRIVE_H
#define DRIVE_H

#include "robotapp.h"
#include "timestep.h"
#include "math.h"

/**
 * \brief Initialize drive system
 * \return
 * 0:OK \n
 * 1:Error \n
*/
int DriveInit(void);

/**
 * \brief Drive robot straight forward over specified distance with specified speed. Returns after drive is complete.
 * \param Distance :Distance to travel in mm
 * \param Speed :Speed in mm/s
*/
void DriveStraightDistance(float Distance,float Speed);

/**
 * \brief Rotate robot around right wheel over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/
void DriveRotateRWheel(float Angle,float Speed);

/**
 * \brief Rotate robot around left wheel over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/
void DriveRotateLWheel(float Angle,float Speed);

/**
 * \brief Rotate robot around center of wheelbase over specified angle, at specified speed. Returns after drive is complete.
 * \param Angle :Angle to rotate in deg
 * \param Speed :Speed in mm/s
*/
void DriveRotateCenter(float Angle,float Speed);

/**
 * \brief Follow line until specified distance has been travelled
 * \param Distance :Distance to travel in mm
 * \param Speed :Speed in mm/s
*/
void DriveLineFollowDistance(int Distance, float Speed);

/**
 * \brief Follow line until end of line segment
 * \param Speed :Speed in mm/s
*/
void DriveLineFollow(float Speed);

#endif
