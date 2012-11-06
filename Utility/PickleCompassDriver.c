#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     armLifterl,    tmotorNormal, PIDControl, encoder);
#pragma config(Sensor, S2,     HTcompass,                  sensorI2CCustom)


#ifndef _PICKLE_COMPASS
#define _PICKLE_COMPASS
int normalizeTheta360(int input)
{
  while (input<0) input=input+360;
  while (input>=360) input=input-360;
  return input;
}
int normalizeThetaPlusMinus180(int input)
{
  while (input>=180) input=input-360;
  while (input<=-180) input=input+360;
  return input;
}

int compassTarget=0;

#define FIELD_HOME "field_home.dat"
#define FIELD_PRACTICE "field_practice.dat"
#define FIELD_COMPETITION_RIGHT "field_right.dat"
#define FIELD_COMPETITION_LEFT "field_left.dat"


void pause()
{
  while (nNxtButtonPressed!=3){};
}

#define IR msensor_S3_1

#include "drivers/HTMC-driver.h"
#define COMPASSADJUSTMENTSDAT "compAdjusts.dat"
#define BASEDATADAT "baseAdjust.dat"
#define TELEOPFORWARDSDAT "TeleopForwards.dat"

float result[360];
float baseHeading;
void setTarget(int input)
{
  compassTarget=input;
}
float readCompass()
{
  return result[normalizeTheta360(HTMCreadHeading(HTcompass)-baseHeading)];
}
float readRelativeCompass()
{
  //Taken from HTMC-driver.h
  int temp = readCompass() - compassTarget + 180;
  return (temp >= 0 ? temp % 360 : 359 - (-1 - temp)%360) - 180;
}

float BPgm1one, BPgm1two, BPgm1three, BPgm1four, BPgm1five,BPgm1six,
      BPgm2one,
      BPgm3one,
      BPgm4one, BPgm4two, BPgm4three,

      RPgm1one, RPgm1two, RPgm1three, RPgm1four, RPgm1five,RPgm1six,
      RPgm2one,
      RPgm3one,
      RPgm4one, RPgm4two, RPgm4three;

void writeTeleopForwards()
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  float teleopForwards=readCompass();
  short nFileSize=sizeof(teleopForwards);
  OpenWrite(hFileHandle, nIoResult, TELEOPFORWARDSDAT, nFileSize);
  WriteFloat(hFileHandle, nIoResult, teleopForwards);
  Close(hFileHandle, nIoResult);
}
float readTeleopForwards()
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  float result=0;
  short nFileSize=sizeof(baseHeading);
  OpenRead(hFileHandle, nIoResult, TELEOPFORWARDSDAT, nFileSize);
  ReadFloat(hFileHandle, nIoResult, result);
  Close(hFileHandle, nIoResult);
  return result;
}
void loadCalibrationData()
{
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize=sizeof(baseHeading);
  OpenRead(hFileHandle, nIoResult, BASEDATADAT, nFileSize);
  ReadFloat(hFileHandle, nIoResult, baseHeading);
  Close(hFileHandle, nIoResult);



  nFileSize=sizeof(result);
  OpenRead(hFileHandle, nIoResult, COMPASSADJUSTMENTSDAT, nFileSize);
  for (int i=0; i<360; i++)
  {
    ReadFloat(hFileHandle, nIoResult, result[i]);
  }
  Close(hFileHandle, nIoResult);


  /*nFileSize=22*sizeof(BPgm1one);
  OpenRead(hFileHandle, nIoResult, FIELD, nFileSize);

  ReadFloat(hFileHandle, nIoResult, BPgm1one);
  ReadFloat(hFileHandle, nIoResult, BPgm1two);
  ReadFloat(hFileHandle, nIoResult, BPgm1three);
  ReadFloat(hFileHandle, nIoResult, BPgm1four);
  ReadFloat(hFileHandle, nIoResult, BPgm1five);
  ReadFloat(hFileHandle, nIoResult, BPgm1six);

  ReadFloat(hFileHandle, nIoResult, BPgm2one);

  ReadFloat(hFileHandle, nIoResult, BPgm3one);

  ReadFloat(hFileHandle, nIoResult, BPgm4one);
  ReadFloat(hFileHandle, nIoResult, BPgm4two);
  ReadFloat(hFileHandle, nIoResult, BPgm4three);

  ReadFloat(hFileHandle, nIoResult, RPgm1one);
  ReadFloat(hFileHandle, nIoResult, RPgm1two);
  ReadFloat(hFileHandle, nIoResult, RPgm1three);
  ReadFloat(hFileHandle, nIoResult, RPgm1four);
  ReadFloat(hFileHandle, nIoResult, RPgm1five);
  ReadFloat(hFileHandle, nIoResult, RPgm1six);

  ReadFloat(hFileHandle, nIoResult, RPgm2one);

  ReadFloat(hFileHandle, nIoResult, RPgm3one);

  ReadFloat(hFileHandle, nIoResult, RPgm4one);
  ReadFloat(hFileHandle, nIoResult, RPgm4two);
  ReadFloat(hFileHandle, nIoResult, RPgm4three);

  Close(hFileHandle, nIoResult);*/

}
float absFloat(float input)
{
  return (input<0)?-input:input;
}
float compassDif(float one, float two)
{
  float highVal=one+360;
  float lowVal=one-360;
  float normalDifference=absFloat(two-one);
  float highDifference=absFloat(two-highVal);
  float lowDifference=absFloat(two-lowVal);
  if (lowDifference<normalDifference && lowDifference<highDifference)
  {
    return lowDifference;
  }
  else if (normalDifference<highDifference)
  {
    return normalDifference;
  }
  else
  {
    return highDifference;
  }
}
void compassTurn(int heading)
{
  setTarget(normalizeTheta360(heading));
  while (abs(readRelativeCompass())>2)
  {
    nxtDisplayTextLine(2, "%f, %d, %f", readRelativeCompass(), heading, readCompass());
    int dif=-readRelativeCompass();
    int sign;
    if (dif<0) sign=-1;
    else sign=1;
    dif=abs(dif);
    if(abs(dif)<25)
    {
      motor[frontLeft] =sign*-20;
      motor[frontRight]=sign*-20;
      motor[backLeft]  =sign*20;
      motor[backRight] =sign*20;
    }
    else
    {
      motor[frontLeft] =sign*-dif*4/(float)5;
      motor[frontRight]=sign*-dif*4/(float)5;
      motor[backLeft]  =sign*dif*4/(float)5;
      motor[backRight] =sign*dif*4/(float)5;
    }
  }
  motor[frontLeft]=0;
  motor[frontRight]=0;
  motor[backLeft]=0;
  motor[backRight]=0;
  //pause();
}

#endif
