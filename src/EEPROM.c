#include "EEPROM.h"

#include <avr/io.h>

// bit field written to EEPROM to indicate valid contents
#define EEPROM_INIT_PATTERN 0b00101011

//write single byte to EEPROM.
void writeByteEEPROM(uint8_t b, uint16_t addr) {
  // wait for any other writes to complete (to FLASH or EEPROM)
  for(;;){
    if (!(EECR & (1 << EEPE)) && !(SPMCSR & (1 << SELFPRGEN))) {
      break;
    }
  }

  // set EEPROM address
  EEARL = (uint8_t)addr;
  EEARH = (uint8_t)(addr >> 8);

  // set new data to be written
  EEDR = b;

  //write (note, intentional separation of assignment)
  EECR |= (1 << EEMPE);
  EECR |= (1 << EEPE);
}

// read a byte at addr from the EEPROM
uint8_t readByteEEPROM(uint16_t addr) {
  // wait for any other writes to complete (to FLASH or EEPROM)
  for(;;){
    if (!(EECR & (1 << EEPE)) && !(SPMCSR & (1 << SELFPRGEN))) {
      break;
    }
  }

  // set EEPROM address
  EEARL = (uint8_t)addr;
  EEARH = (uint8_t)(addr >> 8);

  // read
  EECR |= (1 << EERE);

  return EEDR;
}

// write the buffer (buff) to EEPROM starting at addr.
bool writeEEPROM(uint8_t * buff, uint16_t len, uint16_t addr) {
  uint16_t idx = 0;
  if (addr + len <= EEPROM_SIZE) {
    for (uint16_t a = addr; a < (addr + len); a ++){
      writeByteEEPROM(buff[idx++], a);
    }
    return true;
  }
  return false;
}

// read len bytes of EEPROM data starting at addr in to the buffer, buff.
bool readEEPROM(uint8_t * buff, uint16_t len, uint16_t addr){
  uint16_t idx = 0;
  if (addr + len <= EEPROM_SIZE){
    for (uint16_t a = addr; a < (addr + len); a++){
      buff[idx++] = readByteEEPROM(a);
    }
    return true;
  }
  return false;
}

// write the value, val to len bytes starting at addr
bool clearEEPROM(uint8_t val, uint16_t len, uint16_t addr){
  if (addr + len <= EEPROM_SIZE){
    uint8_t zero = 0;
    for (int a = addr; a < (addr + len); a++) {
      writeEEPROM(&zero, 1, a);
    }
    return true;
  }
  return false;
}

/*
  look for the value defined by byteToFind in the range [startAddr, endAddr).
  if no match is found 0 is returned. (zero address is reserved for EEPROM valid indicator pattern)
*/
uint16_t searchEEPROM(uint8_t byteToFind, uint16_t startAddr, uint16_t endAddr){
  for (int addr = startAddr; addr < endAddr; addr++){
    if (byteToFind == readByteEEPROM(addr)) {
      return addr;
    }
  }
  return 0;
}

// check if EEPROM has been initialized. aka does it contain valid data.
bool isEEPROMContentValid() {
  uint8_t pattern = 0;
  readEEPROM(&pattern, 1, 0);
  if (pattern == EEPROM_INIT_PATTERN){
    return true;
  }
  return false;
}

// write EEPROM valid indicator bit pattern and zero all memory
void initEEPROM(){
  uint8_t pattern = EEPROM_INIT_PATTERN;
  writeEEPROM(&pattern, 1, 0);
  clearEEPROM(0, 1023, 1);
}
