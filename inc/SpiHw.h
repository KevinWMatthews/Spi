#ifndef SpiHw_H
#define SpiHw_H

#include "DataTypes.h"

typedef enum
{
  SPI_HW_SLAVE_ERROR = -1,
  SPI_HW_SLAVE1      =  0
} SpiHw_Slave;
void SpiHw_SetSlaveSelect(SpiHw_Slave slave);

void SpiHw_ClearCounterOverflowInterruptFlag(void);

void SpiHw_SetCounterOverflowInterrupts(BOOL enableInterrupts);

typedef enum
{
  USI_PARTIAL_DISABLE = 0b00,
  // USI_THREE_WIRE      = 0b01,
  USI_TWO_WIRE_1      = 0b10,
  USI_TWO_WIRE_2      = 0b11    //Don't know the difference. Look it up.
} Usi_WireMode;
#define BITMASK_USI_WIRE_MODE (1<<USIWM1) | (1<<USIWM0)
void SpiHw_SetWireMode(Usi_WireMode wireMode);

#endif
