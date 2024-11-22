#include <Wire.h>
#include "MotorI2C.h"
#include "Debug.h"

MotorI2C* MotorI2C::instance = nullptr;

// Constructor to initialize with TwoWire instance, address, and motor pointers
MotorI2C::MotorI2C(TwoWire* wire, SmartMotor* leftMotor, SmartMotor* rightMotor)
  : wireInstance(wire), slaveAddress(slaveAddress), leftMotor(leftMotor), rightMotor(rightMotor),
    receivedLeftSpeed(0), receivedRightSpeed(0), newDataAvailable(false), lastCommandTime(millis()) {}

// Setup I2C communication
void MotorI2C::begin() {                                  // Initialize I2C as slave
  MotorI2C::instance = this;                              // Set static instance for callbacks
  wireInstance->onReceive(MotorI2C::receiveDataStatic);   // Set receive handler
  wireInstance->onRequest(MotorI2C::sendTelemetryStatic); // Set request handler
  leftMotor->begin();                                     // Initialize left motor
  leftMotor->calibrate();                                 // Calibrate left motor
  rightMotor->begin();                                    // Initialize right motor
  rightMotor->calibrate();                                // Calibrate right motor
}

// Function to receive data from the master
void MotorI2C::receiveData(int numBytes) {
  Debug.println("[I2C/Motor] Data received", Levels::DEBUG);      // Print received data message
  if (numBytes >= 2 * sizeof(float)) {                            // Expect two floats (leftSpeed, rightSpeed)
    float buffer[2];                                              // Buffer to store received data
    wireInstance->readBytes((uint8_t*)buffer, 2*sizeof(float));   // Read data from the master
    receivedLeftSpeed = buffer[0];                                // Extract left speed
    receivedRightSpeed = buffer[1];                               // Extract right speed
    newDataAvailable = true;
    lastCommandTime = millis();                                   // Update the time of the last received command
  }
}

// Function to send telemetry (current speeds) back to the master
void MotorI2C::sendTelemetry() {
  Debug.println("[I2C/Motor] Sending telemetry", Levels::DEBUG);  // Print telemetry message
  float buffer[2];                                     // Buffer to store telemetry data
  buffer[0] = leftMotor->getSpeed();
  buffer[1] = rightMotor->getSpeed();

  // Send back two floats (current speeds)
  wireInstance->write((uint8_t*)buffer, 2*sizeof(float));
}

// Unified function to handle motor updates and timeout in loop
void MotorI2C::updateMotors() {
  if (newDataAvailable) {
    Debug.println("[I2C/Motor] Updating speed", Levels::DEBUG);  // Print new data message
    leftMotor->setSpeed(receivedLeftSpeed);           // Update left motor speed
    rightMotor->setSpeed(receivedRightSpeed);         // Update right motor speed
    newDataAvailable = false;                         // Reset flag
  }

  unsigned long currentTime = millis();
  if (currentTime - lastCommandTime > MOTOR_TIMEOUT) {
    Debug.println("Timeout, stopping motors", Levels::WARN);    // Print timeout message
    leftMotor->stop();                           // Stop left motor on timeout
    rightMotor->stop();                          // Stop right motor on timeout
  }

  // Regularly update motors
  leftMotor->update();
  rightMotor->update();
}
