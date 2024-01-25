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
    while (1);
  }
}

/* Current feedback is evaluated from the measurement of voltage on
the IPROPI resistor, formula is from the driver datasheet.
It only works when 12V is supplied to the board.*/
void Sensors::measureCurrent()
{
  float current1 = analogRead(IPROPI1) * (3.3f / 1024) / (0.0015f * 910.f);
  float current2 = analogRead(IPROPI2) * (3.3f / 1024) / (0.0015f * 910.f);
  Serial.print("Current: ");
  Serial.print(current1);
  Serial.print(" A, Current 2: ");
  Serial.print(current2);
  Serial.println(" A");
}

/* Temperature on the motor is given by the thermistor NTC formula.
It only works when 12V is supplied since VREF relies on it.*/
void Sensors::measureTempBoard()
{
  float Vm = analogRead(MTEMP) * VREF / 1024;
  float R = R2 * (VREF / Vm - 1); // Resistance of the NTC
  float temperature = BETA / (log(R / R0) + BETA / T0);
  // Linearizing this function makes the adc read a lower temperature
  // than the real one when it rises over 70°C
  Serial.print("Motor Temp: ");
  Serial.print(temperature - 273.15);
  Serial.println("°C");
}

/*From the example for the SparkFunTMP102 sensor*/
void Sensors::measureTempMotor(byte MTEMP)
{
  float temperature = tsens.readTempC();
  Serial.print("Temperature on the board: ");
  Serial.println(temperature);
}