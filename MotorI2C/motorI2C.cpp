#include "MotorI2C.h"

MotorI2C::MotorI2C(int slaveAddress) {
  _slaveAddress = slaveAddress;
  _leftSpeed = 0.0;
  _rightSpeed = 0.0;
  _speedL = 0.0;
  _speedR = 0.0;
}

// Setup I2C communication
void MotorI2C::setupI2C() {
  Wire.begin(_slaveAddress);  // Initialize the I2C as a slave
  Wire.onReceive(receiveData);  // Set the receive function to handle incoming data
  Wire.onRequest(sendTelemetry); // Set the request function to handle telemetry data
}

// Function to receive data from the main board
void MotorI2C::receiveData(int numBytes) {
  if (Wire.available() >= 2 * sizeof(float)) {  // Expect two floats (leftSpeed, rightSpeed)
    _leftSpeed = Wire.read(); // Read left motor speed
    _rightSpeed = Wire.read(); // Read right motor speed
    
    // Now set the motor speeds accordingly
    setMotorSpeeds(_leftSpeed, _rightSpeed);
  }
}

// Function to send telemetry (current speeds) back to the main board
void MotorI2C::sendTelemetry() {
  _speedL = getMotorSpeedL();
  _speedR = getMotorSpeedR();
  
  // Send back two floats (current speeds)
  Wire.write((byte*)&_speedL, sizeof(float));
  Wire.write((byte*)&_speedR, sizeof(float));
}

// Set the speed of both motors
void MotorI2C::setMotorSpeeds(float leftSpeed, float rightSpeed) {
  // Logic to control the motors goes here
  // e.g., motorTrLeft.setSpeed(leftSpeed);
  // e.g., motorTrRight.setSpeed(rightSpeed);
}

// Get the left motor speed (telemetry)
float MotorI2C::getMotorSpeedL() {
  // Logic to retrieve the actual left motor speed
  return _leftSpeed;  // Placeholder, replace with actual telemetry data
}

// Get the right motor speed (telemetry)
float MotorI2C::getMotorSpeedR() {
  // Logic to retrieve the actual right motor speed
  return _rightSpeed;  // Placeholder, replace with actual telemetry data
}