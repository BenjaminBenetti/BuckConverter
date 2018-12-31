#ifndef UI_H_
#define UI_H_

#include <stdint.h>

// configure UI (digital IO pins)
void initUI();

// get the current power target 0 - 1024.
uint16_t getPowerTarget();

// set power target in range 0 - 1024
void setPowerTarget(uint16_t targ);

#endif
