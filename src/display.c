#include "display.h"

#include <avr/io.h>
#include <math.h>
#include <stdlib.h>

// seven segment display setters
#define SET_DISPLAY_E()   PORTD |= (1 << PORTD4)
#define SET_DISPLAY_D()   PORTD |= (1 << PORTD5)
#define SET_DISPLAY_DP()  PORTD |= (1 << PORTD6)
#define SET_DISPLAY_C()   PORTD |= (1 << PORTD7)
#define SET_DISPLAY_G()   PORTB |= (1 << PORTB0)
#define SET_DISPLAY_A()   PORTB |= (1 << PORTB2)
#define SET_DISPLAY_F()   PORTB |= (1 << PORTB3)
#define SET_DISPLAY_B()   PORTB |= (1 << PORTB4)
#define CLEAR_DISPLAY_E()   PORTD &= ~(1 << PORTD4)
#define CLEAR_DISPLAY_D()   PORTD &= ~(1 << PORTD5)
#define CLEAR_DISPLAY_DP()  PORTD &= ~(1 << PORTD6)
#define CLEAR_DISPLAY_C()   PORTD &= ~(1 << PORTD7)
#define CLEAR_DISPLAY_G()   PORTB &= ~(1 << PORTB0)
#define CLEAR_DISPLAY_A()   PORTB &= ~(1 << PORTB2)
#define CLEAR_DISPLAY_F()   PORTB &= ~(1 << PORTB3)
#define CLEAR_DISPLAY_B()   PORTB &= ~(1 << PORTB4)

// seven segment display enablers
#define ENABLE_DISPLAY_E()   DDRD |= (1 << PORTD4)
#define ENABLE_DISPLAY_D()   DDRD |= (1 << PORTD5)
#define ENABLE_DISPLAY_DP()  DDRD |= (1 << PORTD6)
#define ENABLE_DISPLAY_C()   DDRD |= (1 << PORTD7)
#define ENABLE_DISPLAY_G()   DDRB |= (1 << PORTB0)
#define ENABLE_DISPLAY_A()   DDRB |= (1 << PORTB2)
#define ENABLE_DISPLAY_F()   DDRB |= (1 << PORTB3)
#define ENABLE_DISPLAY_B()   DDRB |= (1 << PORTB4)

//digit selector setters (note, the intentional clear of other digits in each selector)
#define SELECT_DISPLAY_DIG0() PORTC = (1 << PORTC1)
#define SELECT_DISPLAY_DIG1() PORTC = (1 << PORTC2)
#define SELECT_DISPLAY_DIG2() PORTC = (1 << PORTC3)
#define SELECT_DISPLAY_DIG3() PORTC = (1 << PORTC4)

// clear all display digits
#define CLEAR_DISPLAY_DIG_ALL() PORTC = 0x0

// digit selector enablers
#define ENABLE_DISPLAY_DIG0() DDRC |= (1 << PORTC1)
#define ENABLE_DISPLAY_DIG1() DDRC |= (1 << PORTC2)
#define ENABLE_DISPLAY_DIG2() DDRC |= (1 << PORTC3)
#define ENABLE_DISPLAY_DIG3() DDRC |= (1 << PORTC4)

//initialize the 7 segment display
void initDisplay() {
  //segments
  ENABLE_DISPLAY_E();
  ENABLE_DISPLAY_D();
  ENABLE_DISPLAY_DP();
  ENABLE_DISPLAY_C();
  ENABLE_DISPLAY_G();
  ENABLE_DISPLAY_A();
  ENABLE_DISPLAY_F();
  ENABLE_DISPLAY_B();

  //digit selectors
  ENABLE_DISPLAY_DIG0();
  ENABLE_DISPLAY_DIG1();
  ENABLE_DISPLAY_DIG2();
  ENABLE_DISPLAY_DIG3();
}

// set a segment of the 7 segment display.
void setSegment(uint8_t seg){
  if (seg & DISPLAY_A) {
    SET_DISPLAY_A();
  }
  if (seg & DISPLAY_B) {
    SET_DISPLAY_B();
  }
  if (seg & DISPLAY_C) {
    SET_DISPLAY_C();
  }
  if (seg & DISPLAY_D) {
    SET_DISPLAY_D();
  }
  if (seg & DISPLAY_E) {
    SET_DISPLAY_E();
  }
  if (seg & DISPLAY_F) {
    SET_DISPLAY_F();
  }
  if (seg & DISPLAY_G) {
    SET_DISPLAY_G();
  }
  if (seg & DISPLAY_DP) {
    SET_DISPLAY_DP();
  }
}

