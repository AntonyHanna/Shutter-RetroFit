#include <Arduino.h>
#include <Wire.h>
#include <String>
#include <EEPROM.h>

#define i2c_scl 22
#define i2c_sda 21

#define AS5600_ADDR 0x36
#define STATUS_REG 0x0B
#define RAW_ANGLE_REG 0x0E
#define READ_LEN 1
#define GEAR_RATIO_IO 50.0 / 1.0
void GET_ENCODER_STATUS(byte* buffer);
bool VALIDATE_ENCODER_STATUS(byte *buffer);
int read_angle();
float calculate_degrees_deviation(float current_position);

float abs_pos = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();
 // GET_ENCODER_STATUS(buffer);

 // VALIDATE_ENCODER_STATUS(buffer);
}

void loop() {
  float angle = read_angle();
  float deviation = calculate_degrees_deviation(angle);


  Serial.print(deviation, 2);
  Serial.print(" + ");
  Serial.print(abs_pos, 2);
  Serial.print(" = ");
  
  abs_pos += deviation;
  
  Serial.println(abs_pos, 2);

  // should calculate total degree of rotation
  
}

void GET_ENCODER_STATUS(byte* buffer)
{
  Wire.beginTransmission(AS5600_ADDR);
  // Move to STATUS register?
  Wire.write(STATUS_REG);

  Wire.endTransmission();

  Wire.requestFrom(AS5600_ADDR, 1);
  Wire.readBytes(buffer, READ_LEN);

  for (int i = 0; i < READ_LEN; i++)
  {
    Serial.print("Read from register: ");
    Serial.println(buffer[i], BIN);
  }
}

int read_angle()
{
  const float deg_per_res = 360.0 / 4096.0;
  float degrees = 0;
  uint16_t highByte = 0;
  uint16_t lowByte = 0;

  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0E);
  Wire.endTransmission();

  Wire.requestFrom(AS5600_ADDR, 1);

  while(Wire.available() == 0);

  highByte = Wire.read();

  Wire.beginTransmission(AS5600_ADDR);
  Wire.write(0x0F);
  Wire.endTransmission();

  Wire.requestFrom(AS5600_ADDR, 1);

  while(Wire.available() == 0);

  lowByte = Wire.read();

  highByte <<= 8;

  degrees = highByte | lowByte; 
/*
  Serial.print(deg_per_res, 8);
  Serial.print(" * ");
  Serial.print(degrees);
  Serial.print(" = ");
  Serial.println(degrees * deg_per_res);
*/
  return degrees;
}

float calculate_degrees_deviation(float current_position)
{
  // read last angle from eeprom
  // make this the last position
  static float last_position = 0;
  float difference = 0;

  // subtract current position from last position
  difference = current_position - last_position;

  // 359 - 1 = +358, what we would actually want is to say 2 (360 - readin(359) + new reading) = +2
  // 1 - 359 = -358, actual -2 (360 - new_reading + old_reading) = -2
  // crude rollover handling
  if (difference > 330)
  {
    difference = 360 - last_position + current_position;
  }

  if (difference < -330)
  {
    difference = -1 * (360 - current_position + last_position);
  }
  
  last_position = current_position;

  return difference;
}

bool VALIDATE_ENCODER_STATUS(byte *buffer)
{
  bool success = 1;

  // MD - Magnet Detected
  if (!buffer[2])
  {
    Serial.println("Magnet not detected");
    success = 0;
  }

  // ML - Magnet too weak
    Serial.println(buffer[3]);

  if (buffer[3])
  {
    Serial.println("Magnet signal too weak, try bring the magnet closer...");
    success = 0;
  }

  // MH - Magnet too strong
  Serial.println(buffer[4]);
  if (buffer[4])
  {
    Serial.println("Magnet signal to strong, try move the magnet further away...");
    success = 0;
  }

  return success;
}