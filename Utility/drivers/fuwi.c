#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none, none)
#pragma config(Sensor, S2,     HTSMUX,              sensorI2CCustom)
#pragma config(Sensor, S3,     HTSMUX,              sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     backLeft,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backRight,      tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, reversed, encoder)

#include "drivers/HTSMUX-driver.h"
#include "drivers/HTIRS2-driver.h"
#include "drivers/HTMC-driver.h"
#include "drivers/HTAC-driver.h"
#include "drivers/LEGOUS-driver.h"

#define HTcompass msensor_S2_1
#define accel msensor_S2_2
#define IR msensor_S2_3
#define ultra msensor_S2_4
#define IR2 msensor_S3_1

task main()
{
  while(true)
  nxtDisplayTextLine(0, "%f", (USreadDist(ultra))*0.3937007);
}

