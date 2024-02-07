#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <SparkFunTMP102.h>
#include "definitions.h"
#include "Constants.h"

class Sensors
{
public:
    Sensors(byte ipropi, byte mtemp);
    void begin();

    void measureCurrent();
    void measureTempMotor();
    void measureTempBoard();

    float getCurrent();
    float getTempMotor();
    float getTempBoard();

private:
TMP102 tsens;
float current;
float tempmotor;
float tempboard;

byte ipropi, mtemp;

};

#endif