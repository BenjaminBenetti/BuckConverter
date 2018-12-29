#include "ADC.h"

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t lastADCSample = 0;

void initADC() {
  // voltage ref set to AVcc
  // ADLAR off
  // ADC channel 0
  ADMUX = (1 << REFS0);
  // enable ADC
  // interrupt enable
  // prescaler 16 (OC to 1Mhz !!!) 76.9KSa/s
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2);
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

ISR(ADC_vect) {
  lastADCSample = (uint16_t)ADCL;
  lastADCSample |= (((uint16_t)ADCH) << 8);
}

// get last sample result
uint16_t getADCSample() {
  cli();
  uint16_t tmp = lastADCSample;
  sei();
  return tmp;
}
