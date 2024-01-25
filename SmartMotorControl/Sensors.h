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
    void measureCurrent();
    void measureTempBoard();
    void measureTempMotor(byte MTEMP);

private:
}

#endif