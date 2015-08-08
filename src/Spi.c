#include "Spi.h"
#include "SpiHw.h"

static uint8_t inputData = 0;

void Spi_HwSetup(void)
{
  SpiHw_SetWireMode(USI_THREE_WIRE);
  SpiHw_SetClockSource(USI_EXTERNAL_POSITIVE_EDGE_SOFTWARE_STROBE);
  SpiHw_ConfigureUsiPins(USI_PORTB_PINS);
  SpiHw_SetCounterOverflowInterrupts(TRUE);
  SpiHw_SetIsTransmissionInProgressFlag(FALSE);
}

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

void Spi_UsiOverflowInterrupt()
{
  SpiHw_ClearCounterOverflowInterruptFlag();
  inputData = SpiHw_SaveInputData();
}

uint8_t Spi_GetInputData(void)
{
  return inputData;
}

int8_t Spi_SendData(uint8_t data)
{
  if (SpiHw_PrepareOutputData(data) != SPIHW_WRITE_STARTED)
  {
    return SPI_WRITE_IN_PROGRESS;
  }

  do
  {
    SpiHw_ToggleUsiClock();
  } while ( SpiHw_GetIsTransmissionInProgressFlag() == TRUE );

  return SPI_SUCCESS;
}
