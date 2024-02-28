#include "SmartMotor.h"
#include "Sensors.h"

// Define Slave I2C Address
#define SLAVE_ADDR 48

SmartMotor motorTrLeft(DRV_TR_LEFT_PWM, DRV_TR_LEFT_DIR, ENC_TR_LEFT_A, ENC_TR_LEFT_B, IPROPI1, MTEMP1, false);
SmartMotor motorTrRight(DRV_TR_RIGHT_PWM, DRV_TR_RIGHT_DIR, ENC_TR_RIGHT_A, ENC_TR_RIGHT_B, IPROPI2, MTEMP2, true);

unsigned long prevt = 0;
Sensors sensors;

void setup()
{
  //Initializing I2C communication with tmp102 on board sensors
  Wire.setSDA(SDA1);
  Wire.setSCL(SCL1);
  Wire.begin();

  //Initializing I2C communication with Main board
  //Wire1.setSDA(8);
  //Wire1.setSCL(9);
  // Initialize I2C communications as Slave
  //Wire1.begin(SLAVE_ADDR);

  // Function to run when data requested from master
 // Wire1.onRequest(requestEvent); 
  
  // Function to run when data received from master
 // Wire1.onReceive(receiveEvent);

  Serial.begin(9600);
  Serial.println("Start setup");
  Debug.setLevel(Levels::DEBUG); // comment to set debug verbosity to debug

  // initializing PWM
  analogWriteFreq(PWM_FREQUENCY);  // switching frequency to 15kHz
  analogWriteRange(PWM_MAX_VALUE); // analogWrite range from 0 to 512, default is 255

  //Sensors on board initialization
  if(!sensors.begin()){
    Debug.println("TMP102 initilization problem", Levels::WARN);
  }

  // motor initialization
  motorTrLeft.begin();
  motorTrRight.begin();

  // Calibration needs motor connected to work
  motorTrLeft.calibrate();
  motorTrRight.calibrate();


  Debug.println("BEGIN", Levels::INFO);
}

void loop()
{
  int t = millis();

  // update motors
  motorTrLeft.update();
  motorTrRight.update();

  // TODO: speed should be sent from the core managing i2c to the core managing pid
  motorTrLeft.setSpeed(1024);
  motorTrRight.setSpeed(-65);

  // Condition to avoid the use of delay()
  if (t - prevt > 10)
  {
    Debug.print("TRACTION DATA :  \tleft: \t");
    Debug.print(motorTrLeft.getSpeed());
    Debug.print("\tright: \t");
    Debug.println(motorTrRight.getSpeed());
    Debug.print("CURRENTS DATA :  \tleft: \t");
    Debug.print(motorTrLeft.getCurrent());
    Debug.print("\tright: \t");
    Debug.println(motorTrRight.getCurrent());
    Debug.print("MOTOR TEMP :  \tleft: \t");
    Debug.print(motorTrLeft.getTemperature());
    Debug.print("\tright: \t");
    Debug.println(motorTrRight.getTemperature());
    sensors.measureTempBoard();
    Debug.print("BOARD TEMP : \t");
    Debug.print(sensors.getTempBoard());
    Debug.println(" °C");
    t = prevt;
  }

  // Debug.println("Stopping motors.", Levels::INFO);
  // motorTrLeft.stop();
  // motorTrRight.stop();

}

/* 
void setup1()
{
}

void loop1()
{
} 
*/
