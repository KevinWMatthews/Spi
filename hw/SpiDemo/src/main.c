#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <util/delay.h>
#include "Spi.h"
#include "ChipFunctions.h"
#include <avr/interrupt.h>

void flash_led(int num_flashes, volatile uint8_t *led)
{
  int i;
  for (i = 0; i < num_flashes * 2; i++)
  {
    _delay_ms(200);
    *led ^= (1<<PB3);
  }
  *led &= ~(1<<PB3);
}

int main(void)
{
  int8_t inputData, outputData, result;

  Spi_HwSetup();

  DDRB |= (1<<PB3); //Status LED

  inputData = 0;
  outputData = 0xa5;

  ChipFunctions_EnableGlobalInterrupts();

  _delay_ms(7000);

  while (1)
  {
    PORTB ^= (1<<PB3);
    _delay_ms(500);
    PORTB ^= (1<<PB3);
    result = Spi_SendData(outputData);
    if (result != SPI_SUCCESS)
    {
      flash_led(4, &PORTB);
    }
    inputData = Spi_GetInputData();
    if (inputData != outputData)
    {
      flash_led(3, &PORTB);
    }
    outputData++;
    _delay_ms(500);
  }
}

ISR(USI_OVF_vect)
{
  Spi_UsiOverflowInterrupt();
}
