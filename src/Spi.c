#include "Spi.h"
#include "SpiHw.h"
#include <stdlib.h>
#include "BitManip.h"
#include "Timer0_ATtiny861.h"

typedef struct SpiSlaveSelectPinStruct
{
  RegisterPointer port;
  uint8_t bit;
} SpiSlaveSelectPinStruct;

static uint8_t inputData = 0;

void setSpiMode(void)
{
  SpiHw_SetWireMode(USI_THREE_WIRE);
  SpiHw_SetClockSource(USI_EXTERNAL_POSITIVE_EDGE_SOFTWARE_STROBE);
}

void Spi_HwSetupMaster(void)
{
  setSpiMode();
  SpiHw_ConfigureUsiPins(USI_MASTER, USI_PORTB_PINS);
  SpiHw_SetCounterOverflowInterrupts(TRUE);
  SpiHw_SetIsTransmittingFlag(FALSE);

  Timer0_SetTimerBitWidth(T0_EIGHT_BIT);
  Timer0_ClearTimerOnMatch(TRUE);
  Timer0_SetPrescaleFactor(T0_PRESCALE_FACTOR_64);
  Timer0_SetTimerCompareValue0A(125);
  Timer0_SetTimerCompareInterrupt0A(FALSE);
}

void Spi_HwSetupSlave(void)
{
  setSpiMode();
  SpiHw_ConfigureUsiPins(USI_SLAVE, USI_PORTB_PINS);
  SpiHw_SetCounterOverflowInterrupts(TRUE);
  SpiHw_SetIsTransmittingFlag(FALSE);
}

SpiSlaveSelectPin Spi_SlaveSetup(RegisterPointer dataDirectionRegister, RegisterPointer portRegister, uint8_t pinBit)
{
  SpiSlaveSelectPin self;
  RETURN_VALUE_IF_NULL(dataDirectionRegister, NULL);
  RETURN_VALUE_IF_NULL(portRegister, NULL);
  if (pinBit >= SPIHW_DATA_REGISTER_SIZE)
  {
    return NULL;
  }

  self = calloc(1, sizeof(SpiSlaveSelectPin));
  self->port = portRegister;
  self->bit = pinBit;
  SpiHw_ReleaseSlave(portRegister, pinBit);
  SpiHw_SetPinAsOutput(dataDirectionRegister, pinBit);   //Once set, we don't need to keep track of this register
  return self;
}

void Spi_UsiOverflowInterrupt(void)
{
  Timer0_SetTimerCompareInterrupt0A(FALSE);
  SpiHw_ReleaseActiveSlave();
  SpiHw_ClearCounterOverflowInterruptFlag();
  SpiHw_SetIsTransmittingFlag(FALSE);
  inputData = SpiHw_SaveInputData();
}

uint8_t Spi_GetInputData(void)
{
  return inputData;
}


int8_t Spi_SendData(SpiSlaveSelectPin slave, uint8_t data)
{
  RETURN_VALUE_IF_NULL(slave, SPI_NULL_POINTER);
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
  Spi_SelectSlave(slave);
  Timer0_SetTimerCompareInterrupt0A(TRUE);
  return SPI_SUCCESS;
}


void Spi_SelectSlave(SpiSlaveSelectPin self)
{
  RETURN_IF_NULL(self);
  SpiHw_SelectSlave(self->port, self->bit);
}

void Spi_ReleaseSlave(SpiSlaveSelectPin self)
{
  RETURN_IF_NULL(self);
  SpiHw_ReleaseSlave(self->port, self->bit);
}

uint8_t Spi_GetSlaveBit(SpiSlaveSelectPin self)
{
  RETURN_VALUE_IF_NULL(self, 0);
  return self->bit;
}

volatile uint8_t * Spi_GetSlavePortPointer(SpiSlaveSelectPin self)
{
  RETURN_VALUE_IF_NULL(self, NULL);
  return self->port;
}
