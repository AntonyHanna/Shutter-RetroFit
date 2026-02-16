#include <Arduino.h>
#include <Wire.h>
#include <String>
#include <EEPROM.h>
#include "../lib/PWMHelper/PWMHelper.h"

#define I2C_SCL 22
#define I2C_SDA 21
#define AS5600_ADDR 0x36

#define GEAR_RATIO_IO 50.0 / 1.0

// PCB PIN OUTS
#define P_STATUS 18
#define P_ERROR 19
#define P_POWER 23

#define P_UP 27
#define P_DOWN 32

#define EXP_IO 33




// PID related
float calculate_PID();
float calculate_proportion(float current, float destination);
float calculate_integral();
float calculate_derivative(); 

float abs_pos = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
 // GET_ENCODER_STATUS(buffer);

  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(26, LOW);

  digitalWrite(14, HIGH);
  digitalWrite(13, HIGH);


 // VALIDATE_ENCODER_STATUS(buffer);
}

void loop() {



  // read status of panel buttons (interrupt)
  // update panel lights
  // flip between L and R control

  // float angle = read_angle();
  // float deviation = calculate_degrees_deviation(angle);


  // Serial.print(deviation, 2);
  // Serial.print(" + ");
  // Serial.print(abs_pos, 2);
  // Serial.print(" = ");
  
  // abs_pos += deviation;
  
  // Serial.println(abs_pos, 2);

  // should calculate total degree of rotation
  
}
  
