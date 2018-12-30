#include "UI.h"

#include <stdbool.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define MAX_POWER 1024
#define POWER_INCREMENT 10
uint16_t powerTarget = 0;

// how many times must the debounce timer trigger before the debounce period ends.
// this is a way of extending the range of the 8bit timer0.
#define DEBOUNCE_TIME_MULTIPLE 8 // about 125ms
uint16_t debounceCounter = 0;
// true if debounce timer is running. (block all control input if not false)
bool isInDebounce = false;

void initUI() {
  //setup debounce timer
  TCCR0A = 0x0;
  // clock stoped
  TCCR0B = 0x0;

  // OCR0A interrupt enable (overflow interrupt already used by wiring lib)
  TIMSK0 = (1 << OCIE0A);
  OCR0A = 0xfe;

  // INT0 and INT1 to input.
  DDRD &= ~((1 << PORTD2) | (1 << PORTD3));
  // pull up on INT0, INT1
  PORTD |= (1 << PORTD2) | (1 << PORTD3);

  // enable interrupt INT1 and INT0
  EIMSK = (1 << INT1) | (1 << INT0);
  // trigger interrupt for INT1 and INT0 on falling edge
  EICRA = (1 << ISC11) | (1 << ISC01);

}

uint16_t getPowerTarget() {
  cli();
  uint16_t tmp = powerTarget;
  sei();
  return tmp;
}

void stopDebounceTimer() {
  cli();
  TCCR0B = 0x0;
  TCNT0 = 0x0;
  isInDebounce = false;
  debounceCounter = 0;
  sei();
}

void startDebounceTimer() {
  cli();
  TCNT0 = 0x0;
  TCCR0B = (1 << CS02) | (1 << CS00);
  isInDebounce = true;
  sei();
}

// triggered on button 0 pressed (pwr up)
ISR(INT0_vect) {
  if (!isInDebounce) {
    if (powerTarget + POWER_INCREMENT < MAX_POWER) {
      powerTarget += POWER_INCREMENT;
    } else {
      powerTarget = MAX_POWER;
    }
    startDebounceTimer();
  }
}

// triggered on button 1 pressed (pwr down)
ISR(INT1_vect) {
  if (!isInDebounce) {
    if ((int)powerTarget - (int)POWER_INCREMENT > 0){
      powerTarget -= POWER_INCREMENT;
    } else {
      powerTarget = 0;
    }
    startDebounceTimer();
  }
}

// OCR0A compare match (debounce timer)
ISR(TIMER0_COMPA_vect) {
  debounceCounter ++;
  if (debounceCounter >= DEBOUNCE_TIME_MULTIPLE){
    stopDebounceTimer();
  }
}
