#include "SmartMotor.h"

SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, false);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, true);

// void measureCurrent() {

//   float current1, current2, current1_tot, current2_tot;
//   int AVG = 50;

//   for (int i = 0; i < AVG; i++) {
//     current1_tot += analogRead(IPROPI1) * (3.3f / 1024) / (0.0015f * 910.f);
//     current2_tot += analogRead(IPROPI2) * (3.3f / 1024) / (0.0015f * 910.f);
//     delay(10);
//   }

//   current1 = current1_tot / AVG;
//   current2 = current2_tot / AVG;
//   //Serial.print("IPROPI: ");
//   //Serial.println(analogRead(IPROPI2));
//   Serial.print("Current 1: ");
//   Serial.print(current1);
//   Serial.print("A, Current 2: ");
//   Serial.print(current2);
//   Serial.println("A");
// }

void setup() {
  Serial.begin(9600);
  //Debug.setLevel(Levels::DEBUG);  // comment to set debug verbosity to debug

  // initializing PWM
  analogWriteFreq(PWM_FREQUENCY);   // switching frequency to 15kHz
  analogWriteRange(PWM_MAX_VALUE);  // analogWrite range from 0 to 512, default is 255

  // motor initialization
  motorTrLeft.begin();
  motorTrRight.begin();

  //Calibration needs motor connected to work
  //motorTrLeft.calibrate();
  //motorTrRight.calibrate();

  //Debug.println("BEGIN", Levels::INFO);
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
  if(millis() - t < 100){
    // measureCurrent();
    // printEnc();
  Debug.print("TRACTION DATA :  \tleft: \t");
  Debug.print(motorTrLeft.getSpeed());
  Debug.print("\tright: \t");
  Debug.println(motorTrRight.getSpeed());
  }

  // Debug.println("Stopping motors.", Levels::INFO);
  // motorTrLeft.stop();
  // motorTrRight.stop();

}
