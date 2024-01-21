# MotorsBoard
This is the code for the first PCB developed by team Isaac to drive the traction motors of Rese.Q MK2. It is built around RP2040 and can control up to two DC motors. More informations [here](https://docs.teamisaac.it/doc/brushed-motor-board-hYdcsDyAAj)

**MotorControl** is the sketch used to test basic motor control, TMP102 temperature sensor on board, thermistor (NTC) temperature sensor on the motor and current feedback.
 
**SmartMotorControl** is a subset of PicoLowLevel repository (used for Picos in MK1) used to test advanced motor control with Encoder feedback and PID.
