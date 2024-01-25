#include "Sensors.h"

void Sensors::begin()
{
  Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);
  Wire.begin();

  if (!tsens.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    //while (1);
  }
}

/* Current feedback is evaluated from the measurement of voltage on
the IPROPI resistor, formula is from the driver datasheet.
It only works when 12V is supplied to the board.*/
float Sensors::measureCurrent(byte IPROPI)
{
  float current = analogRead(IPROPI) * (3.3f / 1024) / (0.0015f * 910.f);
  Serial.print("Current: ");
  Serial.print(current);
  Serial.print(" A");
  // TODO: add moving avg filter

  return current;
}

/* Temperature on the motor is given by the thermistor NTC formula.
It only works when 12V is supplied since VREF relies on it.*/
float Sensors::measureTempMotor(byte MTEMP)
{
  float Vm = analogRead(MTEMP) * VREF / 1024;
  float R = R2 * (VREF / Vm - 1); // Resistance of the NTC
  float temperature = BETA / (log(R / R0) + BETA / T0);
  // Linearizing this function makes the adc read a lower temperature
  // than the real one when it rises over 70°C
  Serial.print("Motor Temp: ");
  Serial.print(temperature - 273.15);
  Serial.println("°C");

  return temperature - 273.15;
}

/*From the example for the SparkFunTMP102 sensor*/
float Sensors::measureTempBoard()
{
  float temperature = tsens.readTempC();
  Serial.print("Temperature on the board: ");
  Serial.println(temperature);

  return temperature;
}