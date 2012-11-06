#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     lifter,        tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motorI,        tmotorNormal, openLoop)
#pragma config(Sensor, S2,     HTcompass,           sensorI2CCustom)
#pragma config(Sensor, S3,     HTEOPD,               sensorI2CCustom)


#include "drivers/HTEOPD-driver.h"


#ifndef _PICKLE_EOPD_DRIVER
#define _PICKLE_EOPD_DRIVER
bool _EOPDIsSetUp=false;


bool isDetecting(tSensors sensor)
{
  if (!_EOPDIsSetUp)
  {
    _EOPDIsSetUp=true;
    HTEOPDsetLongRange(sensor);
  }
  return HTEOPDreadRaw(sensor)<10;

}
#ifdef __HTSMUX_SUPPORT_
bool isDetecting(tMUXSensor muxsensor)
{
  if (!_EOPDIsSetUp)
  {
    _EOPEIsSetUp=true;
    HTEOPDsetLongRange(muxsensor);
  }
  return HTEOPDreadRaw(muxsensor)<10;
}
#endif
#endif
