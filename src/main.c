#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#include "ADC.h"
#include "PWM.h"
#include "UI.h"

uint16_t calcPWMUpdate(uint16_t v, uint16_t vt){
  // "always 1" seems the best method of the ones I'v tried. perhaps some thing better
  // here in the future?
  return 1;
}

uint16_t currPWM = 0;
void checkNSwitch() {
  uint16_t currVoltage = getADCSample();
  uint16_t currTarget = getPowerTarget();

  if (currVoltage < currTarget) {
    uint16_t update = calcPWMUpdate(currVoltage, currTarget);
    if (currPWM + update <= 0x3ff){
      currPWM += update;
    } else {
      currPWM = 0x3ff;
    }
    setDutyCycle(currPWM);
  } else {
    uint16_t update = calcPWMUpdate(currVoltage, currTarget);
    if ((int)currPWM - (int)update >= 0){
      currPWM -= update;
    } else {
      currPWM = 0;
    }
    setDutyCycle(currPWM);
  }
}


void setup() {
  initADC();
  initPWM();
  initUI();
  sampleADCFreeRun(0x0);
  sei();
}

void loop(){
  checkNSwitch();
}
