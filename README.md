# MotorsBoard
This is the code for the first PCB developed by team Isaac to drive the traction motors of Rese.Q MK2. It is built around RP2040 and can control up to two DC motors with position and current feedback. More informations [here](https://docs.teamisaac.it/doc/brushed-motor-board-hYdcsDyAAj)

## Test codes
Here is the list of working Arduino sketches that can be used to test single subsystems in order to better isolate potential bugs in production code.

- **SensorsTest** is the sketch used to test the onboard sensors, TMP102 temperature sensor on board, thermistor (NTC) temperature sensor on the motor and current feedback. It also can control the motors in order to better test the sensors. More informations about the sensors can be found in the PCB documentation.

- **EncoderTest** can be used to test the motor's Encoders output. The output speed value strongly depends on the definition of `ENC_TR_CONVERSION` in `definitions.h`

- **I2C_Test** has been used to isolate the issue of I2C communication from the MainBoard, so it correctly receives packets with motor values only. It does not have any constraint check nor error handling however. The receive ISR expects two short integers (2bytes each) with the 16bit values in the range `-1024,1024` to be given to motors.
 
- **SmartMotorTest** is a subset of PicoLowLevel repository (used for Picos in MK1, formerly SmartMotorControl) used to test advanced motor control with Encoder feedback and PID. It has only been adjusted to match the new PCB pins. especially in the `motor` class.
