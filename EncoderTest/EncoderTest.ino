#include "pin_definitions.h"
#include "Motor.h"
#include "TractionEncoder.h"

Motor m1(DRV_IN11, DRV_IN21);
Motor m2(DRV_IN12, DRV_IN22);
TractionEncoder e1(ENCA1, ENCB1);
TractionEncoder e2(ENCA2, ENCB2);

void setup() {
  Serial.begin(115200);

  m1.begin();
  m2.begin();

  e1.begin();
  e2.begin();
}

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

void delayCurrent(int ms) {
  int t = millis();
  while (millis() - t < ms) {
    measureCurrent();
    printEnc();
    delay(10);
  }
}

void printEnc() {
  Serial.print("Speed m1: ");
  Serial.println(e1.getSpeed());
  Serial.print("Speed m2: ");
  Serial.println(e2.getSpeed());
}

void loop() {
  //------ loop for encoder testing --------
  m1.write(255);
  m2.write(255);

  delayCurrent(5000);

  // m2.write(0);
  // m1.write(0);
  // delayCurrent(500);

  // for (int i = 50; i < 240; i += 10) {
  //   m2.write(i);
  //   m1.write(i);
  //   delayCurrent(20);
  // }
  // delayCurrent(500);


  // for(int i = 240; i>50; i-=10){
  //   m2.write(i);
  //   m1.write(i);
  //   delayCurrent(50);
  // }
}
