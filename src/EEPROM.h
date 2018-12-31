#ifndef EEPROM_H_
#define EEPROM_H_

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_SIZE 1024 // 1KB

// write the buffer (buff) to EEPROM starting at addr.
bool writeEEPROM(uint8_t * buff, uint16_t len, uint16_t addr);

// read len bytes of EEPROM data starting at addr in to the buffer, buff.
bool readEEPROM(uint8_t * buff, uint16_t len, uint16_t addr);

// write the value, val to len bytes starting at addr
bool clearEEPROM(uint8_t val, uint16_t len, uint16_t addr);

// look for the value defined by byteToFind in the range [startAddr, endAddr)
uint16_t searchEEPROM(uint8_t byteToFind, uint16_t startAddr, uint16_t endAddr);

// check if EEPROM has been initialized. aka does it contain valid data.
bool isEEPROMContentValid();

// write EEPROM valid indicator bit pattern and zero all memory
void initEEPROM();

#endif
