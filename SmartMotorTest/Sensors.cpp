#include "Sensors.h"

bool Sensors::begin()
{
  if (!tsens.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    return false;
  } else{
    return true;
  }
}

/*From the example for the SparkFunTMP102 sensor*/
void Sensors::measureTempBoard()
{
  this->tempboard = tsens.readTempC();
}

float Sensors::getTempBoard()
{
  return this->tempboard;
}