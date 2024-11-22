#include <Arduino.h>
#include <Wire.h>
#include "MotorI2C.h"
#include "SmartMotor.h"
#include "pin_definitions.h"

// Define motors and the I2C instance
SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, IPROPI1, MTEMP1, false);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, IPROPI2, MTEMP2, true);

MotorI2C motorI2C(&Wire, &motorTrLeft, &motorTrRight);  // Pass custom TwoWire instance and I2C address

void setup() {
  Wire.begin(0x08);  // Initialize the I2C bus as slave
  motorI2C.begin();  // Initialize the communication
}

void loop() {
  motorI2C.updateMotors();  // Update motors and check for timeout
  delay(10);               // Small delay to avoid busy loop
}
