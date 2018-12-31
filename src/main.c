#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "ADC.h"
#include "PWM.h"
#include "UI.h"
#include "EEPROM.h"
#include "display.h"

#define PWR_TARGET_EEPROM_ADDR 1
#define PWR_TARGET_EEPROM_MARKER 0b11101010

// find the address of the power target in EEPROM.
uint16_t findPowerTargetInEEPROM(){
  uint16_t addr = searchEEPROM(PWR_TARGET_EEPROM_MARKER, 1, EEPROM_SIZE);
  if (addr == 0){
    return 1;
  }
  return addr;
}

//save power target to EEPROM at a rolling address to extend EEPROM life.
// minimum EEPROM life with the rotation method is 15,000,000 writes. (button presses in this case).
uint16_t savePowerTarget(uint16_t pwrTarget, uint16_t addr){
  uint16_t newAddr = (addr + 3) % (EEPROM_SIZE - 3);
  if (newAddr == 0) {
    newAddr ++;
  }
  //clear old address
  clearEEPROM(0, 3, addr);

  //write to new address
  uint8_t marker = PWR_TARGET_EEPROM_MARKER;
  writeEEPROM(&marker, 1, newAddr);
  writeEEPROM((uint8_t*)&pwrTarget, 2, newAddr + 1);

  return newAddr;
}

//load power target from EEPROM
uint16_t loadPowerTarget(uint16_t addr){
  uint16_t out;
  readEEPROM((uint8_t*)&out, 2, addr + 1);
  return out;
}

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

uint16_t pwrTargetStorageAddress = 0;

// calculated based on voltage divider circuit
#define VOLTAGE_DIVIDER_COMPENSATION_VALUE 30.0f

void checkNSwitch() {
  uint16_t currVoltage = getADCSample();
  uint16_t currTarget = getPowerTarget();

  // update display and save new value if power target has changed.
  if (currTarget != oldPowerTarget) {
    oldPowerTarget = currTarget;
    pwrTargetStorageAddress = savePowerTarget(currTarget, pwrTargetStorageAddress);
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
  if (!isEEPROMContentValid()){
    initEEPROM();
  }

  numToSegmentList(displayDigits, 0, -2);
  sampleADCFreeRun(0x0);
  pwrTargetStorageAddress = findPowerTargetInEEPROM();
  setPowerTarget(loadPowerTarget(pwrTargetStorageAddress));

  sei();
}

void loop(){
  checkNSwitch();
}
