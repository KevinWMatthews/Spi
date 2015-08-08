#include "Spi.h"
#include "SpiHw.h"

static uint8_t inputData = 0;

void Spi_HwSetup(void)
{
  SpiHw_SetWireMode(USI_THREE_WIRE);
  SpiHw_SetClockSource(USI_EXTERNAL_POSITIVE_EDGE_SOFTWARE_STROBE);
  SpiHw_ConfigureUsiPins(USI_PORTB_PINS);
  SpiHw_SetCounterOverflowInterrupts(TRUE);
  SpiHw_SetIsTransmittingFlag(FALSE);
  SpiHw_SetupSlaveSelect1();
}

void Spi_UsiOverflowInterrupt()
{
  SpiHw_ClearCounterOverflowInterruptFlag();
  SpiHw_SetIsTransmittingFlag(FALSE);
  inputData = SpiHw_SaveInputData();
}

uint8_t Spi_GetInputData(void)
{
  return inputData;
}

int8_t Spi_SendData(uint8_t data)
{
  int8_t result;
  result = SpiHw_PrepareOutputData(data);
  if (result == SPIHW_USI_COUNTER_NONZERO)
  {
    //If this occurs, there's a bug with the SpiHw transmission-in-progress flag
    return SPI_USI_COUNTER_ERROR;
  }
  else if (result != SPIHW_WRITE_STARTED)
  {
    return SPI_WRITE_IN_PROGRESS;
  }

  do
  {
    SpiHw_ToggleUsiClock();
  } while ( SpiHw_GetIsTransmittingFlag() == TRUE );

  return SPI_SUCCESS;
}
