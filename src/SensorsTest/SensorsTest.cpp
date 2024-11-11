/* MotorsBoard code to test motors, current feedback, 
temperature sensors on board and on the motor*/

#include <Arduino.h>
#include <Wire.h>
#include <SparkFunTMP102.h>
#include <Motor.h>
#include "pin_definitions.h"
#include "Constants.h"


TMP102 tsens;
Motor m1(DRV_IN11, DRV_IN21);
Motor m2(DRV_IN12, DRV_IN22);

/* Current feedback is evaluated from the measurement of voltage on 
the IPROPI resistor, formula is from the driver datasheet.
It only works when 12V is supplied to the board.*/
void measureCurrent() {
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
void measureTempMotor(byte MTEMP){
  float Vm = analogRead(MTEMP) * VREF/1024;
  float R = R2 * (VREF/Vm - 1); // Resistance of the NTC
  float temperature = BETA/(log(R/R0) + BETA/T0);
   // Linearizing this function makes the adc read a lower temperature
   // than the real one when it rises over 70°C
   Serial.print("Motor Temp: ");
   Serial.print(temperature - 273.15);
   Serial.println("°C");
}

void setup() {
  Serial.begin(115200);

  /*Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);*/
  Wire.begin();

  if(!tsens.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    while(1);
  }

  m1.begin();
  m2.begin();
}

/*From the example for the SparkFunTMP102 sensor*/
void measureTempBoard() {
  float temperature = tsens.readTempC();
  Serial.print("Temperature on the board: ");
  Serial.println(temperature);
}

/*Not for motor operation with PID, use if(millis() - t < ms){}
in the loop() instead to avoid blocking motor execution*/
void delayCurrent(int ms) {
  int t = millis();
  while(millis() - t < ms) {
    measureCurrent();
    measureTempBoard();
    measureTempMotor(MTEMP1);
    measureTempMotor(MTEMP2);
    delay(100);
  }
}

void loop() {
  m1.write(255);
  m2.write(255);
  delayCurrent(5000);
  
  m1.write(-255);
  m2.write(-255);
  delayCurrent(5000);
}
