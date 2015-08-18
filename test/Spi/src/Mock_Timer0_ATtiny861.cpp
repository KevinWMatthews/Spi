extern "C"
{
  #include "Timer0_ATtiny861.h"
}

#include "CppUTestExt/MockSupport.h"

void Timer0_SetTimerCompareInterrupt0A(BOOL enableInterrupt)
{
  mock().actualCall("Timer0_SetTimerCompareInterrupt0A")
        .withParameter("enableInterrupt", enableInterrupt);
}
