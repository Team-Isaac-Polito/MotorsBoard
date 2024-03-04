#ifndef definitions_h
#define definitions_h

// Main loop timings
#define DT_BAT 1000   // 1000ms / 1000 = 1 Hz
#define DT_PID 10     // 1000ms / 100   = 100 Hz
#define DT_TEL 10     // 1000ms / 10   = 100 Hz
#define DT_ENC 20     // 1000ms / 20   = 50 Hz

// PWM configuration
#define PWM_MAX_VALUE 1023
#define PWM_FREQUENCY 15000

// I²C connected to main board
#define SDA2 8
#define SCL2 9

// I²C connected to sensors
#define SDA1 20
#define SCL1 21

#define ABSOLUTE_ENCODER_ADDRESS 0x40 // ToDo check, maybe 0x44

// Battery configuration
#define BAT_LOW 11.1f
#define BAT_NOM 12.6f
#define BAT_PIN 28
#define BAT_R1 10000
#define BAT_R2 3300

// Motor 1 pins
#define DRV_TR_LEFT_DIR  25
#define DRV_TR_LEFT_PWM  24
#define ENC_TR_LEFT_A   22
#define ENC_TR_LEFT_B   23
#define IPROPI1 28 // current feedback
#define MTEMP1 29 // temperature feedback

//Motor 2 pins
#define DRV_TR_RIGHT_DIR  17
#define DRV_TR_RIGHT_PWM  16
#define ENC_TR_RIGHT_A  18
#define ENC_TR_RIGHT_B  19
#define IPROPI2 26 // current feedback
#define MTEMP2 27 // temperature feedback


// Motor configuration
#define MAX_SPEED 65.f

// Encoder conversion constant
// K = 100          *       10^6        *       60      / (     48   *             74,83                    *       2 )
//      centiRPM           microsecToSec        SecToMin       intPerRotation        transmissionRatio             transmissionRatio2
#define ENC_TR_CONVERSION (835226)

// Traction encoder filter samples
#define ENC_TR_SAMPLES 10

// Display
#define DISPLAY_ADDR 0x3c
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// Interface definitions
#define NMENUS 4
#define MENUTIMEOUT 18
#define BTNOK 16
#define BTNNAV 17
#define DEBOUNCE 300

// Versioning
#ifndef VERSION
  #define VERSION "testing"
#endif

// OTA configuration
#define OTA_PWD "ciaociao"
#define WIFI_SSID "iswifi"
#define WIFI_PWD "ciaociao"
#define WIFI_HOSTBASE "picow-"
#define CONF_PATH "/config.txt"

#endif
