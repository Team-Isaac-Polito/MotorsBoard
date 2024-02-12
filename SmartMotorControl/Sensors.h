#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <SparkFunTMP102.h>
#include "definitions.h"
#include "Constants.h"

class Sensors
{
public:
    bool begin();

    void measureTempBoard();

    float getTempBoard();

private:
TMP102 tsens;
float tempboard;


};

#endif