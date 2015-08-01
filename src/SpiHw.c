#include "SpiHw.h"
#include <avr/io.h>
#include "BitManip.h"

void SpiHw_ClearOverflowInterruptFlag(void)
{
  SET_BIT_NUMBER(USISR, USIOIF);
}
