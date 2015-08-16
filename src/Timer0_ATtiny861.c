#include "Timer0_ATtiny861.h"
#include "BitManip.h"

void Timer0_HwSetup(void)
{
  Timer0_SetTimerBitWidth(T0_EIGHT_BIT);
  Timer0_ClearTimerOnMatch(TRUE);
  Timer0_SetPrescaleFactor(T0_PRESCALE_FACTOR_64);
  Timer0_SetTimerCompareValue0A(125);
  Timer0_SetTimerCompareInterrupt0A(FALSE);
}

void Timer0_SetTimerBitWidth(Timer0_TimerBitWidth timerBitWidth)
{
  SHIFT_AND_SET_BITMASK_TO(TCCR0A, timerBitWidth, BITMASK_T0_TIMER_BIT_WIDTH);
}

void Timer0_ClearTimerOnMatch(BOOL clearOnMatchFlag)
{
  SHIFT_AND_SET_BITMASK_TO(TCCR0A, clearOnMatchFlag, BITMASK_T0_CLEAR_TIMER_ON_MATCH);
}

void Timer0_SetPrescaleFactor(Timer0_PrescaleFactor prescaleFactor)
{
  SHIFT_AND_SET_BITMASK_TO(TCCR0B, prescaleFactor, BITMASK_T0_PRESCALE_FACTOR);
}

void Timer0_SetTimerCompareValue0A(uint8_t timerCompareValue)
{
  OCR0A = timerCompareValue;
}

void Timer0_SetTimerCompareInterrupt0A(BOOL enableInterrupt)
{
  SHIFT_AND_SET_BITMASK_TO(TIMSK, enableInterrupt, BITMASK_T0_COMPARE_INTERRUPT_0A);
}
