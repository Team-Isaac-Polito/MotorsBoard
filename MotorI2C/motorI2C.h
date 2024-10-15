#ifndef MotorI2C_h
#define MotorI2C_h

#include <Wire.h>

class MotorI2C {
  public:
    MotorI2C(int slaveAddress);
    void setupI2C();                      // Setup I2C communication
    void receiveData(int numBytes);        // Receive speed data from the main board
    void sendTelemetry();                 // Send telemetry (speed) data back to the main board

    void setMotorSpeeds(float leftSpeed, float rightSpeed);  // Set motor speeds
    float getMotorSpeedL();               // Get left motor speed
    float getMotorSpeedR();               // Get right motor speed

  private:
    int _slaveAddress;
    float _leftSpeed;
    float _rightSpeed;
    float _speedL;  // Telemetry for left motor
    float _speedR;  // Telemetry for right motor
};

#endif