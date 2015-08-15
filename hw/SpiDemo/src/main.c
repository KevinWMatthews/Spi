#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <util/delay.h>
#include "Spi.h"
#include "ChipFunctions.h"
#include <avr/interrupt.h>
#include <avr/io.h>
#include "BitManip.h"



#define SPI_SLAVE_SELECT_1 (PINB3)

#define STATUS_PIN1        (PINB5)
#define STATUS_PIN2        (PINB6)


void led_on(uint8_t led)
{
  SET_BIT_NUMBER(PORTB, led);
}

void led_off(uint8_t led)
{
  CLEAR_BIT_NUMBER(PORTB, led);
}

void spi_send_wrapper(SpiSlaveSelectPin slave, uint8_t outputData)
{
  uint8_t inputData;
  int8_t result;

  result = Spi_SendData(slave, outputData);
  if (result != SPI_SUCCESS)
  {
    led_on(STATUS_PIN1);
  }

  inputData = Spi_GetInputData();
  if (inputData != outputData)
  {
    led_on(STATUS_PIN2);
  }
  _delay_ms(1000);  //Display results

  //Reset things for the next time around
  led_off(STATUS_PIN1);
  led_off(STATUS_PIN2);
  _delay_ms(1000);
}


int main(void)
{
  int i;
  uint8_t outputData;
  SpiSlaveSelectPin slave1;

  Spi_HwSetup();
  slave1 = Spi_SlaveSetup(&DDRB, &PORTB, SPI_SLAVE_SELECT_1);
  outputData = 0;

  //Setup status pins
  SET_BIT_NUMBER(DDRB, STATUS_PIN1);
  SET_BIT_NUMBER(DDRB, STATUS_PIN2);

  ChipFunctions_EnableGlobalInterrupts();
  _delay_ms(7000);

  while (1)
  {
    spi_send_wrapper(slave1, outputData);
    outputData++;
  }
}

ISR(USI_OVF_vect)
{
  Spi_UsiOverflowInterrupt();
  //Put a delay here if you want to see the Slave Select pin turn off
}
