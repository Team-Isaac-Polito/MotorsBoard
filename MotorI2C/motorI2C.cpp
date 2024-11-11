#include <Wire.h>
#include "MotorI2C.h"

// Constructor to initialize with TwoWire instance, address, and motor pointers
MotorI2C::MotorI2C(TwoWire& wire, int slaveAddress, SmartMotor* leftMotor, SmartMotor* rightMotor)
  : wireInstance(wire), slaveAddress(slaveAddress), leftMotor(leftMotor), rightMotor(rightMotor),
    receivedLeftSpeed(0), receivedRightSpeed(0), newDataAvailable(false), lastCommandTime(0) {}

// Setup I2C communication
void MotorI2C::begin() {
  wireInstance.begin(slaveAddress);                 // Initialize I2C as slave
  wireInstance.onReceive([this](int numBytes) { receiveData(numBytes); });  // Set receive handler with lambda
  wireInstance.onRequest([this]() { sendTelemetry(); });                    // Set request handler with lambda
  leftMotor->begin();                              // Initialize left motor
  leftMotor->calibrate();                          // Calibrate left motor
  rightMotor->begin();                             // Initialize right motor
  rightMotor->calibrate();                         // Calibrate right motor
}

// Function to receive data from the master
void MotorI2C::receiveData(int numBytes) {
  if (numBytes >= 2 * sizeof(float)) {               // Expect two floats (leftSpeed, rightSpeed)
    wireInstance.readBytes((char*)&receivedLeftSpeed, sizeof(float));  // Read left motor speed
    wireInstance.readBytes((char*)&receivedRightSpeed, sizeof(float)); // Read right motor speed
    newDataAvailable = true;
    lastCommandTime = millis();                      // Update the time of the last received command
  }
}

// Function to send telemetry (current speeds) back to the master
void MotorI2C::sendTelemetry() {
  float leftSpeed = leftMotor->getSpeed();
  float rightSpeed = rightMotor->getSpeed();

  // Send back two floats (current speeds)
  wireInstance.write((byte*)&leftSpeed, sizeof(float));
  wireInstance.write((byte*)&rightSpeed, sizeof(float));
}

// Unified function to handle motor updates and timeout in loop
void MotorI2C::updateMotors() {
  if (newDataAvailable) {
    leftMotor->setSpeed(receivedLeftSpeed);           // Update left motor speed
    rightMotor->setSpeed(receivedRightSpeed);         // Update right motor speed
    newDataAvailable = false;                         // Reset flag
  }

  unsigned long currentTime = millis();
  if (currentTime - lastCommandTime > MOTOR_TIMEOUT) {
    leftMotor->setSpeed(0);                           // Stop left motor on timeout
    rightMotor->setSpeed(0);                          // Stop right motor on timeout
  }

  // Regularly update motors
  leftMotor->update();
  rightMotor->update();
}
