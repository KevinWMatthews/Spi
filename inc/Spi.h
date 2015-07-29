#ifndef Spi_H
#define Spi_H

#include "DataTypes.h"

typedef enum
{
  SPI_SLAVE1 = 0,
} Spi_Slave;
int8_t Spi_SelectSlave(Spi_Slave slave);

enum
{
  SPI_SS_ERROR = -1,
  SPI_SUCCESS  =  0
};

#endif
