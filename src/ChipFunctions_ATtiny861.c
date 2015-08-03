#include "ChipFunctions.h"
#include "BitManip.h"
#include <avr/interrupt.h>
#include <avr/io.h>


void ChipFunctions_HwSetup(void)
{
  ChipFunctions_SetCpuPrescaler(CF_CPU_PRESCALE_FACTOR_1);
}

void ChipFunctions_EnableGlobalInterrupts(void)
{
  //This call is specific to AVR chips
  //Can also manually set bit 7 (I) of SREG
  sei();
}

void ChipFunctions_DisableGlobalInterrupts(void)
{
  //This call is specific to AVR chips
  //Can also manually clear bit 7 (I) of SREG
  cli();
}

void ChipFunctions_SetCpuPrescaler(Cf_CpuPrescaleFactor prescaleFactor)
{
  SHIFT_AND_SET_BITMASK_TO(CLKPR, prescaleFactor, BITMASK_CF_PRESCALE_FACTOR);
}
