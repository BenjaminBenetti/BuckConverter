#include "Arduino.h"

#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define CLOSE_VOLTAGE 0
uint16_t POWER_TARGET_TMP = 100;

// ---------- ADC --------------
uint16_t lastADCSample = 0;
void initADC() {
  // voltage ref set to AVcc
  // ADLAR off
  // ADC channel 0
  ADMUX = (1 << REFS0);
  // enable ADC
  // interupt enable
  // prescalar 16 (OC to 1Mhz !!!)
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS2);
}

// true if currently sampling ADC
bool isSamplingADC() {
  if (ADCSRA & (1 << ADSC)) {
    return true;
  }
  return false;
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
bool isClose(uint16_t n1, uint16_t n2, uint16_t thresh) {
  return (abs((int)n1 - (int)n2) < thresh);
}

void checkNSwitch() {
  uint16_t currVoltage = getADCSample();

  if (isClose(currVoltage, POWER_TARGET_TMP, CLOSE_VOLTAGE)) {
    //nop
  }
  if (currVoltage < POWER_TARGET_TMP) {
    PORTD |= (1 << PORTD6);
  } else {
    PORTD &= ~(1 << PORTD6);
  }

}


void setup() {
  DDRD = (1 << PORTD6);
  PORTD = (0 << PORTD6);
  initADC();
  sampleADCFreeRun(0x0);
  sei();
}

void loop(){
  checkNSwitch();
}
