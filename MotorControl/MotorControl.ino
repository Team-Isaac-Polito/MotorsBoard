#include <Wire.h>
#include <SparkFunTMP102.h>
#include "pin_definitions.h"
#include "Motor.h"


TMP102 tsens;
Motor m1(DRV_IN11, DRV_IN21);
Motor m2(DRV_IN12, DRV_IN22);

void measureCurrent() {
  float current1 = analogRead(IPROPI1) * (3.3f / 1024) / (0.0015f * 910.f);
  float current2 = analogRead(IPROPI2) * (3.3f / 1024) / (0.0015f * 910.f);
  Serial.print("Current: ");
  Serial.print(current1);
  Serial.print(" A, Current 2: ");
  Serial.print(current2);
  Serial.println(" A");
}

void setup() {
  Serial.begin(115200);

  Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);
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

void measureTemperature() {
  float temperature = tsens.readTempC();
  Serial.print("Temperature: ");
  Serial.println(temperature);
}

void delayCurrent(int ms) {
  int t = millis();
  while(millis() - t < ms) {
    measureCurrent();
    measureTemperature();
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
