#include "SpiHw.h"
#include <avr/io.h>
#include "BitManip.h"

void SpiHw_ClearCounterOverflowInterruptFlag(void)
{
  SET_BIT_NUMBER(USISR, USIOIF);
}

void SpiHw_SetCounterOverflowInterrupts(BOOL enableInterrupts)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, enableInterrupts, (1<<USIOIE));
}
