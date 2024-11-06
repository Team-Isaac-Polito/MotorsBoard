#include <Wire.h>
#include "MotorI2C.h"

// Create a custom I2C instance for the Pico (as slave)
TwoWire I2C_Slave = TwoWire(0);  // Use bus 0 for the I2C

// Constructor now takes motor pointers as arguments
MotorI2C::MotorI2C(TwoWire& wire, int slaveAddress, SmartMotor* leftMotor, SmartMotor* rightMotor)
  : wireInstance(wire), slaveAddress(slaveAddress), leftMotor(leftMotor), rightMotor(rightMotor),
    receivedLeftSpeed(0), receivedRightSpeed(0), newDataAvailable(false), lastCommandTime(0) {}

// Setup I2C communication
void MotorI2C::begin() {
  wireInstance.onReceive([this](int numBytes) { receiveData(numBytes); });  // Set receive handler with lambda
  wireInstance.onRequest([this]() { sendTelemetry(); }); // Set request handler with lambda
}

// Function to receive data from the master

/*
void MotorI2C::receiveData(int numBytes) {
  if (numBytes >= 2 * sizeof(float)) {  // Expect two floats (leftSpeed, rightSpeed)
    wireInstance.readBytes((char*)&receivedLeftSpeed, sizeof(float));  // Read left motor speed
    wireInstance.readBytes((char*)&receivedRightSpeed, sizeof(float)); // Read right motor speed

    newDataAvailable = true;
    lastCommandTime = millis();  // Update the time of the last received command
  }
}
*/

// Function to send telemetry (current speeds) back to the master
void MotorI2C::sendTelemetry() {
   float leftSpeed = leftMotor->getSpeed();
  float rightSpeed = rightMotor->getSpeed();

  // Send back two floats (current speeds)
  wireInstance.write((byte*)&leftSpeed, sizeof(float));
  wireInstance.write((byte*)&rightSpeed, sizeof(float));
}

// Check if new data is available and return the motor speeds
bool MotorI2C::isAvailable(float* leftSpeed, float* rightSpeed) {
  if (newDataAvailable) {
    *leftSpeed = receivedLeftSpeed;
    *rightSpeed = receivedRightSpeed;
    newDataAvailable = false;
    return true;
  }
  return false;
}

// Function to check if motors should be stopped due to timeout
void MotorI2C::checkSafety() {
  unsigned long currentTime = millis();
  if (currentTime - lastCommandTime > MOTOR_TIMEOUT) {
    leftMotor->stop();
    rightMotor->stop();
  }
}
