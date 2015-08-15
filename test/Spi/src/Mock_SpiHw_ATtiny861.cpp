extern "C"
{
  #include "SpiHw.h"
  #include <avr/io.h>
  #include "BitManip.h"
}
#include "Mock_SpiHw_ATtiny861.h"

#include "CppUTestExt/MockSupport.h"

void SpiHw_SetWireMode(Usi_WireMode wireMode)
{
  mock().actualCall("SpiHw_SetWireMode")
        .withParameter("wireMode", wireMode);
}

void SpiHw_SetClockSource(Usi_ClockSource clockSource)
{
  mock().actualCall("SpiHw_SetClockSource")
        .withParameter("clockSource", clockSource);
}

void SpiHw_ConfigureUsiPins(Usi_PinPosition pinPosition)
{
  mock().actualCall("SpiHw_ConfigureUsiPins")
        .withParameter("pinPosition", pinPosition);
}

void SpiHw_SetCounterOverflowInterrupts(BOOL enableInterrupts)
{
  mock().actualCall("SpiHw_SetCounterOverflowInterrupts")
        .withParameter("enableInterrupts", enableInterrupts);
}

void SpiHw_ClearCounterOverflowInterruptFlag(void)
{
  mock().actualCall("SpiHw_ClearCounterOverflowInterruptFlag");
}

uint8_t SpiHw_SaveInputData(void)
{
  mock().actualCall("SpiHw_SaveInputData");
  return mock().intReturnValue();
}

void SpiHw_SetIsTransmittingFlag(BOOL isTransmitting)
{
  mock().actualCall("SpiHw_SetIsTransmittingFlag")
        .withParameter("isTransmitting", isTransmitting);
}

BOOL SpiHw_GetIsTransmittingFlag(void)
{
  mock().actualCall("SpiHw_GetIsTransmittingFlag");
  return (BOOL)(mock().intReturnValue());
}

void SpiHw_PrepareOutputData(uint8_t data)
{
  mock().actualCall("SpiHw_PrepareOutputData")
        .withParameter("data", data);
}

void SpiHw_ToggleUsiClock(void)
{
  mock().actualCall("SpiHw_ToggleUsiClock");
  if (IF_BITMASK(0xf, USISR, BITMASK_USI_COUNTER)) //4-bit counter
  {
    SET_BITMASK_TO(USISR, 0x0, BITMASK_USI_COUNTER);
    SET_BIT_NUMBER(USISR, USIOIF);
    SpiHw_SetIsTransmittingFlag(FALSE);
  }
  else
  {
    USISR++;
  }
}

uint8_t SpiHw_GetUsiCounter(void)
{
  mock().actualCall("SpiHw_GetUsiCounter");
  return mock().intReturnValue();
}

BOOL SpiHw_IsAnySlaveSelected(void)
{
  mock().actualCall("SpiHw_IsAnySlaveSelected");
  return (BOOL)(mock().intReturnValue());
}

// void SpiHw_SelectSlave(RegisterPointer port, uint8_t bit)
// {
//   mock().actualCall("SpiHw_SelectSlave")
//         .withParameter("port", port)
//         .withParameter("bit", bit);
// }

void SpiHw_ReleaseSlave(RegisterPointer port, uint8_t bit)
{
  mock().actualCall("SpiHw_ReleaseSlave")
        .withParameter("port", (uint8_t *)port)
        .withParameter("bit", bit);
}
