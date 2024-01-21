#include "SmartMotor.h"
#include "definitions.h"

SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, false, pio1);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, true, pio1);
// Motor m1(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR);
// Motor m2(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR);
// TractionEncoder e1(ENC_TR_LEFT_A, ENC_TR_LEFT_B, new MovingAvgFilter<int>(ENC_TR_SAMPLES));
// TractionEncoder e2(ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, new MovingAvgFilter<int>(ENC_TR_SAMPLES), true);
// PID pid1(0.f, 0.f, 0.f, MAX_SPEED, 1.f);
// PID pid2(0.f, 0.f, 0.f, MAX_SPEED, 1.f);

void measureCurrent() {

  float current1, current2, current1_tot, current2_tot;
  int AVG = 50;

  for (int i = 0; i < AVG; i++) {
    current1_tot += analogRead(IPROPI1) * (3.3f / 1024) / (0.0015f * 910.f);
    current2_tot += analogRead(IPROPI2) * (3.3f / 1024) / (0.0015f * 910.f);
    delay(10);
  }

  current1 = current1_tot / AVG;
  current2 = current2_tot / AVG;
  //Serial.print("IPROPI: ");
  //Serial.println(analogRead(IPROPI2));
  Serial.print("Current 1: ");
  Serial.print(current1);
  Serial.print("A, Current 2: ");
  Serial.print(current2);
  Serial.println("A");
}

void setup() {
  Serial.begin(9600);
  //Debug.setLevel(Levels::DEBUG);  // comment to set debug verbosity to debug

  // initializing PWM
  analogWriteFreq(PWM_FREQUENCY);   // switching frequency to 15kHz
  analogWriteRange(PWM_MAX_VALUE);  // analogWrite range from 0 to 512, default is 255

  // motor initialization
  motorTrLeft.begin();
  motorTrRight.begin();

  motorTrLeft.calibrate();
  motorTrRight.calibrate();

  // m1.begin();
  // m2.begin();

  // e1.begin();
  // e2.begin();

  //Debug.println("BEGIN", Levels::INFO);
}

void delayCurrent(int ms) {
  int t = millis();
  while (millis() - t < ms) {
    measureCurrent();
    printEnc();
    delay(100);
  }
}

void printEnc(){
  Serial.print("speed e1: ");
  Serial.println(e1.getSpeed());
  Serial.print("speed e2: ");
  Serial.println(e2.getSpeed());
}

void loop() {

  // update motors
  motorTrLeft.update();
  motorTrRight.update();
  Serial.println("Motors updated");

  motorTrLeft.setSpeed(65);
  motorTrRight.setSpeed(30);

  Debug.print("TRACTION DATA :  \tleft: \t");
  Debug.print(motorTrLeft.getSpeed());
  Debug.print("\tright: \t");
  Debug.println(motorTrRight.getSpeed());

  // m1.write(200);
  // m2.write(100);
  

  delayCurrent(4000);

  // Debug.println("Stopping motors.", Levels::INFO);
  motorTrLeft.stop();
  motorTrRight.stop();

  delayCurrent(1000);
}
