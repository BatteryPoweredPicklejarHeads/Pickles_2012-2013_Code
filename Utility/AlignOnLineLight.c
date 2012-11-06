




#ifndef _ALIGN_ON_LINE
#define _ALIGN_ON_LINE

#include "PickleLightDriver.c"
#include "PickleEOPDDriver.c"
#define frontPos 0.5
#define backPos -0.5
void moveForwardsToEOPD(int power)
{
  motor[frontRight]=power;
  motor[backLeft]=-power;
  motor[backRight]=0;
  motor[frontLeft]=0;
  while (isDetecting(frontEOPD));
  motor[frontRight]=0;
  motor[backLeft]=0;
}


void alignOnLineLeft(int power)
{
  LSsetActive(frontLight);
  LSsetActive(backLight);
  bool frontSensor=false, backSensor=false;
  motor[frontLeft]=power;
  motor[backRight]=-power;
  while(!frontSensor || !backSensor)
  {
    if(LSvalNorm(frontLight)>50)
    {
      frontSensor=true;
    }
    if (LSvalNorm(backLight)<50)
    {
      backSensor=true;
    }
    if (frontSensor)
    {
      float Pnorm=power/(float)(1+frontPos);
      motor[frontLeft]=-Pnorm*(1-frontPos);
      motor[backRight]=power;
      int side=sqrt(1+frontPos*frontPos)*cos(atan(frontPos));
      motor[backLeft]=-side;
      motor[frontRight]=side;
    }
    if (backSensor)
    {
      float Pnorm=power/(float)(1+backPos);
      motor[frontLeft]=Pnorm*(1-backPos);
      motor[backRight]=-power;
      int side=sqrt(1+backPos*backPos)*cos(atan(backPos));
      motor[backLeft]=side;
      motor[frontRight]=-side;
    }
  }
  moveForwardsToEOPD(power);

}
void alignOnLineRight(int power)
{
  LSsetActive(frontLight);
  LSsetActive(backLight);
  bool frontSensor=false, backSensor=false;
  motor[frontLeft]=-power;
  motor[backRight]=power;
  while(!frontSensor || !backSensor)
  {
    if(LSvalNorm(frontLight)>50)
    {
      frontSensor=true;
    }
    if (LSvalNorm(backLight)<50)
    {
      backSensor=true;
    }
    if (frontSensor)
    {
      float Pnorm=power/(float)(1+frontPos);
      motor[frontLeft]=Pnorm*(1-frontPos);
      motor[backRight]=-power;
      int side=sqrt(1+frontPos*frontPos)*cos(atan(frontPos));
      motor[backLeft]=side;
      motor[frontRight]=-side;
    }
    if (backSensor)
    {
      float Pnorm=power/(float)(1+backPos);
      motor[frontLeft]=-Pnorm*(1-backPos);
      motor[backRight]=power;
      int side=sqrt(1+backPos*backPos)*cos(atan(backPos));
      motor[backLeft]=-side;
      motor[frontRight]=side;
    }
  }
  moveForwardsToEOPD(power);
}

#endif
