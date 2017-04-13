#include "robot/robot.h"

decode_results results;
int manualStepCount = 0;
float manualPwmLeft = 0;
float manualPwmRight = 0;
float manualPwm = 85;
void Robot::stepRemote() {
  if (sensorIR.decode(&results)) {
    switch(results.value) {
      case 0xFF02FD: // ok -> stop
        Serial.println("STOP");
        autonomousMode = false;
        manualStepCount = 0;
        manualPwmLeft = 0;
        manualPwmRight = 0;
        break;
      case 0xFF629D: // forward
        autonomousMode = false;
        manualStepCount = 0;
        manualPwmLeft = manualPwm;
        manualPwmRight = manualPwm;
        break;
      case 0xFF22DD: // left
        autonomousMode = false;
        manualStepCount = 0;
        manualPwmLeft = -manualPwm;
        manualPwmRight = manualPwm;
        break;
      case 0xFFC23D: // right
        autonomousMode = false;
        manualStepCount = 0;
        manualPwmLeft = manualPwm;
        manualPwmRight = -manualPwm;
        break;
      case 0xFFA857: // reverse
        autonomousMode = false;
        manualStepCount = 0;
        manualPwmLeft = -manualPwm;
        manualPwmRight = -manualPwm;
        break;
      case 0xFF6897: // 1
        manualPwm = 45;
        Serial.println("Manual speed changed to 45%");
        break;
      case 0xFF9867: // 2
        manualPwm = 65;
        Serial.println("Manual speed changed to 65%");
        break;
      case 0xFFB04F: // 3
        manualPwm = 80;
        Serial.println("Manual speed changed to 80%");
        break;
      case 0xFF30CF: Serial.println("4");    break;
      case 0xFF18E7: Serial.println("5");    break;
      case 0xFF7A85: Serial.println("6");    break;
      case 0xFF10EF: Serial.println("7");    break;
      case 0xFF38C7: Serial.println("8");    break;
      case 0xFF5AA5: Serial.println("9");    break;
      case 0xFF4AB5: Serial.println("0");    break;
      case 0xFF52AD: // # -> reset odometry and navigation
        x = 0;
        y = 0;
        theta = 0;
        waypointSet.reset();
        autonomousMode = false;
        manualStepCount = 3;
        flagObstacleAvoidance = false;
        Serial.println("Reset navigation and odometry");
        break;
      case 0xFF42BD: // * -> toggle autonomous/manual mode
        if (autonomousMode == false ) {
          autonomousMode = true;
          Serial.println("Switched to autonomous mode");
        } else {
          autonomousMode = false;
          Serial.println("Switched to manual mode");
        }
        break;
      case 0xFFFFFFFF: // repeat previous command
        manualStepCount = 0;
        break;
      default:
        Serial.print("Could not read IR command: ");
        Serial.println(results.value, HEX);
    }

    sensorIR.resume();
  }

  if (autonomousMode == false) {
    regulatorMotorLeft.Compute(0, 0);
    regulatorMotorRight.Compute(0, 0);
    regulatorNavigationOmega.Compute(0, 0);
    regulatorObstacleAvoidanceOmega.Compute(0, 0);
  }

  if (autonomousMode == false && manualStepCount < 3) {
    manualStepCount++;
    flagRemote = true;
    motorLeft.step(manualPwmLeft);
    motorRight.step(manualPwmRight);
  } else if (autonomousMode == false) {
    flagRemote = true;
    motorLeft.step(0);
    motorRight.step(0);
  } else {
    flagRemote = false;
  }
}
