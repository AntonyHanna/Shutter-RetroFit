#include "LightSequencer.h"
#include <Arduino.h>
#include <utility>
class LightSequencer {
    private:
    unsigned long trackedMillis = -1; 
    
    unsigned long* lightSpans;
    int8_t pin;
    uint pulseCount;
    unsigned long timeSpan;
    
    public:
    bool isInProgress;
    

    /// @brief 
    /// @param pin the pin to pulse
    LightSequencer(uint8_t _pin) {
        trackedMillis = -1;
        isInProgress = false;
        pin = _pin;
        pulseCount = 0;
        lightSpans = NULL;
    }
        
    /// @brief builds an internal structure to determine when to flash the lights
    /// @param _pulseCount number of pulses we want
    /// @param _timeSpan the amount of time in which we need to perform all pulses
    void buildLightSequence(int _pulseCount, unsigned long _timeSpan) {
        if (pin == -1) {
            return;
        }

        if (pulseCount <= 0) {
            return;
        }

        pulseCount = _pulseCount;
        timeSpan = _timeSpan;
        

        trackedMillis = millis();
        lightSpans = calculateLightSpans(pulseCount, timeSpan);
    }

    /// @brief pulses the given number of times within the calculated light span. Should be called in the main loop to keep the lights sync'd.
    void flashSequence() {
        unsigned long current = millis();

        for (int i = 0; i < pulseCount * 2; i++) {
            // guard against leaving array bound
            if (i + 1 >= pulseCount) {
                isInProgress = false;
                return;
            }

            isInProgress = true;
            
            // within a lightSpan
            if (current > lightSpans[i] && current < lightSpans[i + 1]) {
                // if divisible by 2 then its the off period
                if (i % 2 == 0) {
                    digitalWrite(pin, LOW);
                } else {
                    digitalWrite(pin, HIGH);
                }

            }
        }
    }

    void replay() {
        // put light in a known status
        digitalWrite(pin, LOW);
        isInProgress = false;

        trackedMillis = millis();
        lightSpans = calculateLightSpans(pulseCount, timeSpan);
    }

    unsigned long* calculateLightSpans(int pulseCount, unsigned long timeSpan) {
        unsigned long* result;
        unsigned long timePerPulse = timeSpan / (pulseCount * 2);

        if (result != NULL) {
            delete(result);
        }
        result = new unsigned long[pulseCount * 2];

        // builds the seq
        for (uint i = 0; i < pulseCount * 2; i++) {
            result[i] = millis() + (timePerPulse * i);
        }
        
        return result;
    }


    void printLightSpan() {
        Serial.println("LightSequence:");
        
        if (pulseCount == 0) {
            return;
        }
        
        for (int i = 0; i < pulseCount; i++) {
            Serial.printf("\t[%i] | %lu", i, lightSpans[i]);
        }
    }

    ~LightSequencer() {
            if (lightSpans != NULL) {
                delete(lightSpans);
            }
        }
};