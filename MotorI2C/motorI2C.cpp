#include <Wire.h>
#include "MotorI2C.h"
#include "SmartMotor.h"

// Create a custom I2C instance for the Pico (as slave)
TwoWire I2C_Slave = TwoWire(0);  // Use bus 0 for the I2C

MotorI2C::MotorI2C(TwoWire& wire, int slaveAddress) 
  : wireInstance(wire), slaveAddress(slaveAddress), receivedLeftSpeed(0), receivedRightSpeed(0), newDataAvailable(false), lastCommandTime(0) {}

// Setup I2C communication
void MotorI2C::begin() {
  wireInstance.begin(slaveAddress);  // Initialize the I2C as a slave
  wireInstance.onReceive(receiveData);  // Set the receive function to handle incoming data
  wireInstance.onRequest(sendTelemetry); // Set the request function to handle telemetry data
}

// Function to receive data from the master
void MotorI2C::receiveData(int numBytes) {
  if (numBytes >= 2 * sizeof(float)) {  // Expect two floats (leftSpeed, rightSpeed)
    wireInstance.readBytes((char*)&receivedLeftSpeed, sizeof(float));  // Read left motor speed
    wireInstance.readBytes((char*)&receivedRightSpeed, sizeof(float)); // Read right motor speed

    newDataAvailable = true;
    lastCommandTime = millis();  // Update the time of the last received command
  }
}

// Function to send telemetry (current speeds) back to the master
void MotorI2C::sendTelemetry() {
  float leftSpeed = receivedLeftSpeed;
  float rightSpeed = receivedRightSpeed;

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
void MotorI2C::checkSafety(SmartMotor* leftMotor, SmartMotor* rightMotor) {
  unsigned long currentTime = millis();
  if (currentTime - lastCommandTime > MOTOR_TIMEOUT) {
    leftMotor->stop();
    rightMotor->stop();
  }
}
