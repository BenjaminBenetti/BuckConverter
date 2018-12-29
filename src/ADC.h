#ifndef ADC_H_
#define ADC_H_

#include <stdbool.h>
#include <stdint.h>

// initialize the analog to digital converter
void initADC();

// true if currently sampling ADC
bool isSamplingADC();

// perform an ADC sample on the given channel
void sampleADC(uint8_t port);

// start free run ADC on given channel
void sampleADCFreeRun(uint8_t port);

// get last sample result
uint16_t getADCSample();

#endif
