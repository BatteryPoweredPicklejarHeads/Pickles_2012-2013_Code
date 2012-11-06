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

#define FIELD FIELD_HOME

#include "PickleCompassDriver.c"

int sizemult;

int getValue(int prevReading)
{
  sizemult++;
  while(true)
  {
    int btn=nNxtButtonPressed;
    if (btn==2) return prevReading;
    if (btn==1) return readCompass();
  }
}


task main()
{

  sizemult=0;
  loadCalibrationData();
  nxtDisplayTextLine(6, "  Prev    New");
  nxtDisplayTextLine(2, FIELD);
  nxtDisplayBigTextLine(0, "BPgm1one");
  float one1b=getValue(BPgm1one);
  pause();
  nxtDisplayBigTextLine(0, "BPgm1two");
  float one2b=getValue(BPgm1two);
  pause();
  nxtDisplayBigTextLine(0, "BPgm1three");
  float one3b=getValue(BPgm1three);
  pause();
  nxtDisplayBigTextLine(0, "BPgm1four");
  float one4b=getValue(BPgm1four);
  pause();
  nxtDisplayBigTextLine(0, "BPgm1five");
  float one5b=getValue(BPgm1five);
  pause();
  nxtDisplayBigTextLine(0, "BPgm1six");
  float one6b=getValue(BPgm1six);
  pause();


  nxtDisplayBigTextLine(0, "BPgm2one");
  float two1b=getValue(BPgm2one);
  pause();


  nxtDisplayBigTextLine(0, "BPgm3one");
  float three1b=getValue(BPgm3one);
  pause();


  nxtDisplayBigTextLine(0, "BPgm4one");
  float four1b=getValue(BPgm4one);
  pause();
  nxtDisplayBigTextLine(0, "BPgm4two");
  float four2b=getValue(BPgm4two);
  pause();
  nxtDisplayBigTextLine(0, "BPgm4three");
  float four3b=getValue(BPgm4three);
  pause();


  nxtDisplayBigTextLine(0, "RPgm1one");
  float one1r=getValue(RPgm1one);
  pause();
  nxtDisplayBigTextLine(0, "RPgm1two");
  float one2r=getValue(RPgm1two);
  pause();
  nxtDisplayBigTextLine(0, "RPgm1three");
  float one3r=getValue(RPgm1three);
  pause();
  nxtDisplayBigTextLine(0, "RPgm1four");
  float one4r=getValue(RPgm1four);
  pause();
  nxtDisplayBigTextLine(0, "RPgm1five");
  float one5r=getValue(RPgm1five);
  pause();
  nxtDisplayBigTextLine(0, "RPgm1six");
  float one6r=getValue(RPgm1six);
  pause();

  nxtDisplayBigTextLine(0, "RPgm2one");
  float two1r=getValue(RPgm2one);
  pause();
  nxtDisplayBigTextLine(0, "RPgm3one");
  float three1r=getValue(RPgm3one);
  pause();

  nxtDisplayBigTextLine(0, "RPgm4one");
  float four1r=getValue(RPgm4one);
  pause();
  nxtDisplayBigTextLine(0, "RPgm4two");
  float four2r=getValue(RPgm4one);
  pause();
  nxtDisplayBigTextLine(0, "RPgm4three");
  float four3r=getValue(RPgm4one);
  pause();
  eraseDisplay();

  TFileHandle hFileHandle;
  TFileIOResult nIoResult;

  Delete(FIELD, nIoResult);
  int nFileSize=sizeof(one1b)*sizemult;
  OpenWrite(hFileHandle, nIoResult, FIELD, nFileSize);

  WriteFloat(hFileHandle, nIoResult, one1b);
  WriteFloat(hFileHandle, nIoResult, one2b);
  WriteFloat(hFileHandle, nIoResult, one3b);
  WriteFloat(hFileHandle, nIoResult, one4b);
  WriteFloat(hFileHandle, nIoResult, one5b);
  WriteFloat(hFileHandle, nIoResult, one6b);

  WriteFloat(hFileHandle, nIoResult, two1b);

  WriteFloat(hFileHandle, nIoResult, three1b);

  WriteFloat(hFileHandle, nIoResult, four1b);
  WriteFloat(hFileHandle, nIoResult, four2b);
  WriteFloat(hFileHandle, nIoResult, four3b);

  WriteFloat(hFileHandle, nIoResult, one1r);
  WriteFloat(hFileHandle, nIoResult, one2r);
  WriteFloat(hFileHandle, nIoResult, one3r);
  WriteFloat(hFileHandle, nIoResult, one4r);
  WriteFloat(hFileHandle, nIoResult, one5r);
  WriteFloat(hFileHandle, nIoResult, one6r);

  WriteFloat(hFileHandle, nIoResult, two1r);

  WriteFloat(hFileHandle, nIoResult, three1r);

  WriteFloat(hFileHandle, nIoResult, four1r);
  WriteFloat(hFileHandle, nIoResult, four2r);
  WriteFloat(hFileHandle, nIoResult, four3r);

  Close(hFileHandle, nIoResult);
}
