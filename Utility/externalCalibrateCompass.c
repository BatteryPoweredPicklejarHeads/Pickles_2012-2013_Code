#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTMotor,  none)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     backLeft,      tmotorNormal, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     frontLeft,     tmotorNormal, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     armLifterl,    tmotorNormal, PIDControl, encoder)
#pragma config(Sensor, S2,     HTcompass,                  sensorI2CCustom)

//Normalize the heading to 0-360 degrees.
int normalizeTheta360(int input)
{
  while (input<0) input=input+360;
  while (input>=360) input=input-360;
  return input;
}


float absFloat(float input)
{
return (input<0)?-input:input;
}

//Calculate the difference between two headings.
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
//#include "drivers/HTSMUX-driver.h"
#include "drivers/HTMC-driver.h"
#define IR msensor_S3_1
#define COMPASSADJUSTMENTSDAT "compAdjusts.dat"
#define BASEDATADAT "baseAdjust.dat"

float result[360];

int firstOccurrence[360];
int numOccurrences[360];
float deltaSum[360];
int arrays[2][2580];


task main()
{

  for (int init=0; init<360; init++)
  {
    result[init]=-1;
    numOccurrences[init]=-1;
    deltaSum[init]=0;
  }
  int curHeading=1;
  int counter=0;
  bool passedHalf=false;
  motor[frontLeft]=-10;
  motor[frontRight]=-10;
  motor[backLeft]=10;
  motor[backRight]=10;
  wait1Msec(1000);
  float baseHeading=HTMCreadHeading(HTcompass);

  ClearTimer(T1);
  //Spin around one full rotation
  while(curHeading!=359 || !passedHalf)
  {
    curHeading=normalizeTheta360(HTMCreadHeading(HTcompass)-baseHeading);
    nxtDisplayTextLine(2, "%d", curHeading);
    arrays[0][counter]=curHeading;
    arrays[1][counter]=time1[T1];
    counter++;
    if (abs(180-curHeading)<10) passedHalf=true;
  }
  motor[frontRight]=0;
  motor[frontLeft]=0;
  motor[backRight]=0;
  motor[backLeft]=0;
  //while (nNxtButtonPressed!=kButtonCenter);
  float timePerDegree=time1[T1]/(float)360;
  //Calculate at each sensor reading what the real heading was.
  for (int normal=0; normal<counter; normal++)
  {
    arrays[1][normal]=(float)arrays[1][normal]/
                      (float) timePerDegree;
  }
  for (int i=0; i<360; i++)
  {
    firstOccurrence[i]=-1;
  }

  //Find the first occurrence of each sensor reading.
  //Then, find all the other readings and sum the differences
  //between them and the first value.
  for (int arrayCount=0; arrayCount<counter; arrayCount++)
  {
    numOccurrences[arrays[0][arrayCount]]++;
    if (firstOccurrence[arrays[0][arrayCount]]==-1)
    {
      firstOccurrence[arrays[0][arrayCount]]=arrayCount;
    }
    else
    {
      float firstValue=arrays[1][firstOccurrence[arrays[0][arrayCount]]];
      float curValue=arrays[1][arrayCount];
      float absDif=compassDif(firstValue, curValue);
      if (firstValue>curValue) absDif=absDif*-1;
      if (absFloat(firstValue-curValue)>180) absDif=absDif*-1;
      deltaSum[arrays[0][arrayCount]]=
                   deltaSum[arrays[0][arrayCount]]+absDif;
    }

  }

  //Write all the values
  //(first occurrence+(sum of the differences/number of extra readings))
  for (int i=0; i<360; i++)
  {
    if (numOccurrences[i]!=-1)
    {
      if (numOccurrences[i]==0)
      {
        result[i]=arrays[i][firstOccurrence[i]];
      }
      else
      {
        result[i]=normalizeTheta360(arrays[1][firstOccurrence[i]]
                         +(deltaSum[i]/(float)numOccurrences[i]));
                         nxtDisplayTextLine(3, "%f, %d",deltaSum[i], numOccurrences[i]);
      }
    }
  }
  //If a value never shows up set it to the average of the adjacent values.
  for (int i=0; i<360; i++)
  {
    if (numOccurrences[i]==-1)
    {
      int prev=-1, next=-1;
      for (int findPrev=normalizeTheta360(i-1); prev==-1;
                                 findPrev=normalizeTheta360(findPrev-1))
      {
        if (result[findPrev]!=-1) prev=findPrev;
      }
      for (int findNext=normalizeTheta360(i+1); next==-1;
                                 findNext=normalizeTheta360(findNext+1))
      {
        if (result[findNext]!=-1) next=findNext;
      }

      result[i]=result[prev]+compassDif(result[prev], result[next])/2;
    }
  }

  //Write everything to the filesystem.
  TFileHandle hFileHandle;
  TFileIOResult nIoResult;

  short nFileSize = sizeof(baseHeading);
  Delete(BASEDATADAT, nIoResult);
  OpenWrite(hFileHandle, nIoResult, BASEDATADAT, nFileSize);
  WriteFloat(hFileHandle, nIoResult, baseHeading);
  Close(hFileHandle, nIoResult);


  nFileSize = sizeof(result);
  Delete(COMPASSADJUSTMENTSDAT, nIoResult);
  OpenWrite(hFileHandle, nIoResult, COMPASSADJUSTMENTSDAT, nFileSize);
  for (int i=0; i<360; i++)
  {
    WriteFloat(hFileHandle, nIoResult, result[i]);
  }
  Close(hFileHandle, nIoResult);

  while(true)
  {
    nxtDisplayTextLine(0, "%f", result[HTMCreadHeading(HTcompass)]);
  }

}
