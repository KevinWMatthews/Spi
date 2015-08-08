#ifndef Spi_H
#define Spi_H

#include "DataTypes.h"

void Spi_HwSetup(void);

typedef enum
{
  SPI_SLAVE1 = 0,
} Spi_Slave;
int8_t Spi_SelectSlave(Spi_Slave slave);

//Place this function in the Usi Overflow Interrupt handler.
//This interrupt occurs when a single byte of data has been transferred.
//This function captures data from from the Data Input register into locally scoped memory.
void Spi_UsiOverflowInterrupt(void);

//Access the byte of SPI data that is stored during the USI counter overflow interrupt.
uint8_t Spi_GetInputData(void);

//Send data over SPI lines
int8_t Spi_SendData(uint8_t data);

enum
{
  SPI_USI_COUNTER_ERROR = -3, //This condition should not occur!
  SPI_WRITE_IN_PROGRESS = -2,
  SPI_SS_ERROR          = -1,
  SPI_SUCCESS           =  0
};

#endif
