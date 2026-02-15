#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include <Arduino.h>
#include "PWMHelper.h"

class PWMHelper {
    private:
    int pin = -1;
    int freq = -1;
    int duty = -1;

    mcpwm_unit_t mcpwm_unit;
    mcpwm_timer_t mcpwm_timer;
    mcpwm_io_signals_t mcpwm_singal;
    public:
    PWMHelper(int pwm_pin, int pwm_freq = 20000, int pwm_duty = 50) {
        pin = pwm_pin;
        freq = pwm_freq;
        duty = pwm_duty;

        mcpwm_unit = mcpwm_unit_t::MCPWM_UNIT_0;
        mcpwm_timer = mcpwm_timer_t::MCPWM_TIMER_0;
        mcpwm_singal = mcpwm_io_signals_t::MCPWM0A;

        mcpwm_gpio_init(mcpwm_unit, mcpwm_singal, pin);

        update_freq(freq);
        update_duty(duty);
    }

    void enable() {
        esp_err_t error = mcpwm_start(mcpwm_unit, mcpwm_timer);

        if(error == ESP_OK) {
            Serial.printf("Successfully enabled MCPWM on pin %i", pin);
        } else {
            Serial.printf("Failed to enable MCPWM on pin: [%i], with code: [%i]", pin, error);
        }
    }

    void disable () {
        esp_err_t error = mcpwm_stop(mcpwm_unit, mcpwm_timer);

         if(error == ESP_OK) {
            Serial.printf("Successfully disabled MCPWM on pin %i", pin);
        } else {
            Serial.printf("Failed to disable MCPWM on pin: [%i], with code: [%i]", pin, error);
        }
    }

    int get_freq() {
        return freq;
    }

    void update_freq(int pwm_freq) {
        freq = pwm_freq;
        esp_err_t error = mcpwm_set_frequency(mcpwm_unit, mcpwm_timer, freq);

        if(error == ESP_OK) {
            Serial.printf("Successfully updated MCPWM frequency on pin %i", pin);
        } else {
            Serial.printf("Failed to update MCPWM frequency on pin: [%i], with code: [%i]", pin, error);
        }
    }

    int get_duty() {
        return duty;
    }

    void update_duty(int pwm_duty) {
        duty = pwm_duty;
        esp_err_t error = mcpwm_set_duty(mcpwm_unit, mcpwm_timer, mcpwm_generator_t::MCPWM_GEN_A, duty);

        if(error == ESP_OK) {
            Serial.printf("Successfully updated MCPWM duty cycle on pin %i", pin);
        } else {
            Serial.printf("Failed to update MCPWM duty cycle on pin: [%i], with code: [%i]", pin, error);
        }
    }
};