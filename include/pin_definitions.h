#ifndef pin_definitions_h
#define pin_definitions_h

// I²C connected to main board
#define SDA2 8
#define SCL2 9

// I²C connected to sensors
#define SDA1 20
#define SCL1 21

// Motor 1
#define DRV_IN11 24 // driver pin IN1
#define DRV_IN21 25 // driver pin IN2
#define ENCA1 22 // encoder pin A
#define ENCB1 23 // encoder pin B
#define IPROPI1 28 // current feedback
#define MTEMP1 29 // temperature feedback

// Motor 2
#define DRV_IN12 16 // driver pin IN1
#define DRV_IN22 17 // driver pin IN2
#define ENCA2 18 // encoder pin A
#define ENCB2 19 // encoder pin B
#define IPROPI2 26 // current feedback
#define MTEMP2 27 // temperature feedback

#endif
