#ifndef SpiHw_H
#define SpiHw_H

#include "DataTypes.h"

typedef enum
{
  SPI_HW_SLAVE_ERROR = -1,
  SPIHW_SLAVE_1      =  0
} SpiHw_SlaveNumber;
void SpiHw_SelectSlave(RegisterPointer port, uint8_t bit);
void SpiHw_ReleaseSlave(RegisterPointer port, uint8_t bit);
void SpiHw_ReleaseActiveSlave(void);  //Interrupt safe

void SpiHw_ClearCounterOverflowInterruptFlag(void);

//Copy data into output register
//Sets an internal transmission started flag
void SpiHw_PrepareOutputData(uint8_t data);

//Copy data from input register
uint8_t SpiHw_SaveInputData(void);

void SpiHw_ToggleUsiClock(void);

void SpiHw_SetIsTransmittingFlag(BOOL isTransmitting);
BOOL SpiHw_GetIsTransmittingFlag(void);

#define BITMASK_USI_COUNTER ((1<<USICNT3) | (1<<USICNT2) | (1<<USICNT1) | (1<<USICNT0))
uint8_t SpiHw_GetUsiCounter(void);
void SpiHw_ClearUsiCounter(void);

BOOL SpiHw_IsAnySlaveSelected(void);



//*******************************//
//*** Configuration Functions ***//
//*******************************//
//These are inteded for use by the SpiHw module itself.
#define SPIHW_DATA_REGISTER_SIZE (sizeof(uint8_t) * 8)

typedef enum
{
  USI_PARTIAL_DISABLE = 0b00,
  USI_THREE_WIRE      = 0b01,
  USI_TWO_WIRE_1      = 0b10,
  USI_TWO_WIRE_2      = 0b11    //Don't know the difference. Look it up.
} Usi_WireMode;
#define BITMASK_USI_WIRE_MODE (1<<USIWM1) | (1<<USIWM0)
void SpiHw_SetWireMode(Usi_WireMode wireMode);

typedef enum
{
  USI_NONE                                   = 0b000,
  USI_EXTERNAL_POSITIVE_EDGE_SOFTWARE_STROBE = 0b101,
  USI_EXTERNAL_NEGATIVE_EDGE_SOFTWARE_STROBE = 0b111
} Usi_ClockSource;
#define BITMASK_USI_CLOCK_SOURCE (1<<USICS1) | (1<<USICS0) | (1<<USICLK)
void SpiHw_SetClockSource(Usi_ClockSource clockSource);

typedef enum
{
  USI_PORTB_PINS = 0,
  USI_PORTA_PINS = 1
} Usi_PinPosition;
typedef enum
{
  USI_MASTER = 0,
  USI_SLAVE  = 1
} Usi_DeviceType;
#define USI_PORTA_MISO_BIT DDA0
#define USI_PORTA_MOSI_BIT DDA1
#define USI_PORTA_USCK_BIT DDA2
#define USI_PORTB_MISO_BIT DDB0
#define USI_PORTB_MOSI_BIT DDB1
#define USI_PORTB_USCK_BIT DDB2
void SpiHw_ConfigureUsiPins(Usi_DeviceType masterOrSlave, Usi_PinPosition pinPosition);

//Enable or disable counter overflow interrupts on the USI
void SpiHw_SetCounterOverflowInterrupts(BOOL enableInterrupts);

void SpiHw_SetPinAsOutput(RegisterPointer dataDirectionRegister, uint8_t bit);
void SpiHw_SetPinAsInput(RegisterPointer dataDirectionRegister, uint8_t bit);

#endif