// clear a segment of the 7 segment display.
void clearSegment(uint8_t seg){
  if (seg & DISPLAY_A) {
    CLEAR_DISPLAY_A();
  }
  if (seg & DISPLAY_B) {
    CLEAR_DISPLAY_B();
  }
  if (seg & DISPLAY_C) {
    CLEAR_DISPLAY_C();
  }
  if (seg & DISPLAY_D) {
    CLEAR_DISPLAY_D();
  }
  if (seg & DISPLAY_E) {
    CLEAR_DISPLAY_E();
  }
  if (seg & DISPLAY_F) {
    CLEAR_DISPLAY_F();
  }
  if (seg & DISPLAY_G) {
    CLEAR_DISPLAY_G();
  }
  if (seg & DISPLAY_DP) {
    CLEAR_DISPLAY_DP();
  }
}

void clearAllSegments() {
  clearSegment(DISPLAY_A);
  clearSegment(DISPLAY_B);
  clearSegment(DISPLAY_C);
  clearSegment(DISPLAY_D);
  clearSegment(DISPLAY_E);
  clearSegment(DISPLAY_F);
  clearSegment(DISPLAY_G);
  clearSegment(DISPLAY_DP);
}

// set the digit of the display (clears others)
void setDigit(uint8_t digit){
  switch (digit) {
    case DISPLAY_DIG0:
      SELECT_DISPLAY_DIG0();
      break;
    case DISPLAY_DIG1:
      SELECT_DISPLAY_DIG1();
      break;
    case DISPLAY_DIG2:
      SELECT_DISPLAY_DIG2();
      break;
    case DISPLAY_DIG3:
      SELECT_DISPLAY_DIG3();
      break;
  }
}

void clearAllDigits() {
  CLEAR_DISPLAY_DIG_ALL();
}


// convert number in range [0 - 9] to segment bitmask
uint8_t numToSegment(uint8_t num){
  switch (num) {
    case 0:
      return DISPLAY_A | DISPLAY_B | DISPLAY_F | DISPLAY_E | DISPLAY_C | DISPLAY_D;
    case 1:
      return DISPLAY_B | DISPLAY_C;
    case 2:
      return DISPLAY_A | DISPLAY_B | DISPLAY_G | DISPLAY_E | DISPLAY_D;
    case 3:
      return DISPLAY_A | DISPLAY_B | DISPLAY_G | DISPLAY_C | DISPLAY_D;
    case 4:
      return DISPLAY_F | DISPLAY_G | DISPLAY_B | DISPLAY_C;
    case 5:
      return DISPLAY_A | DISPLAY_F | DISPLAY_G | DISPLAY_C | DISPLAY_D;
    case 6:
      return DISPLAY_A | DISPLAY_F | DISPLAY_G | DISPLAY_C | DISPLAY_D | DISPLAY_E;
    case 7:
      return DISPLAY_A | DISPLAY_B | DISPLAY_C;
    case 8:
      return DISPLAY_A | DISPLAY_B | DISPLAY_F | DISPLAY_G | DISPLAY_E | DISPLAY_C | DISPLAY_D;
    case 9:
      return DISPLAY_A | DISPLAY_F | DISPLAY_B | DISPLAY_G | DISPLAY_C | DISPLAY_D;
  }
  return DISPLAY_NONE;
}

// like floor but rounds up when value is really close to upper integer
#define QUANTIZATION_CLOSE_THRESH 0.001
uint16_t quantizationErrorAdjustedFloor(double val) {
  if (fabs(val - ceil(val)) <= QUANTIZATION_CLOSE_THRESH){
    return ceil(val);
  } else {
    return floor(val);
  }
}


/*
  convert number num. to a list of 4 numbers. these numbers are the codes
  to illuminate the 7 segment display for digit 0 - 3.
  param:
    - [lst] the list of 4 numbers in to which the segment values will be placed.
    - [num] the number to convert for display
    - [expShift] decimal place shift to apply.
*/
void numToSegmentList(uint8_t * lst, float num, int expShift){
  for (int i =0; i < 4; i ++){
    uint8_t n = (uint8_t)((uint16_t)quantizationErrorAdjustedFloor(num / pow(10, i + expShift)) % 10);
    lst[i] = numToSegment(n);

    if ((i + expShift) == 0) {
      lst[i] |= DISPLAY_DP;
    }
  }
}
