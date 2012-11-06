#pragma config(Sensor, S3,  EOPD,                  sensorI2CCustom)

#include "drivers/HTEOPD-driver.h"

task main()
{
  while(true)
  {
    if (nNxtButtonPressed==kLeftButton)
    {
      HTEOPDsetShortRange(EOPD);
    }
    else if (nNxtButtonPressed==kRightButton)
    {
      HTEOPDsetLongRange(EOPD);
    }
    nxtDisplayTextLine(2, "%d", HTEOPDreadRaw(EOPD));
    nxtDisplayTextLine(3, "%d", HTEOPDreadProcessed(EOPD));
  }
}
