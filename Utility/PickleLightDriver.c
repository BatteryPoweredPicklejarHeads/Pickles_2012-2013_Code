#ifndef __LEGOLS_H__
#define __LEGOLS_H__
//Based on Xander Soldaat's Light Sensor Driver
//Unused overloaded functions removed to lower the likelihood
//of crossing the 160 function limit.
//This can calibrate light sensors connected in any manner to the NXT.
#pragma systemFile

#ifndef __COMMON_H__
#include "drivers/common.h"
#endif
#include "drivers/HTSMUX-driver.h"
#define PORT1 0
#define PORT2 4
#define PORT3 8
#define PORT4 12

// Globals
//Stores whether or not a sensor has been calibrated
bool calibrated[16]={false,false,false,false,false,false,false,false,false,
                     false,false,false,false,false,false,false};
//Calibration Data
int lows[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int highs[16]={1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,
               1023,1023,1023,1023};
//Calibration Data Files
const string dataFiles[16]={"zero.dat","one.dat","two.dat","three.dat",
                            "four.dat","five.dat","six.dat","seven.dat",
                            "eight.dat","nine.dat","ten.dat","eleven.dat",
                            "twelve.dat","thirteen.dat","fourteen.dat",
                            "fifteen.dat"};

//Set the light sensors to be in active mode
void LSsetActive(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueActive(muxsensor);
}

void LSsetActive(tSensors link) {
  SensorType[link] = sensorLightActive;
  SensorMode[link] = modeRaw;
  wait1Msec(5);
}

//Set the light sensors to be in inactive mode
void LSsetInactive(tMUXSensor muxsensor) {
  HTSMUXsetAnalogueInactive(muxsensor);
}

void LSsetInactive(tSensors link) {
  SensorType[link] = sensorLightInactive;
  SensorMode[link] = modeRaw;
  wait1Msec(5);
}

//Return an index to the array of data
int lookUpSensorLINK(tSensors link)
{
  switch (link)
  {
    case S1:
      return PORT1;
      break;
    case S2:
      return PORT2;
      break;
    case S3:
      return PORT3;
      break;
    case S4:
      return PORT4;
      break;
    default:
      nxtDisplayCenteredTextLine(2,"Invalid SensorPort");
      wait1Msec(5000);
      StopAllTasks();
      return -1;
  }
}

int lookUpSensorMUX(tMUXSensor currentSensor)
{
  switch (currentSensor)
  {
    case msensor_S1_1:
    return 0;
    break;
    case msensor_S1_2:
    return 1;
    break;
    case msensor_S1_3:
    return 2;
    break;
    case msensor_S1_4:
    return 3;
    break;
    case msensor_S2_1:
    return 4;
    break;
    case msensor_S2_2:
    return 5;
    break;
    case msensor_S2_3:
    return 6;
    break;
    case msensor_S2_4:
    return 7;
    break;
    case msensor_S3_1:
    return 8;
    break;
    case msensor_S3_2:
    return 9;
    break;
    case msensor_S3_3:
    return 10;
    break;
    case msensor_S3_4:
    return 11;
    break;
    case msensor_S4_1:
    return 12;
    break;
    case msensor_S4_2:
    return 13;
    break;
    case msensor_S4_3:
    return 14;
    break;
    case msensor_S4_4:
    return 15;
    break;
    default:
    nxtDisplayCenteredTextLine(2,"Invalid MUXsensor");
    wait1Msec(1000);
    StopAllTasks();
    return -1;
  }
}
void _LSwriteCalVals(int curSensor) {
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize = 4;
  //Open the file
  Delete(dataFiles[curSensor],nIoResult);
  OpenWrite(hFileHandle, nIoResult, dataFiles[curSensor], nFileSize);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    eraseDisplay();
    nxtDisplayTextLine(3, "W:can't cal file");
    PlaySound(soundException);
    while(bSoundActive);
    wait1Msec(5000);
    StopAllTasks();
  }

  // Write the low calibration value
  WriteShort(hFileHandle, nIoResult, lows[curSensor]);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    nxtDisplayTextLine(3, "can't write lowval");
    PlaySound(soundException);
    while(bSoundActive);
    wait1Msec(5000);
    StopAllTasks();
  }

  // Write the high calibration value
  WriteShort(hFileHandle, nIoResult, highs[curSensor]);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    nxtDisplayTextLine(3, "can't write highval");
    PlaySound(soundException);
    while(bSoundActive);
    wait1Msec(5000);
    StopAllTasks();
  }

  // Close the file
  Close(hFileHandle, nIoResult);
  if (nIoResult != ioRsltSuccess) {
    eraseDisplay();
    nxtDisplayTextLine(3, "Can't close");
    PlaySound(soundException);
    while(bSoundActive);
    wait1Msec(5000);
    StopAllTasks();
  }
}

