#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "ADC.h"
#include "PWM.h"

uint16_t POWER_TARGET_TMP = 125;

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
