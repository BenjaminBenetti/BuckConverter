#include "PWM.h"

#include <avr/io.h>

void setDutyCycle(uint16_t val) {
  OCR1AL = (uint8_t)val;
  OCR1AH = (uint8_t)(val >> 8);
}

void initPWM() {
  DDRB |= (1 << PORTB1);
  setDutyCycle(0x0);
  // clear OC1A on compare match, set OC1A when down counting
  // Phase correct PWM 10 bit.
  TCCR1A = (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
  // prescaler 1 (16Mhz clock)
  TCCR1B = (1 << CS10);
}
