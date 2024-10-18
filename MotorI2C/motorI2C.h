#ifndef MotorI2C_h
#define MotorI2C_h

#include <Wire.h>
#include "SmartMotor.h"

#define MOTOR_TIMEOUT 2000  // Timeout in milliseconds to stop motors if no commands are received

class MotorI2C {
  public:
    MotorI2C(int slaveAddress);
    void setupI2C(SmartMotor* leftMotor, SmartMotor* rightMotor); // Setup I2C and initialize motors
    void receiveData(int numBytes);        // Receive speed data from the main board
    void sendTelemetry();                 // Send telemetry (speed) data back to the main board
    void loopUpdate();                    // Regular update function to be called in the main loop

  private:
    int _slaveAddress;
    SmartMotor* _leftMotor;               // Pointer to left motor
    SmartMotor* _rightMotor;              // Pointer to right motor
    bool _newDataAvailable;               // Flag to indicate new data has been received
    unsigned long _lastCommandTime;       // Time of the last received command
};

#endif