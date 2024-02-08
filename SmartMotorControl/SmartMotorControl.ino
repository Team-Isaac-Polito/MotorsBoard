#include "SmartMotor.h"

SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, IPROPI1, MTEMP1, false);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, IPROPI2, MTEMP2, true);

unsigned long prevt = 0;

void setup() {
  Serial.begin(9600);
  delay(15000);
  Serial.println("Start setup");
  Debug.setLevel(Levels::DEBUG);  // comment to set debug verbosity to debug

  // initializing PWM
  analogWriteFreq(PWM_FREQUENCY);   // switching frequency to 15kHz
  analogWriteRange(PWM_MAX_VALUE);  // analogWrite range from 0 to 512, default is 255

  // motor initialization
  motorTrLeft.begin();
  //motorTrRight.begin();

  //Calibration needs motor connected to work
  //motorTrLeft.calibrate();
  //motorTrRight.calibrate();

  delay(10000);

  Debug.println("BEGIN", Levels::INFO);
}

void loop() {
  int t = millis();

  // update motors
  motorTrLeft.update();
  motorTrRight.update();

  //TODO: speed should be sent from the core managing i2c to the core managing pid
  motorTrLeft.setSpeed(65);
  motorTrRight.setSpeed(30);


//Condition to avoid the use of delay()
  if(t - prevt > 10){
  Debug.print("TRACTION DATA :  \tleft: \t");
  Debug.print(motorTrLeft.getSpeed());
  Debug.print("\tright: \t");
  Debug.println(motorTrRight.getSpeed());
  Debug.print("CURRENTS DATA :  \tleft: \t");
  Debug.print(motorTrLeft.getCurrent());
  Debug.print("\tright: \t");
  Debug.println(motorTrRight.getCurrent());
  Debug.print("MOTOR TEMP :  \tleft: \t");
  Debug.print(motorTrLeft.getTempMotor());
  Debug.print("\tright: \t");
  Debug.println(motorTrRight.getTempMotor());
  Debug.print("BOARD TEMP : \t");
  Debug.print(motorTrLeft.getTempBoard());
  Debug.println(" °C");
  t = prevt;
  }

  // Debug.println("Stopping motors.", Levels::INFO);
  // motorTrLeft.stop();
  // motorTrRight.stop();

}