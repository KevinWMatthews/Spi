#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <util/delay.h>
#include "Spi.h"
#include "ChipFunctions.h"
#include <avr/interrupt.h>

int main(void)
{
  Spi_HwSetup();

  ChipFunctions_EnableGlobalInterrupts();

  while (1)
  {
    //TODO create and add Spi transmission function
  }
}

ISR(USI_OVF_vect)
{
  Spi_UsiOverflowInterrupt();
}
