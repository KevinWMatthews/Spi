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


#endif
