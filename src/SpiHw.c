#include "SpiHw.h"
#include <avr/io.h>
#include "BitManip.h"

//This flag isn't strictly necessary for single-threaded SPI?
//If we're going multi-threaded, does it need to be more complex?
//Are writes atomic? Should this be a macro?
//This flag is set by when data is placed in the output register and
//is clear when the overflow interrupt is serviced.
static BOOL isTransmissionInProgressFlag = FALSE;

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

int8_t SpiHw_PrepareOutputData(uint8_t data)
{
  if (SpiHw_GetIsTransmissionInProgressFlag() == TRUE)
  {
    return SPIHW_WRITE_IN_PROGRESS;
  }
  SpiHw_SetIsTransmissionInProgressFlag(TRUE);
  USIDR = data;
  return SPIHW_WRITE_STARTED;
}

uint8_t SpiHw_SaveInputData(void)
{
  return USIDR;
}

void SpiHw_ToggleUsiClock(void)
{
  SET_BIT_NUMBER(USICR, USITC);
}

void SpiHw_SetSlaveSelect(SpiHw_Slave slave)
{
  //temporary dummy for compiling
}

void SpiHw_SetIsTransmissionInProgressFlag(BOOL isTransmissionInProgress)
{
  isTransmissionInProgressFlag = isTransmissionInProgress;
}

BOOL SpiHw_GetIsTransmissionInProgressFlag(void)
{
  return isTransmissionInProgressFlag;
}

uint8_t SpiHw_GetUsiCounter(void)
{
  return USISR & BITMASK_USI_COUNTER;
}

void SpiHw_ClearUsiCounter(void)
{
  SHIFT_AND_SET_BITMASK_TO(USISR, 0x0, BITMASK_USI_COUNTER);
}
