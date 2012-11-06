#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     armLifterl,    tmotorNormal, PIDControl, encoder)
#pragma config(Sensor, S2,     HTcompass,                  sensorI2CCustom)
#pragma config(Sensor, S3,     HTEOPD,                     sensorI2CCustom)


#include "drivers/HTEOPD-driver.h"

task main()
{
  HTEOPDsetLongRange(HTEOPD);
  while (true)
  {
    if (HTEOPDreadRaw(HTEOPD)<10)
    {
	    motor[frontLeft]=40;
	    motor[frontRight]=40;
	    motor[backLeft]=40;
	    motor[backRight]=40;
	  }
	  else
	  {
	    motor[frontLeft]=0;
	    motor[frontRight]=0;
	    motor[backLeft]=0;
	    motor[backRight]=0;
	  }
	}
}
