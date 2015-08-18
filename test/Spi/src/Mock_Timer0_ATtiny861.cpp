extern "C"
{
  #include "Timer0_ATtiny861.h"
}

#include "CppUTestExt/MockSupport.h"

void Timer0_SetTimerBitWidth(Timer0_TimerBitWidth timerBitWidth)
{
  mock().actualCall("Timer0_SetTimerBitWidth")
        .withParameter("timerBitWidth", timerBitWidth);
}

void Timer0_ClearTimerOnMatch(BOOL clearOnMatchFlag)
{
  mock().actualCall("Timer0_ClearTimerOnMatch")
        .withParameter("clearOnMatchFlag", clearOnMatchFlag);
}

void Timer0_SetPrescaleFactor(Timer0_PrescaleFactor prescaleFactor)
{
  mock().actualCall("Timer0_SetPrescaleFactor")
        .withParameter("prescaleFactor", prescaleFactor);
}

void Timer0_SetTimerCompareValue0A(uint8_t timerCompareValue)
{
  mock().actualCall("Timer0_SetTimerCompareValue0A")
        .withParameter("timerCompareValue", timerCompareValue);
}

void Timer0_SetTimerCompareInterrupt0A(BOOL enableInterrupt)
{
  mock().actualCall("Timer0_SetTimerCompareInterrupt0A")
        .withParameter("enableInterrupt", enableInterrupt);
}
