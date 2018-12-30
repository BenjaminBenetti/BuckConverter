/*
 7 segment, 4 digit display driving code.
*/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

// segments of display
#define DISPLAY_NONE  0x00
#define DISPLAY_A     0x01
#define DISPLAY_B     0x02
#define DISPLAY_C     0x04
#define DISPLAY_D     0x08
#define DISPLAY_E     0x10
#define DISPLAY_F     0x20
#define DISPLAY_G     0x40
#define DISPLAY_DP    0x80

// digits
#define DISPLAY_DIG0 0
#define DISPLAY_DIG1 1
#define DISPLAY_DIG2 2
#define DISPLAY_DIG3 3

//initialize the 7 segment display
void initDisplay();

// set a segment of the 7 segment display.
void setSegment(uint8_t seg);

// clear a segment of the 7 segment display.
void clearSegment(uint8_t seg);

// clear all segments on the 7 segment display
void clearAllSegments();

// set the digit of the display (clears others)
void setDigit(uint8_t digit);

// clear all digits from display (blank the display)
void clearAllDigits();

// convert number in range [0 - 9] to segment bitmask
uint8_t numToSegment(uint8_t num);

/*
  convert number num. to a list of 4 numbers. these numbers are the codes
  to illuminate the 7 segment display for digit 0 - 3.
  param:
    - [lst] the list of 4 numbers in to which the segment bitmasks  will be placed.
    - [num] the number to convert for display
    - [expShift] decimal place shift to apply.
*/
void numToSegmentList(uint8_t * lst, float num, int expShift);

#endif
