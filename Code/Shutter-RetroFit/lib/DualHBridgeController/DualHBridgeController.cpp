#include <Wire.h>
#include <Arduino.h>

#include "DualHBridgeController.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"

#define H_LEN 14
#define H_REN 13

#define H_LCTRL 25
#define H_RCTRL 26

#define M_SENSE 34

#define PWM_FREQ 5000
#define PWM_DUTY 30


#define AS5600_ADDR 0x36
#define STATUS_REG 0x0B
#define READ_LEN 1

class DualHBridgeController {
    public:
    DualHBridgeController() {
        direction = 1;
        pendingDirectionChange = false;
    }
    
    void ToggleDriveDirection() {


        if (direction == 1) {
            direction = 2;
        } else {
            direction = 1;
        }

        pendingDirectionChange = true;
    }

    void DriveMotor(int freq, int duty) {
        

        // gen_pwm(PWM_FREQ, PWM_DUTY);

        // turn off both bridges
        digitalWrite(H_LEN, LOW);
        digitalWrite(H_REN, LOW);

        if (direction = 1) {
            digitalWrite(H_LCTRL, HIGH);
            digitalWrite(H_RCTRL, LOW);
        } else {
            digitalWrite(H_LCTRL, LOW);
            digitalWrite(H_RCTRL, HIGH);
        }

        // turn on both bridges
        digitalWrite(H_LEN, HIGH);
        digitalWrite(H_REN, HIGH);
    }

void gen_pwm(int drive_pin) {
  // config pin
  mcpwm_gpio_init(mcpwm_unit_t::MCPWM_UNIT_0, mcpwm_io_signals_t::MCPWM0A, drive_pin);

  mcpwm_config_t pwm;
  pwm.frequency = PWM_FREQ;
  pwm.cmpr_a = 100 - PWM_DUTY;
  pwm.counter_mode = mcpwm_counter_type_t::MCPWM_UP_COUNTER;
  pwm.duty_mode = mcpwm_duty_type_t::MCPWM_DUTY_MODE_0;
  
  mcpwm_init(mcpwm_unit_t::MCPWM_UNIT_0, mcpwm_timer_t::MCPWM_TIMER_0, &pwm);
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

    ~DualHBridgeController() {
        
    }

    private:
    int direction;
    bool pendingDirectionChange;
};