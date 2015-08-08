#ifndef Spi_H
#define Spi_H

#include "DataTypes.h"

void Spi_HwSetup(void);

//Place this function in the Usi Overflow Interrupt handler.
//This interrupt occurs when a single byte of data has been transferred.
//This function captures data from from the Data Input register into locally scoped memory.
void Spi_UsiOverflowInterrupt(void);

//Access the byte of SPI data that is stored during the USI counter overflow interrupt.
uint8_t Spi_GetInputData(void);

enum
{
  SPI_USI_COUNTER_NONZERO = -4, //This condition should not occur!
  SPI_SLAVE_SELECTED      = -3,
  SPI_WRITE_IN_PROGRESS   = -2,
  SPI_SS_ERROR            = -1,
  SPI_SUCCESS             =  0
};
//Send data over SPI lines
int8_t Spi_SendData(uint8_t data);

#endif
