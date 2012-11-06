#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  HTServo)
#pragma config(Sensor, S2,     HTMAG,               sensorHiTechnicMagnetic)
#pragma config(Sensor, S3,     HTSMUX,               sensorI2CCustom)
#pragma config(Sensor, S4,     HTcompass,                  sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     frontLeft,     tmotorNormal, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     frontRight,    tmotorNormal, PIDControl)
#pragma config(Motor,  mtr_S1_C2_1,     conveyor,      tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     bowlBallGrabber, tmotorNormal, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     backLeft,      tmotorNormal, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C3_2,     backRight,     tmotorNormal, PIDControl)
#pragma config(Servo,  srvo_S1_C4_1,    redMagStorer,        tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_2,    magSwitch,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_3,    blueMagDumper,            tServoStandard)
#pragma config(Servo,  srvo_S1_C4_4,    blueMagStorer,         tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C4_5,    redMagDumper,         tServoStandard)
#pragma config(Servo,  srvo_S1_C4_6,    crateArm,             tServoStandard)

#include "drivers/HTSMUX-driver.h"
#include "PickleCompassDriver.c"

typedef struct
{
  long target, P,prevP,I,D, absdifference, expectedPos, startPoint;
  int power, direction, PIDadjustedPower;
}myMotor;

#define mfrontLeft 0
#define mfrontRight 1
#define mbackLeft 2
#define mbackRight 3

#define kP (float)0.1
#define kI (float)0
#define kD (float)0

#define threshold 100

// bool readSwitchState()
// {
// return true;
// int inputdata;  //statemenst copied from HTSPB-exp1.c
// HTSPBsetupIO(HTSPB, 0x1);
// inputdata = HTSPBreadADC(HTSPB, 0, 10);
// nxtDisplayTextLine(1, "A0: %d", inputdata);
// return (inputdata>512);
// }


bool initializeRobot()
{

  return true;
}
float tan(float theta)
{
  return sinDegrees(theta)/cosDegrees(theta);
}
void reset()
{
  nMotorEncoder[frontLeft]=0;
  nMotorEncoder[frontRight]=0;
  nMotorEncoder[backLeft]=0;
  nMotorEncoder[backRight]=0;
  return;
}

bool allArrived(long first, long second, long third, long fourth) // This function returns true when all of the wheels have arrived at their final encoder value for a specific move
{
  if (abs(first)<threshold&&abs(second)<threshold&&abs(third)<threshold&&abs(fourth)<threshold)
    return true;
  return false;
}

int greatest(long first, long second, long third, long fourth) {
  if ((first>second)&&(first>third)&&(first>fourth)) {
    return mfrontLeft;
  }
  else if ((second>=first)&&(second>third)&&(second>fourth)) {
    return mfrontRight;
  }
  else if ((third>=first)&&(third>=second)&&(third>fourth)) {
    return mbackLeft;
  }
  else  {
    return mbackRight;
  }
}

int calcPower(long top, long bottom, int basePower, int direction) {
  return (((float)top*basePower*direction)/(float)bottom);
}

int lookUpComparison(int theMotor)
{
  switch(theMotor)
  {
  case mfrontLeft:
    return frontLeft;
    break;
  case mfrontRight:
    return frontRight;
    break;
  case mbackLeft:
    return backLeft;
    break;
  case mbackRight:
    return backRight;
    break;
  }
  return -1;
}

void stopMotors() // Stops all motors
{
  motor[frontLeft]=0;
  motor[frontRight]=0;
  motor[backLeft]=0;
  motor[backRight]=0;
  return;
}

myMotor motors[4];

float atan2(long y,long nx)
{
  return 2*atan(y/(sqrt(nx*nx+y*y)+nx));
}
float absFloat(float input)
{
return(input>0)?input:-input;
}

void move(int power)  //
{
  reset();
  for (int cnt=0; cnt<4; cnt++)
  {
    motors[cnt].absdifference=abs(motors[cnt].target-nMotorEncoder[lookUpComparison(cnt)]);
    motors[cnt].P=0;
    motors[cnt].prevP=0;
    motors[cnt].I=0;
    motors[cnt].D=0;
    motors[cnt].PIDadjustedPower=0;
    motors[cnt].startPoint=nMotorEncoder[lookUpComparison(cnt)];
  }
  //int greatestTarget=greatest(abs(motors[mfrontLeft].target),abs(motors[mfrontRight].target), abs(motors[mbackLeft].target), abs(motors[mbackRight].target));
  while (!allArrived(motors[mfrontLeft].absdifference,motors[mfrontRight].absdifference,motors[mbackLeft].absdifference,motors[mbackRight].absdifference))
  {
    int greatestValue=greatest(abs(motors[mfrontLeft].absdifference),abs(motors[mfrontRight].absdifference), abs(motors[mbackLeft].absdifference), abs(motors[mbackRight].absdifference));
    nxtDisplayCenteredTextLine(6, "%d,%d,%d", greatestValue, motors[greatestValue].absdifference, motors[greatestValue].target);
    for (int cnt=0; cnt<4; cnt++)
    {
      nxtDisplayCenteredTextLine(cnt, "%d,%d,%d", nMotorEncoder[lookUpComparison(cnt)], motors[cnt].absdifference, motors[cnt].power);
      writeDebugStreamLine("%d:%d,%d,%d", cnt,nMotorEncoder[lookUpComparison(cnt)], motors[cnt].absdifference, motors[cnt].power);
      //nxtDisplayCenteredTextLine(cnt+4, "%d,%d,%d", motors[cnt].absdifference, motors[cnt].P, motors[cnt].PIDadjustedPower);
      motors[cnt].absdifference=abs(motors[cnt].target-nMotorEncoder[lookUpComparison(cnt)]);
      motors[cnt].direction=( motors[cnt].target<nMotorEncoder[lookUpComparison(cnt)])?-1:1;
      motors[cnt].power=calcPower(abs(motors[cnt].absdifference), abs(motors[greatestValue].absdifference),abs(power), motors[cnt].direction);
      // motors[cnt].expectedPos=(((float)motors[cnt].target-motors[cnt].startPoint) * abs((float)expectedTravelDistance / (float)motors[greatestTarget].target)) +motors[cnt].startPoint;
      // motors[cnt].P=nMotorEncoder[lookUpComparison(cnt)]-motors[cnt].expectedPos;
      // motors[cnt].I+=motors[cnt].P;
      // motors[cnt].D=motors[cnt].P-motors[cnt].prevP;
      // motors[cnt].prevP=motors[cnt].P;
      //motors[cnt].PIDadjustedPower=((motors[cnt].P*kP)+(motors[cnt].I*kI)+(motors[cnt].D*kD))+motors[cnt].power;
      //motors[cnt].PIDadjustedPower=motors[cnt].power;//+(kP*motors[cnt].P);
    }
    for (int cnt=0; cnt<4; cnt++)
    {
      motor[lookUpComparison(cnt)]=motors[cnt].power;
    }
  }
  stopMotors();
  return;
}

#define RED 0
#define BLUE 1
#define SIDEDATADAT "SideData.dat"

void writeSide(short isLeft)
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize = 16;
  Delete(SIDEDATADAT, nIoResult);
  OpenWrite(hFileHandle, nIoResult, SIDEDATADAT, nFileSize);
  WriteShort(hFileHandle, nIoResult, isLeft);
  Close(hFileHandle, nIoResult);
}
