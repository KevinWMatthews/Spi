#ifndef Timer0_ATtiny861_H
#define Timer0_ATtiny861_H

#include "DataTypes.h"
#include <avr/io.h>



typedef enum
{
  T0_EIGHT_BIT   = 0,
  T0_SIXTEEN_BIT = 1
} Timer0_TimerBitWidth;
#define BITMASK_T0_TIMER_BIT_WIDTH (1<<TCW0)
void Timer0_SetTimerBitWidth(Timer0_TimerBitWidth timerBitWidth);

#define BITMASK_T0_CLEAR_TIMER_ON_MATCH (1<<CTC0)
void Timer0_ClearTimerOnMatch(BOOL clearOnMatchFlag);

typedef enum
{
  T0_TIMER_STOPPED      = 0b000,
  T0_NO_PRESCALING      = 0b001,
  T0_PRESCALE_FACTOR_8  = 0b010,
  T0_PRESCALE_FACTOR_64 = 0b011,
  T0_EXTERNAL_T0_RISING = 0b111
} Timer0_PrescaleFactor;
#define BITMASK_T0_PRESCALE_FACTOR ((1<<CS02) | (1<<CS01) | (1<<CS00))
void Timer0_SetPrescaleFactor(Timer0_PrescaleFactor prescaleFactor);

void Timer0_SetTimerCompareValue0A(uint8_t timerCompareValue);

#define BITMASK_T0_COMPARE_INTERRUPT_0A (1<<OCIE0A)
void Timer0_SetTimerCompareInterrupt0A(BOOL enableInterrupt);



#endif
