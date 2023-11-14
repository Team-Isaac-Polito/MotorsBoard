#include <Wire.h>
#include <SparkFunTMP102.h>
#include "pin_definitions.h"
#include "Motor.h"
#include "Constants.h"

TMP102 tsens;
Motor m1(DRV_IN11, DRV_IN21);
Motor m2(DRV_IN12, DRV_IN22);

const int slaveAddress = 8;
int m1val = 0; // 0-255 value for motor pwm control
int m2val = 0;
bool newRxData = false;

void setup() {
  Serial.begin(115200);

  Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);
  Wire.begin();

  // Wire.setSDA(SDA2);
  // Wire.setSCL(SCL2);
  // Wire.begin(slaveAddress);
  // Wire.onReceive(receiveEvent);

  if(!tsens.begin())
  {
    Serial.println("Cannot connect to TMP102.");
    Serial.println("Is the board connected? Is the device ID correct?");
    while(1);
  }

  m1.begin();
  m2.begin();
}

void receiveEvent(int numBytesReceived) {
  Serial.print("Bytes available: ");
  Serial.println(Wire.available());
  if (newRxData == false) {
        // copy the data to rxData
        Wire.readBytes( (byte*) &m2val, numBytesReceived);
        Serial.print("m2val received: ");
        Serial.println(m2val);
        Serial.print("numbytes: ");
        Serial.println(numBytesReceived);
        newRxData = true;
    }
    else {
            // dump the data
        while(Wire.available() > 0) {
            byte c = Wire.read();
        }
    }
}

void measureCurrent() {
  float current1 = analogRead(IPROPI1) * (3.3f / 1024) * (1500.f/910.f);
  float current2 = analogRead(IPROPI2) * (3.3f / 1024) * (1500.f/910.f);
  //Serial.print("IPROPI: ");
  //Serial.println(analogRead(IPROPI2));
  Serial.print("Current 1: ");
  Serial.print(current1);
  Serial.print("A, Current 2: ");
  Serial.print(current2);
  Serial.println("A");
}

void measureTempMotor(byte MTEMP){
  
  float Vm = analogRead(MTEMP) * VREF/1024;
  //Serial.println(Vm);
  float R = R2 * (VREF/Vm - 1); // Resistance of the NTC
  //Serial.print("Resistance: ")
  //Serial.print(R);
  //Serial.println("Ohms");
  float temperature = BETA/(log(R/R0) + BETA/T0);
   // Linearizing this function makes the adc read a lower temperature
   // than the real one when it rises over 70°C
   Serial.print("Motor Temp: ");
   Serial.print(temperature - 273.15);
   Serial.println("°C");
}

void measureTempBoard() {
  float temperature = tsens.readTempC();
  Serial.print("Board Temp: ");
  Serial.print(temperature);
  Serial.println("°C");
}

void delayCurrent(int ms) {
  int t = millis();
  while(millis() - t < ms) {
    measureCurrent();
    measureTempBoard();
    //measureTempMotor(MTEMP1);
    measureTempMotor(MTEMP2);
    delay(100);
  }
}

void loop() {
// ------ loop for i2c test ------
//  if (newRxData == true) {
//         m1.write(m1val);
//         m2.write(m2val);
//         newRxData = false;
//     }
//   Serial.print("m2val: ");
//   Serial.println(m2val);
//   delayCurrent(50);

// ------ loop for motor and temp testing --------
  for(int i = 50; i<240; i+=10){
    m2.write(i);
    m1.write(i);
    // float current2 = analogRead(IPROPI2) * (3.3f / 1024) * (1500.f/910.f);
    // Serial.println(current2);
    delayCurrent(50);
  }
  delayCurrent(500);
  for(int i = 240; i>50; i-=10){
    m2.write(i);
    m1.write(i);
    delayCurrent(50);
  }
  
}
