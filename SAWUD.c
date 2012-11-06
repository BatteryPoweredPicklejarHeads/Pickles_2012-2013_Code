#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     lifter,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorNormal, openLoop)
#pragma config(Sensor, S2,     HTcompass,           sensorI2CCustom)
#pragma config(Sensor, S3,     touch,               sensorI2CCustom)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"
#include "OperateCompass2.c"
#include "drivers/LEGOTS-driver.h"

void initializeRobot()
{
  return;
}

void runMotors(float flbr, float frbl)
{
  motor[frontLeft] = flbr;
  motor[backLeft] = frbl;
  motor[backRight] = flbr;
  motor[frontRight] = frbl;
}

void encoderReset()
{
  nMotorEncoder[backLeft] = 0;
  nMotorEncoder[backRight] = 0;
  nMotorEncoder[frontRight] = 0;
  nMotorEncoder[frontLeft] = 0;
}
void stopMotors()
{
  motor[frontLeft] = 0;
  motor[backLeft] = 0;
  motor[backRight] = 0;
  motor[frontRight] = 0;
  wait1Msec(500);
}
void spin(int dir)
{
  motor[frontLeft] = dir*20;
  motor[frontRight] = dir*-20;
  motor[backLeft] =  dir*-20;
  motor[backRight] = dir*20;
  nxtDisplayTextLine(6, "awsdkjhfasd");
}

task main()
{
  loadCalibrationData();
  setTarget(readCompass()); //Sets the starting position of the robot to "north"
  float initVal = readCompass();
  bool touchVal = TSreadState(touch);

  encoderReset();
  while(abs(nMotorEncoder[lifter]) < 5)
  {
    motor[lifter] = -80;
  }
  stopMotors();
  encoderReset();
  while(!TSreadState(touch))
  {
    runMotors(20, 27);
  }
  stopMotors();
  encoderReset();
  while(abs(nMotorEncoder[backLeft]) < 300)
  {
    nxtDisplayCenteredTextLine(4, "%d", nMotorEncoder[backLeft]);
    runMotors(-20, -20);
  }
  stopMotors();
  encoderReset();
  while(abs(nMotorEncoder[backLeft]) < 750)
  {
    runMotors(20, -20);
  }
  stopMotors();
  encoderReset();
  while(abs(nMotorEncoder[backLeft]) < 650)
  {
    runMotors(20, 20);
  }
  compassTurn(initVal);
  stopMotors();
  encoderReset();
  while(abs(nMotorEncoder[backLeft]) < 625)
  {
    runMotors(-20, 20);
  }
  stopMotors();
  encoderReset();
  while(abs(nMotorEncoder[lifter]) < 400)
  {
    motor[lifter] = 80;
  }

}
