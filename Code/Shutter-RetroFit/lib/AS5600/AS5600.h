#ifndef AS5600_H
#define AS5600_H
#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>

#define STATUS_REG 0x0B
#define RAW_ANGLE_REG 0x0E
#define READ_LEN 1

class AS5600 {
    private:
    uint8_t address;

    public:
        AS5600(uint8_t _addr);
        uint16_t get_encoder_error(uint8_t *buffer);
        bool validate_encoder_error(uint8_t *buffer);
        int read_sensor_angle();
        float calculate_degrees_deviation();
};
#endif