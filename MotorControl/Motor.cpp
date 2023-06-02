#include "Motor.h"

/**
 * Create object and set motor pins.
 * @param in1 Input 1 pin.
 * @param in2 Input 2 pin.
 * @param invert Invert motor direction, usuful when motors are mounted opposite to one another.
 */
Motor::Motor(byte in1, byte in2, bool invert)
    : in1(in1), in2(in2), invert(invert)
{}

/**
 * Initialize motors.
 */
void Motor::begin() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
}

/**
 * Sets the motor speed.
 * @param value Speed of the motor, ranging from 0 to maximum PWM value.
 */
void Motor::write(int value) {
  int mot = constrain(abs(value), 0, 255);
  bool dir = invert ^ (value < 0);
  analogWrite(dir?in2:in1, mot);
  digitalWrite(dir?in1:in2, 0);
}