void _LSreadCalVals(int sensorNum) {
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;
  short nFileSize;

  short lv = 0;
  short hv = 0;

  // Open the data file for reading
  calibrated[sensorNum]=true;
  OpenRead(hFileHandle, nIoResult, dataFiles[sensorNum],nFileSize);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Read the low calibration value
  ReadShort(hFileHandle, nIoResult, lv);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Read the high calibration value
  ReadShort(hFileHandle, nIoResult, hv);
  if (nIoResult != ioRsltSuccess) {
    Close(hFileHandle, nIoResult);
    return;
  }

  // Assign values and close file
  lows[sensorNum] = lv;
  highs[sensorNum] = hv;
  Close(hFileHandle, nIoResult);
}
//Check the sensor's function
void _LScheckSensor(tSensors link) {
  if (SensorMode[link] != modeRaw &&
    ((SensorType[link] != sensorLightActive) ||
     (SensorType[link] != sensorLightInactive))) {
      LSsetInactive(link);
    }
}
//Return the raw value
int LSvalRaw(tSensors link) {
  _LScheckSensor(link);

  return SensorRaw[link];
}

int LSvalRaw(tMUXSensor muxsensor) {
  return 1023 - HTSMUXreadAnalogue(muxsensor);
}

//Return a normalized value
int LSvalNorm(tSensors link) {
  long currval = 0;

  _LScheckSensor(link);
  int curSensor=lookUpSensorLINK(link);
  if (!calibrated[curSensor]) {
    _LSreadCalVals(curSensor);
  }

  currval = LSvalRaw(link);

  if (currval <= lows[curSensor])
    return 0;
  else if (currval >= highs[curSensor])
    return 100;

  return ((currval-lows[curSensor])*100)/(highs[curSensor]-lows[curSensor]);
}


int LSvalNorm(tMUXSensor muxsensor) {
  long currval = 0;
  int curSensor=lookUpSensorMUX(muxsensor);
  if (!calibrated[curSensor]) {
    _LSreadCalVals(curSensor);
  }

  currval = LSvalRaw(muxsensor);

  if (currval <= lows[curSensor])
    return 0;
  else if (currval >= highs[curSensor])
    return 100;

  return ((currval-lows[curSensor])*100)/(highs[curSensor]-lows[curSensor]);
}

//Calibration
//Store the low sensor value
void LScalLow(tMUXSensor muxsensor) {
  int curSensor=lookUpSensorMUX(muxsensor);
  lows[curSensor] = LSvalRaw(muxsensor);
  _LSwriteCalVals(curSensor);
}


void LScalLow(tSensors link) {
  _LScheckSensor(link);
  int curSensor=lookUpSensorLINK(link);
  lows[curSensor] = SensorRaw[link];
  _LSwriteCalVals(curSensor);
}


//Store the high value
void LScalHigh(tMUXSensor muxsensor) {
  int curSensor=lookUpSensorMUX(muxsensor);
  highs[curSensor] = LSvalRaw(muxsensor);
  _LSwriteCalVals(curSensor);
}

void LScalHigh(tSensors link)
{
  _LScheckSensor(link);
  int curSensor=lookUpSensorLINK(link);
  highs[curSensor] = SensorRaw[link];
  _LSwriteCalVals(curSensor);
}
#endif // __LEGOLS_H__
