#include "Arduino.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

uint16_t POWER_TARGET_TMP = 125;
// ---------- ADC --------------
void initADC() {
  // voltage ref set to AVcc
  // ADLAR off
  // ADC channel 0
  ADMUX = (1 << REFS0);
  // enable ADC
  // interrupt enable
  // prescaler 8 (OC to 2Mhz !!!) 153KSa/s
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS1) | (1 << ADPS0);
}

// true if currently sampling ADC
bool isSamplingADC() {
  return (bool)(ADCSRA & (1 << ADSC));
}

// perform an ADC sample on the given channel
void sampleADC(uint8_t port) {
  ADMUX = (ADMUX & 0xf0) | (port & 0x0f);
  ADCSRA |= (1 << ADSC);
}

// start free run ADC on given channel
void sampleADCFreeRun(uint8_t port){
  ADMUX = (ADMUX & 0xf0) | (port & 0x0f);
  ADCSRB = 0x00;
  ADCSRA |= (1 << ADATE);
  ADCSRA |= (1 << ADSC);
}

uint16_t lastADCSample = 0;
ISR(ADC_vect) {
  cli();
  lastADCSample = (uint16_t)ADCL;
  lastADCSample |= (((uint16_t)ADCH) << 8);
  sei();
}

// get last sample result
uint16_t getADCSample() {
  return lastADCSample;
}
// /////////// END ADC /////////
// ------------ PWM -----------
void setDutyCycle(uint16_t val) {
  OCR1AL = (uint8_t)val;
  OCR1AH = (uint8_t)(val >> 8);
}

void initPWM() {
  setDutyCycle(0x0);
  // clear OC1A on compare match, set OC1A when down counting
  // Phase correct PWM 10 bit.
  TCCR1A = (1 << COM1A1) | (1 << WGM11) | (1 << WGM10);
  // prescaler 1 (16Mhz clock)
  TCCR1B = (1 << CS10);
}
// ////////// END PWM //////////

uint16_t calcPWMUpdate(uint16_t v, uint16_t vt){
  // "always 1" seems the best method of the ones I'v tried. perhaps some thing better
  // here in the future?
  return 1;
}

uint16_t currPWM = 0;
void checkNSwitch() {
  uint16_t currVoltage = getADCSample();

  if (currVoltage < POWER_TARGET_TMP) {
    uint16_t update = calcPWMUpdate(currVoltage, POWER_TARGET_TMP);
    if (currPWM + update <= 0x3ff){
      currPWM += update;
    } else {
      currPWM = 0x3ff;
    }
    setDutyCycle(currPWM);
  } else {
    uint16_t update = calcPWMUpdate(currVoltage, POWER_TARGET_TMP);
    if ((int)currPWM - (int)update >= 0){
      currPWM -= update;
    } else {
      currPWM = 0;
    }
    setDutyCycle(currPWM);
  }
}


void setup() {
  DDRB = (1 << PORTB1);
  initADC();
  initPWM();
  sampleADCFreeRun(0x0);
  sei();
}

void loop(){
  checkNSwitch();
}
