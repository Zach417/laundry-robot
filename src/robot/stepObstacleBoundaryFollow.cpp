/*
  1. Estimate section of obstacle using sensors
  2. Compute vectors tangential to obstacle and perpendicular to obstacle
  3. Combine two vectors and steer in director of new vector
*/

#include "robot/robot.h"

int wallFollowProgramStep = 0;
int wallFollowProgramStepCount = -1;
void Robot::stepObstacleBoundaryFollow() {
  int enterDistance = 24;
  int exitDistance = 36;

  int distanceLeft = sensorLeft.getPreviousRead();
  int distanceRight = sensorRight.getPreviousRead();
  int distanceForward = sensorForward.getPreviousRead();

  float thetaDesired = waypointSet.getThetaDesired(x, y);

  if (abs(thetaDesired - theta) > PI) {
    if (distanceLeft < enterDistance) {
      flagObstacleBoundaryFollow = true;
      wallFollowProgramStep = 1;
      wallFollowProgramStepCount = -1;
    } else if (distanceRight < enterDistance) {
      flagObstacleBoundaryFollow = true;
      wallFollowProgramStep = 1;
      wallFollowProgramStepCount = -1;
    }
  }

  if (flagObstacleBoundaryFollow == true) {
    if (wallFollowProgramStep == 1) {
      if (distanceLeft < enterDistance / 2) {
        float omega = PI / 2;
        targetVelocityLeft = (2*velocitySlow + omega*wheelAxelLength)/(2*wheelRadius);
        targetVelocityRight = (2*velocitySlow - omega*wheelAxelLength)/(2*wheelRadius);
      } else if (distanceLeft >= enterDistance / 2) {
        wallFollowProgramStep = 2;
      } else if (distanceRight < enterDistance / 2) {
        float omega = -PI / 2;
        targetVelocityLeft = (2*velocitySlow + omega*wheelAxelLength)/(2*wheelRadius);
        targetVelocityRight = (2*velocitySlow - omega*wheelAxelLength)/(2*wheelRadius);
      } else if (distanceRight >= enterDistance / 2) {
        wallFollowProgramStep = 2;
      }
    } else if (wallFollowProgramStep == 2) {
      wallFollowProgramStepCount++;
      targetVelocityLeft = velocitySlow;
      targetVelocityRight = velocitySlow;

      if (wallFollowProgramStepCount >= 20) {
        flagObstacleBoundaryFollow = false;
        wallFollowProgramStep = 0;
        wallFollowProgramStepCount = -1;
      }
    }
  }
}
