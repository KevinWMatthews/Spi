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

void SpiHw_SetWireMode(Usi_WireMode wireMode)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, wireMode, BITMASK_USI_WIRE_MODE);
}

void SpiHw_SetClockSource(Usi_ClockSource clockSource)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, clockSource, BITMASK_USI_CLOCK_SOURCE);
}

void SpiHw_ConfigureUsiPins(Usi_PinPosition pinPosition)
{
  SHIFT_AND_SET_BITMASK_TO(USIPP, pinPosition, (1<<USIPOS));
  CLEAR_BIT_NUMBER(DDRB, DDB0);
  SET_BIT_NUMBER(DDRB, DDB1);
  SET_BIT_NUMBER(DDRB, DDB2);
}

void SpiHw_PrepareOutputData(uint8_t data)
{
  USIDR = data;
}
