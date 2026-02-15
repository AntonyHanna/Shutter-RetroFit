#ifndef PWMHELPER_H
#define PWMHELPER_H

class PWMHelper {
    private:

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