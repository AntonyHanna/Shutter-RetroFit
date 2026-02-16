#include "PWMHelper.h"

PWMHelper::PWMHelper(int pwm_pin, int pwm_freq, int pwm_duty) {
    pin = pwm_pin;
    freq = pwm_freq;
    duty = pwm_duty;

    mcpwm_unit = mcpwm_unit_t::MCPWM_UNIT_0;
    mcpwm_timer = mcpwm_timer_t::MCPWM_TIMER_0;
    mcpwm_singal = mcpwm_io_signals_t::MCPWM0A;

    mcpwm_gpio_init(mcpwm_unit, mcpwm_singal, pin);

    cfg = mcpwm_config_t();

    cfg.frequency = freq;
    cfg.cmpr_a = duty;
    cfg.counter_mode = mcpwm_counter_type_t::MCPWM_UP_COUNTER;
    cfg.duty_mode = mcpwm_duty_type_t::MCPWM_DUTY_MODE_0;

    update_freq(freq);
    update_duty(duty);
}

void PWMHelper::enable() {
    mcpwm_gpio_init(mcpwm_unit, mcpwm_singal, pin);
    esp_err_t error = mcpwm_init(mcpwm_unit_t::MCPWM_UNIT_0, mcpwm_timer_t::MCPWM_TIMER_0, &cfg);

    if(error == ESP_OK) {
        Serial.printf("Successfully enabled MCPWM on pin %i\n", pin);
    } else {
        Serial.printf("Failed to enable MCPWM on pin: [%i], with code: [%i]\n", pin, error);
    }
}

void PWMHelper::disable () {
  gpio_reset_pin((gpio_num_t)pin);
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

int PWMHelper::get_freq() {
    return freq;
}

void PWMHelper::update_freq(int pwm_freq) {
    freq = pwm_freq;
    esp_err_t error = mcpwm_set_frequency(mcpwm_unit, mcpwm_timer, freq);

    if(error == ESP_OK) {
        Serial.printf("Successfully updated MCPWM frequency on pin %i\n", pin);
    } else {
        Serial.printf("Failed to update MCPWM frequency on pin: [%i], with code: [%i]\n", pin, error);
    }
}

int PWMHelper::get_duty() {
    return duty;
}

void PWMHelper::update_duty(int pwm_duty) {
    duty = pwm_duty;
    esp_err_t error = mcpwm_set_duty(mcpwm_unit, mcpwm_timer, mcpwm_generator_t::MCPWM_GEN_A, duty);

    if(error == ESP_OK) {
        Serial.printf("Successfully updated MCPWM duty cycle on pin %i\n", pin);
    } else {
        Serial.printf("Failed to update MCPWM duty cycle on pin: [%i], with code: [%i]\n", pin, error);
    }
}
