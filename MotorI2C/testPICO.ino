#include <Wire.h>
#include "MotorI2C.h"
#include "SmartMotor.h"

// Define motors and the I2C instance
SmartMotor leftMotor;
SmartMotor rightMotor;
TwoWire I2C_Slave = TwoWire(0);  // Custom TwoWire instance on bus 0
MotorI2C motorI2C(I2C_Slave, 0x08);  // Pass custom TwoWire instance and I2C address

void setup() {
  I2C_Slave.begin(0x08);  // Initialize the I2C bus as slave
  motorI2C.begin();  // Initialize the communication
}

void loop() {
  float leftSpeed, rightSpeed;

  // Check if there are new data available
  if (motorI2C.isAvailable(&leftSpeed, &rightSpeed)) {
    // Update the motors with the new speeds
    leftMotor.setSpeed(leftSpeed);
    rightMotor.setSpeed(rightSpeed);
  }

  // Regularly update motors
  leftMotor.update();
  rightMotor.update();

  // Check if the motors need to be stopped due to timeout
  motorI2C.checkSafety(&leftMotor, &rightMotor);
}
