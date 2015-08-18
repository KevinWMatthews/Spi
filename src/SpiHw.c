#include "SpiHw.h"
#include <avr/io.h>
#include "BitManip.h"
#include "DataTypes.h"


//This flag isn't strictly necessary for single-threaded SPI?
//If we're going multi-threaded, does it need to be more complex?
//Are writes atomic? Should this be a macro?
//This flag is set by when data is placed in the output register and
//is clear when the overflow interrupt is serviced.
static BOOL isTransmittingFlag = FALSE;

//Only one slave can be selected at a time.
//Store this slave so it can easily be released from within an interrupt.
static RegisterPointer activeSlavePort;
static uint8_t         activeSlaveBit;

void SpiHw_ClearCounterOverflowInterruptFlag(void)
{
  SET_BIT_NUMBER(USISR, USIOIF);
}

void SpiHw_SetCounterOverflowInterrupts(BOOL enableInterrupts)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, enableInterrupts, (1<<USIOIE));
}

void SpiHw_SetWireMode(Usi_WireMode wireMode)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, wireMode, BITMASK_USI_WIRE_MODE);
}

void SpiHw_SetClockSource(Usi_ClockSource clockSource)
{
  SHIFT_AND_SET_BITMASK_TO(USICR, clockSource, BITMASK_USI_CLOCK_SOURCE);
}

void setMisoAsInput(Usi_PinPosition pinPosition)
{
  if (pinPosition == USI_PORTB_PINS)
  {
    CLEAR_BIT_NUMBER(DDRB, DDB0);
  }
  else if (pinPosition == USI_PORTA_PINS)
  {
    CLEAR_BIT_NUMBER(DDRA, DDA0);
  }
}

void setMosiAsOutput(Usi_PinPosition pinPosition)
{
  if (pinPosition == USI_PORTB_PINS)
  {
    SET_BIT_NUMBER(DDRB, DDB1);
  }
  else if (pinPosition == USI_PORTA_PINS)
  {
    SET_BIT_NUMBER(DDRA, DDA1);
  }
}

void setClockAsOutput(Usi_PinPosition pinPosition)
{
  if (pinPosition == USI_PORTB_PINS)
  {
    SET_BIT_NUMBER(DDRB, DDB2);
  }
  else if (pinPosition == USI_PORTA_PINS)
  {
    SET_BIT_NUMBER(DDRA, DDA2);
  }
}

void SpiHw_ConfigureUsiPins(Usi_PinPosition pinPosition)
{
  SHIFT_AND_SET_BITMASK_TO(USIPP, pinPosition, (1<<USIPOS));
  setMisoAsInput(pinPosition);
  setMosiAsOutput(pinPosition);
  setClockAsOutput(pinPosition);
}

void SpiHw_PrepareOutputData(uint8_t data)
{
  SpiHw_SetIsTransmittingFlag(TRUE);
  USIDR = data;
}

uint8_t SpiHw_SaveInputData(void)
{
  return USIDR;
}

void SpiHw_ToggleUsiClock(void)
{
  SET_BIT_NUMBER(USICR, USITC);
}

void SpiHw_SetIsTransmittingFlag(BOOL isTransmitting)
{
  isTransmittingFlag = isTransmitting;
}

BOOL SpiHw_GetIsTransmittingFlag(void)
{
  return isTransmittingFlag;
}

uint8_t SpiHw_GetUsiCounter(void)
{
  return USISR & BITMASK_USI_COUNTER;
}

void SpiHw_ClearUsiCounter(void)
{
  SHIFT_AND_SET_BITMASK_TO(USISR, 0x0, BITMASK_USI_COUNTER);
}

void SpiHw_SelectSlave(RegisterPointer port, uint8_t bit)
{
  RETURN_IF_NULL(port);
  CLEAR_BIT_NUMBER(*port, bit);
  activeSlavePort = port;
  activeSlaveBit  = bit;
}

void SpiHw_ReleaseSlave(RegisterPointer port, uint8_t bit)
{
  RETURN_IF_NULL(port);
  SET_BIT_NUMBER(*port, bit);
}

void SpiHw_ReleaseActiveSlave(void)
{
  SpiHw_ReleaseSlave(activeSlavePort, activeSlaveBit);
  activeSlavePort = NULL;
}

BOOL SpiHw_IsAnySlaveSelected(void)
{
  //Change this behavior once a second slave select line is implemented.
  return FALSE;
}

void SpiHw_SetPinAsOutput(RegisterPointer dataDirectionRegister, uint8_t bit)
{
  RETURN_IF_NULL(dataDirectionRegister);
  SET_BIT_NUMBER(*dataDirectionRegister, bit);
}
