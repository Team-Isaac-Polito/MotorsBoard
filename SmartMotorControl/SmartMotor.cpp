#include "SmartMotor.h"

/**
 * Create SmartMotor object, creating all necessary objects.
 * @param pwm PWM pin.
 * @param dir Direction pin.
 * @param enc_a Pin A of the encoder.
 * @param enc_b Pin B of the encoder.
 * @param invert Invert motor direction, usuful when motors are mounted opposite to one another.
 * @param pio PIO to use for the encoder. Each PIO can handle up to 4 encoders.
 */
SmartMotor::SmartMotor(byte pwm, byte dir, byte enc_a, byte enc_b, byte ipropi, byte mtemp, bool invert, PIO pio)
    : motor(pwm, dir, invert),
      encoder(enc_a, enc_b, new MovingAvgFilter<int>(ENC_TR_SAMPLES), invert, pio),
      pid(0.f, 0.f, 0.f, MAX_SPEED, 1.f),
      invert(invert),
      sensors(ipropi, mtemp)
{
}

/**
 * Initialize SmartMotor and necessary components.
 */
void SmartMotor::begin()
{
    motor.begin();
    encoder.begin();
    sensors.begin();
}

/**
 * Update routine, updating the PID and the motor speed.
 * This function will be executed at a fixed rate, defined by DT_PID, and should therefore be called as often as possible.
 */
void SmartMotor::update()
{
    unsigned long now = millis();
    if (now - pid_last > DT_PID)
    {
        pid.updateFeedback(getSpeed());
        pid.calculate();
        motor.write(speedToPower(pid.getOutput()));
        pid_last = now;

        // Sensors measurements. Warning: functions get called for each motor
        sensors.measureCurrent();
        sensors.measureTempMotor();
        sensors.measureTempBoard();

        while (sensors.getTempMotor() > 40)
        {
            motor.write(0);
            //Serial.println("Motor overheating");
        }

        // TODO:test overheating control
    }
}

// Get temperature of the motor
float SmartMotor::getTempMotor()
{
  return this->sensors.getTempMotor();
}

// Get how much current the motor is drawing
float SmartMotor::getCurrent()
{
  return this->sensors.getCurrent();
}

// Get the temperature on the board
float SmartMotor::getTempBoard()
{
  return this->sensors.getTempBoard();
}

/**
 * Set the desired speed of the motor.
 * @param value Desired motor speed between -MAX_SPEED and MAX_SPEED.
 */
void SmartMotor::setSpeed(float value)
{
    pid.updateReferenceValue(value);
}

/**
 * Get the current speed of the motor.
 * The value is only updated at a fixed rate, defined by DT_ENC, to avoid losing precision.
 * @return float Current speed of the motor between -MAX_SPEED and MAX_SPEED.
 */
float SmartMotor::getSpeed()
{
    unsigned long now = millis();
    if (now - enc_last > DT_ENC)
    {
        speed = (float)(encoder.getSpeed()) / 100.f;
        enc_last = now;
    }
    return speed;
}

/**
 * Stop the motor.
 * This function will stop the motor and reset the PID.
 */
void SmartMotor::stop()
{
    motor.write(0);
    pid.updateReferenceValue(0.f);
    pid.resetState();
}

/**
 * Calibrate the PID controller.
 * This function will set the PID parameters to values that should work for the motor.
 * The method used is based on Åström–Hägglund tuning method, while using Ziegler-Nichols formulas to compute the gains.
 * Only the Kp and Ki gains are computed while the Kd gain is set to 0 since it doesn't have a positive effect on controlling the motor.
 * @param target Target speed to use for calibration.
 */
void SmartMotor::calibrate(float target)
{
    delay(5000);
    Debug.print("STARTING CALIBRATION MOTOR ");
    Debug.println(this->motor.getID());
    float th = target + 5.f;
    float tl = target - 5.f;

    motor.write(PWM_MAX_VALUE);
    delay(2000);
    Debug.println(getSpeed());
    while (getSpeed() < th){
      Serial.println("start while");
      delay(DT_ENC);
    }
    Serial.println("end while");
    int t_high = millis();
    float val_high = getSpeed();
    motor.write(0);
    while (getSpeed() > tl)
        delay(DT_ENC);
    int t_low = millis();
    float val_low = getSpeed();

    float tu = (float)(t_low - t_high) / 1000.f;
    float amplitude = (val_high - val_low) / 2.f;
    float ku = 4.f * MAX_SPEED / (PI * amplitude);

    float ti = 0.83f * tu;

    float kp = 0.45f * ku * 0.1f;
    float ki = kp / ti;

    Debug.println("Motor calibration result: Kp " + String(kp) + ", Ki " + String(ki), Levels::INFO);

    pid.setKp(kp);
    pid.setKi(ki);
}

/**
 * Converts speed to power.
 * This function linearly scales the speed to the PWM duty cycle, without taking into account the motor's.
 * @param speed Theretical speed of the motor.
 * @return int PWM value to set the motor to.
 */
int SmartMotor::speedToPower(float speed)
{
    return (speed / MAX_SPEED) * PWM_MAX_VALUE;
}
