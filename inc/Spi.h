#ifndef Spi_H
#define Spi_H

#include "DataTypes.h"

typedef struct SpiSlaveSelectPinStruct * SpiSlaveSelectPin;



//************************//
//*** Public Functions ***//
//************************//
void Spi_HwSetup(void);

//Setup up each slave device individually.
//Returns a handle to the slave that is can be used to send data to that slave
SpiSlaveSelectPin Spi_SlaveSetup(RegisterPointer ddr_register, RegisterPointer port_register, uint8_t pin_bit);

//Place this function in the Usi Overflow Interrupt handler.
//This interrupt occurs when a single byte of data has been transferred.
//This function captures data from from the Data Input register into locally scoped memory.
void Spi_UsiOverflowInterrupt(void);

enum
{
  SPI_USI_COUNTER_NONZERO = -5, //This condition should not occur!
  SPI_SLAVE_SELECTED      = -4,
  SPI_WRITE_IN_PROGRESS   = -3,
  SPI_SS_ERROR            = -2,
  SPI_NULL_POINTER        = -1,
  SPI_SUCCESS             =  0
};
//Send data over SPI lines
int8_t Spi_SendData(SpiSlaveSelectPin slave, uint8_t data);

//Access the byte of SPI data that was received from the slave
uint8_t Spi_GetInputData(void);



//*************************//
//*** Private Functions ***//
//*************************//
//These functions are used internally by the module itself.
//There should be no reason for the user to call these.
//They have been made public to facilitate unit testing.
void Spi_SelectSlave(SpiSlaveSelectPin self);
void Spi_ReleaseSlave(SpiSlaveSelectPin self);

uint8_t Spi_GetSlaveBit(SpiSlaveSelectPin self);
uint8_t * Spi_GetSlavePortPointer(SpiSlaveSelectPin self);

#endif
