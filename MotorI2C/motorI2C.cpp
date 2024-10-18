#include "MotorI2C.h"
#include "SmartMotor.h"

MotorI2C::MotorI2C(int slaveAddress) {
  _slaveAddress = slaveAddress;
  _leftMotor = nullptr;  // Pointer to SmartMotor instance for left motor
  _rightMotor = nullptr; // Pointer to SmartMotor instance for right motor
  _newDataAvailable = false;
  _lastCommandTime = 0;
}

// Setup I2C communication and initialize motors
void MotorI2C::setupI2C(SmartMotor* leftMotor, SmartMotor* rightMotor) {
  _leftMotor = leftMotor;
  _rightMotor = rightMotor;
  Wire.begin(_slaveAddress);  // Initialize the I2C as a slave
  Wire.onReceive(receiveData);  // Set the receive function to handle incoming data
  Wire.onRequest(sendTelemetry); // Set the request function to handle telemetry data
}

// Function to receive data from the main board
void MotorI2C::receiveData(int numBytes) {
  if (numBytes >= 2 * sizeof(float)) {  // Expect two floats (leftSpeed, rightSpeed)
    float leftSpeed, rightSpeed;
    Wire.readBytes((char*)&leftSpeed, sizeof(float));  // Read left motor speed
    Wire.readBytes((char*)&rightSpeed, sizeof(float)); // Read right motor speed

    // Store the speeds and mark new data as available
    _leftMotor->setSpeed(leftSpeed);
    _rightMotor->setSpeed(rightSpeed);
    _newDataAvailable = true;
    _lastCommandTime = millis();  // Update the time of the last received command
  }
}

// Function to send telemetry (current speeds) back to the main board
void MotorI2C::sendTelemetry() {
  float leftSpeed = _leftMotor->getSpeed();
  float rightSpeed = _rightMotor->getSpeed();

  // Send back two floats (current speeds)
  Wire.write((byte*)&leftSpeed, sizeof(float));
  Wire.write((byte*)&rightSpeed, sizeof(float));
}

// Function to be called in the main loop for regular updates
void MotorI2C::loopUpdate() {
  // If new data has been received, update the motor control
  if (_newDataAvailable) {
    _leftMotor->update();
    _rightMotor->update();
    _newDataAvailable = false;
  }

  // Check if the motors should be stopped due to timeout
  unsigned long currentTime = millis();
  if (currentTime - _lastCommandTime > MOTOR_TIMEOUT) {
    _leftMotor->stop();
    _rightMotor->stop();
  }
}