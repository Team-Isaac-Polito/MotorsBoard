#ifndef MotorI2C_h
#define MotorI2C_h

#include <Wire.h>
#include "SmartMotor.h"

#define MOTOR_TIMEOUT 2000  // Timeout in milliseconds to stop motors if no commands are received

class MotorI2C {
  public:
    MotorI2C(TwoWire& wire, int slaveAddress, SmartMotor* leftMotor, SmartMotor* rightMotor);
    void begin();                        // Initialize I2C
    void receiveData(int numBytes);      // Receive speed data from the main board
    void sendTelemetry();                // Send telemetry (speed) data back to the main board
    bool isAvailable(float* leftSpeed, float* rightSpeed);  // Check if new data is available
    void checkSafety();                  // Check for motor timeout

  private:
    TwoWire& wireInstance;
    int slaveAddress;
    SmartMotor* leftMotor;               // Pointer to the left motor
    SmartMotor* rightMotor;              // Pointer to the right motor
    float receivedLeftSpeed;             // Received left motor speed
    float receivedRightSpeed;            // Received right motor speed
    bool newDataAvailable;               // Flag to indicate new data has been received
    unsigned long lastCommandTime;       // Time of the last received command
};

#endif
