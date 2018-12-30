#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "ADC.h"
#include "PWM.h"
#include "UI.h"
#include "display.h"

uint16_t calcPWMUpdate(uint16_t v, uint16_t vt){
  // "always 1" seems the best method of the ones I'v tried. perhaps some thing better
  // here in the future?
  return 1;
}

void updateDisplay();

// display vars
uint8_t displayDigits[4];
int displayIndex = 0;

// power regulation vars
uint16_t currPWM = 0;
uint16_t oldPowerTarget = 0;
// calculated based on voltage divider circuit
#define VOLTAGE_DIVIDER_COMPENSATION_VALUE 30.0f

void checkNSwitch() {
  uint16_t currVoltage = getADCSample();
  uint16_t currTarget = getPowerTarget();

  // update display if power target changed.
  if (currTarget != oldPowerTarget) {
    oldPowerTarget = currTarget;
    numToSegmentList(displayDigits, VOLTAGE_DIVIDER_COMPENSATION_VALUE * ((float)getPowerTarget() / 1024.0f), -2);
  }

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
  updateDisplay();
}

void updateDisplay(){
    clearAllSegments();
    setDigit(displayIndex);
    setSegment(displayDigits[displayIndex]);
    displayIndex = (displayIndex + 1) % 4;
}

void setup() {
  initADC();
  initPWM();
  initUI();
  initDisplay();

  memset(displayDigits, 0, 4);
  sampleADCFreeRun(0x0);

  sei();
}

void loop(){
  checkNSwitch();
}
