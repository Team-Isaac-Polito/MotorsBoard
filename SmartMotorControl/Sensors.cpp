#include "Sensors.h"


/**
 * Create object and set motor pins.
 * @param ipropi IPROPI pin for current feedback
 * @param mtemp MTEMP pin for NTC connection
 */
Sensors::Sensors(byte ipropi, byte mtemp):
  ipropi(ipropi), mtemp(mtemp)
{}


void Sensors::begin()
{
  Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);
  Wire.begin();

  if (!tsens.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    // while (1);
  }
}

/* Current feedback is evaluated from the measurement of voltage on
the IPROPI resistor, formula is from the driver datasheet.
It only works when 12V is supplied to the board.*/
void Sensors::measureCurrent()
{
  float current, current_tot;
  int AVG = 50;

  for (int i = 0; i < AVG; i++)
  {
    current_tot += analogRead(this->ipropi) * (3.3f / 1024) / (0.0015f * 910.f);
    delay(10);
  }

  this->current = current_tot / AVG;
}

/* Temperature on the motor is given by the thermistor NTC formula.
It only works when 12V is supplied since VREF relies on it.*/
void Sensors::measureTempMotor()
{
  float Vm = analogRead(this->mtemp) * VREF / 1024;
  float R = R2 * (VREF / Vm - 1); // Resistance of the NTC
  float T = BETA / (log(R / R0) + BETA / T0);
  this->tempmotor = T - 273.15;

  // Linearizing this function makes the adc read a lower temperature
  // than the real one when it rises over 70°C
}

/*From the example for the SparkFunTMP102 sensor*/
void Sensors::measureTempBoard()
{
  this->tempboard = tsens.readTempC();
}

float Sensors::getCurrent()
{
  return this->current;
}
float Sensors::getTempMotor()
{
  return this->tempmotor;
}
float Sensors::getTempBoard()
{
  return this->tempboard;
}