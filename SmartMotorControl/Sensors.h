#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <SparkFunTMP102.h>
#include "definitions.h"
#include "Constants.h"

TMP102 tsens;

class Sensors
{
public:
    void begin();

    float measureCurrent(byte IPROPI);
    float measureTempMotor(byte MTEMP);
    float measureTempBoard();

private:

}

#endif