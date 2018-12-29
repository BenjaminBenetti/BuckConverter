#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

// set duty cycle for time 1 on OC1A pin
void setDutyCycle(uint16_t val);

// set up phase correct 10bit PWM on timer 1
void initPWM();

#endif
