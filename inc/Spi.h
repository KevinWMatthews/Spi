#ifndef Spi_H
#define Spi_H

#include "DataTypes.h"

typedef enum
{
  SPI_SLAVE1 = 0,
} Spi_Slave;
int8_t Spi_SelectSlave(Spi_Slave slave);

void Spi_HwSetup(void);

//This interrupt occurs when a single byte of data has been transferred.
//This function captures data from from the Data Input register into locally scoped memory.
void Spi_UsiOverflowInterrupt();

//Access the byte of SPI data that is stored during the USI counter overflow interrupt.
uint8_t Spi_GetInputData(void);

enum
{
  SPI_SS_ERROR = -1,
  SPI_SUCCESS  =  0
};

#endif
