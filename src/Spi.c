#include "Spi.h"
#include "SpiHw.h"

SpiHw_Slave getSpiHwSlaveFromSpiSlave(Spi_Slave slave)
{
  switch (slave)
  {
    case SPI_SLAVE1:
      return SPI_HW_SLAVE1;
    default:
      break;
  }
  return SPI_HW_SLAVE_ERROR;
}

int8_t Spi_SelectSlave(Spi_Slave slave)
{
  SpiHw_Slave hwSlave;

  hwSlave = getSpiHwSlaveFromSpiSlave(slave);
  if (hwSlave == SPI_HW_SLAVE_ERROR)
  {
    return SPI_SS_ERROR;  //Not sure how to test this
  }

  SpiHw_SetSlaveSelect(hwSlave);
  return SPI_SUCCESS;
}
