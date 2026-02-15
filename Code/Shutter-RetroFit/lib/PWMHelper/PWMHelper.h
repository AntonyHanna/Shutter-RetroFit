#ifndef PWMHELPER_H
#define PWMHELPER_H

#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include <Arduino.h>

class PWMHelper {
    private:
    int pin = -1;
    int freq = -1;
    int duty = -1;
    mcpwm_config_t cfg;

    mcpwm_unit_t mcpwm_unit;
    mcpwm_timer_t mcpwm_timer;
    mcpwm_io_signals_t mcpwm_singal;

    public:
    PWMHelper(int pwm_pin, int pwm_freq = 20000, int pwm_duty = 50);
    void enable();
    void disable();
    int get_freq();
    void update_freq(int pwm_freq);
    int get_duty();
    void update_duty(int pwm_duty);
};

#endif