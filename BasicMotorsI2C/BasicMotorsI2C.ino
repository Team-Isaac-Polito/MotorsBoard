#include <Wire.h>
#include "Motor.h"
#include "pin_definitions.h"

// Define Slave I2C Address
#define SLAVE_ADDR 48

Motor m1(DRV_IN11, DRV_IN21);
Motor m2(DRV_IN12, DRV_IN22);

short int m1val = 0; // -1024,1024 value for motor pwm control
short int m2val = 0;
bool newRxData = false;
int receivedbytes = 0;

void setup()
{
    Serial.begin(115200);

    // I2C Pins going to the MotorBoard
    Wire.setSDA(8);
    Wire.setSCL(9);
    // Initialize I2C communications as Slave
    Wire.begin(SLAVE_ADDR);

    // Function to run when data requested from master
    // Wire.onRequest(requestEvent);

    // Function to run when data received from master
    Wire.onReceive(receiveEvent);

    m1.begin();
    m2.begin();

    Serial.println("Ready!");
}

void receiveEvent(int numBytesReceived)
{
    // --- prints are not recommended here ---
    receivedbytes = Wire.available();;
    if (newRxData == false)
    {

        if(Wire.available() == 4) {m1val = Wire.read(); //Read MSB
        // Serial.print("4: ");
        // Serial.println(m1val);
        }
        if(Wire.available() == 3) {m1val = (m1val << 8) | Wire.read(); //Shift left and make OR to join LSB with MSB
        // Serial.print("3: ");
        // Serial.println(m1val);
        }
        if(Wire.available() == 2) {m2val = Wire.read();
        // Serial.print("2: ");
        // Serial.println(m2val);
        }
        if(Wire.available() == 1) {m2val = (m2val << 8) | Wire.read();
        // Serial.print("1: ");
        // Serial.println(m2val);
        }
        
        newRxData = true;
    }
    else
    {
        // dump the data
        /* while (Wire.available() > 0)
        {
            byte c = Wire.read();
        } */
    }
}

void loop()
{
    // Serial.println(newRxData);
    //  ------ loop for i2c test ------
    if (newRxData == true)
    {
        Serial.println("Receive event");
        // Command to motors
        m1.write(m1val);
        m2.write(m2val);
        newRxData = false;

        // Debug
        Serial.print("m1val: ");
        Serial.println(m1val);
        Serial.print("m2val: ");
        Serial.println(m2val);

        Serial.print("Bytes available: ");
        Serial.println(receivedbytes);
    }
    else
    {
        Serial.println("No data received");
        receivedbytes = 0;

        Serial.print("Bytes available: ");
        Serial.println(receivedbytes);
    }

    delay(100);
}