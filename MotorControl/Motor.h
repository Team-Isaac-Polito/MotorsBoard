#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

/**
 * Motor class used to control simple DC motors.
 */
class Motor {
  public:
    Motor(byte in1, byte in2, bool invert = false);
    void begin();
    void write(int value);
    
  private:
    byte in1, in2;
    bool invert;
};

#endif
