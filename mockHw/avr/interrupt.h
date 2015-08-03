#ifndef interrupt_H
#define interrupt_H

#include <avr/io.h>
#include "BitManip.h"


//Manually mock chip-specific macros
#define sei() SET_BIT_NUMBER(SREG, SREG_I)
#define cli() CLEAR_BIT_NUMBER(SREG, SREG_I)

// Go back through and test the chip functions.
// We need to ensure that all of the registers are mocked in the correct place.

#endif
