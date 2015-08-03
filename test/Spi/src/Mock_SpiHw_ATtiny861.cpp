extern "C"
{
  #include "SpiHw.h"
}

#include "CppUTestExt/MockSupport.h"

void SpiHw_SetSlaveSelect(SpiHw_Slave slave)
{
  mock().actualCall("SpiHw_SetSlaveSelect")
        .withParameter("slave", slave);
}

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
