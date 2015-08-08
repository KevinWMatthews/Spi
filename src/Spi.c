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
  //This test should be sufficient
  if (SpiHw_GetIsTransmittingFlag() == TRUE)
  {
    return SPI_WRITE_IN_PROGRESS;
  }

  //Sanity checks
  if (SpiHw_IsAnySlaveSelected() == TRUE)
  {
    return SPI_SLAVE_SELECTED;
  }
  if (SpiHw_GetUsiCounter() != 0)
  {
    //If this occurs, there's a bug with the SpiHw transmission-in-progress flag
    return SPI_USI_COUNTER_NONZERO;
  }

  SpiHw_PrepareOutputData(data);
  SpiHw_SelectSlave(SPIHW_SLAVE_1);

  do
  {
    SpiHw_ToggleUsiClock();
  } while ( SpiHw_GetIsTransmittingFlag() == TRUE );
  SpiHw_ReleaseSlave(SPIHW_SLAVE_1);

  return SPI_SUCCESS;
}
