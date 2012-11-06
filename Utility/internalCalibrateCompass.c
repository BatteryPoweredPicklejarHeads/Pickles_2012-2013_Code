#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorH,        tmotorNone, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorNone, openLoop)
#pragma config(Sensor, S2,     HTcompass,                  sensorI2CCustom)



#include "drivers/HTSMUX-driver.h"
#include "drivers/HTMC-driver.h"

#define IR msensor_S3_1

task main()
{
  HTMCstartCal(HTcompass);
  motor[frontLeft]=-10;
  motor[frontRight]=-10;
  motor[backLeft]=10;
  motor[backRight]=10;
  wait1Msec(20000);
  motor[frontLeft]=0;
  motor[frontRight]=0;
  motor[backLeft]=0;
  motor[backRight]=0;
  HTMCstopCal(HTcompass);
  while(true)
  {
    nxtDisplayTextLine(0, "%d", HTMCReadHeading(HTcompass));
}
}
