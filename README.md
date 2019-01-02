# BuckConverter
Home made buck converter with Arduino. Can provide switching between 0-30v (cannot provide more voltage than input source). Max input power is 30v. If not external
power is supplied (through Arduino USB jack) minimum voltage of 6 volts required to power Arduino. Max amperage is unknown
but be careful as this unit can get very hot (no joke it can burn down your house). any thing under 2 amps ought to be ok.

## Hardware Requirments
  - 3d Printer, to build the case.
  - 1x 7 segment 4 digit display
  - 1x Arduino nano (any Atmega 48P/88P/168P or 328P AVR MCU)
  - 1x inductor
  - 1x 1000 uF capacitor
  - 1x P type Mosfet
  - 5x NPN transistor
  - 2x tact switch
  - 1x diode
  - 2x 10k Ohm  resistor
  - 4x 5.1k Ohm resistor
  - 1x 2K Ohm   resistor
  - 1x 1K Ohm   resistor
  - 2x 330 Ohm  resistor
  - 1x 100 Ohm  resistor

## Hardware Build

with all the hardware in hand you should be able to build the circuits in the /Hardware folder:
- [Buck Converter circuit](https://github.com/CanadianCommander/BuckConverter/blob/master/Hardware/BuckConverterCircuit.pdf)
- [User Interface circuit](https://github.com/CanadianCommander/BuckConverter/blob/master/Hardware/UserInterfaceCircuit.pdf)

After that print out the case with:
- [Case Top](https://github.com/CanadianCommander/BuckConverter/blob/master/Hardware/BuckConverterTop.stl)
- [Case Bottom](https://github.com/CanadianCommander/BuckConverter/blob/master/Hardware/BuckConverterBottom.stl)

Now sandwich all the electronics in to the case and you should end up with some thing similar to [this](https://github.com/CanadianCommander/BuckConverter/blob/master/Hardware/BuckConverterFull.stl)

## Code Build
- download repo.
- install [arduion make](https://github.com/sudar/Arduino-Makefile)
- if necessary edit the Makefile (/src/Makefile) to reflect your configuration
- then `make upload` to build and upload the code to your board.
- enjoy your buck converter!
