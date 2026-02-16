#include "../lib/AS5600/AS5600.h"

class AS5600 {
    uint8_t AS5600::addr;

    AS5600::AS5600(uint8_t _addr) {
        addr = _addr;
    }

    void AS5600::get_encoder_error(byte* buffer) {
        Wire.beginTransmission(addr);
        // Move to STATUS register?
        Wire.write(STATUS_REG);

        Wire.endTransmission();

        Wire.requestFrom(addr, 1);
        Wire.readBytes(buffer, READ_LEN);

        for (int i = 0; i < READ_LEN; i++)
        {
            Serial.print("Read from register: ");
            Serial.println(buffer[i], BIN);
        }
    }

    int AS5600::read_sensor_angle() {
    const float deg_per_res = 360.0 / 4096.0;
        float degrees = 0;
        uint16_t highByte = 0;
        uint16_t lowByte = 0;

        Wire.beginTransmission(addr);
        Wire.write(0x0E);
        Wire.endTransmission();

        Wire.requestFrom(addr, 1);

        while(Wire.available() == 0);

        highByte = Wire.read();

        Wire.beginTransmission(addr);
        Wire.write(0x0F);
        Wire.endTransmission();

        Wire.requestFrom(addr, 1);

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

    float AS5600::calculate_degrees_deviation(float current_position)  {
        // read last angle from eeprom
        // make this the last position
        static float last_position = 0;
        float difference = 0;

        // subtract current position from last position
        difference = current_position - last_position;

        // 359 - 1 = +358, what we would actually want is to say 2 (360 - readin(359) + new reading) = +2
        // 1 - 359 = -358, actual -2 (360 - new_reading + old_reading) = -2
        // crude rollover handling
        if (difference > 330) {
            difference = 360 - last_position + current_position;
        }

        if (difference < -330) {
            difference = -1 * (360 - current_position + last_position);
        }
        
        last_position = current_position;

        return difference;
    }

    bool AS5600::validate_encoder_error(byte *buffer) {
        bool success = 1;

        // MD - Magnet Detected
        if (!buffer[2]) {
            Serial.println("Magnet not detected");
            success = 0;
        }

        // ML - Magnet too weak
        Serial.println(buffer[3]);

        if (buffer[3])  {
            Serial.println("Magnet signal too weak, try bring the magnet closer...");
            success = 0;
        }

        // MH - Magnet too strong
        Serial.println(buffer[4]);
        if (buffer[4]) {
            Serial.println("Magnet signal to strong, try move the magnet further away...");
            success = 0;
        }

        return success;
    }
};