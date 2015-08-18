extern "C"
{
  #include "Timer0_ATtiny861.h"
  #include "BitManip.h"
  #include "DataTypes.h"
  #include <avr/io.h>
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_Timer0_ATtiny861.h"

TEST_GROUP(Timer0_ATtiny861)
{
  uint8_t expected;

  void setup()
  {
    expected = 0;
    TCCR0A   = 0;
    TCCR0B   = 0;
    OCR0A    = 0;
    TIMSK    = 0;
  }

  void teardown()
  {
  }
};

TEST(Timer0_ATtiny861, MockRegistersClearedAfterSetup)
{
  LONGS_EQUAL(0, expected);
  BYTES_EQUAL(0, TCCR0A);
  BYTES_EQUAL(0, TCCR0B);
  BYTES_EQUAL(0, OCR0A);
  BYTES_EQUAL(0, TIMSK);
}

TEST(Timer0_ATtiny861, SetAllTimerBitWidthBits)
{
  SET_BITS(expected, BITMASK_T0_TIMER_BIT_WIDTH);

  Timer0_SetTimerBitWidth(T0_SIXTEEN_BIT);

  BYTES_EQUAL(expected, TCCR0A);
}

TEST(Timer0_ATtiny861, ClearAllTimerBitWidthBits)
{
  TCCR0A = 0xff;
  expected = 0xff;
  CLEAR_BITS(expected, BITMASK_T0_TIMER_BIT_WIDTH);

  Timer0_SetTimerBitWidth(T0_EIGHT_BIT);

  BYTES_EQUAL(expected, TCCR0A);
}

TEST(Timer0_ATtiny861, SetAllTimerOnMatchBits)
{
  SET_BITS(expected, BITMASK_T0_CLEAR_TIMER_ON_MATCH);

  Timer0_ClearTimerOnMatch(TRUE);

  BYTES_EQUAL(expected, TCCR0A);
}

TEST(Timer0_ATtiny861, ClearAllTimerOnMatchBits)
{
  TCCR0A = 0xff;
  expected = 0xff;
  CLEAR_BITS(expected, BITMASK_T0_CLEAR_TIMER_ON_MATCH);

  Timer0_ClearTimerOnMatch(FALSE);

  BYTES_EQUAL(expected, TCCR0A);
}

TEST(Timer0_ATtiny861, SetAllPrescalerBits)
{
  SET_BITS(expected, BITMASK_T0_PRESCALE_FACTOR);
  Timer0_SetPrescaleFactor(T0_EXTERNAL_T0_RISING);
  BYTES_EQUAL(expected, TCCR0B);
}

TEST(Timer0_ATtiny861, ClearAllPrescalerBits)
{
  TCCR0B = 0xff;
  expected = 0xff;
  CLEAR_BITS(expected, BITMASK_T0_PRESCALE_FACTOR);

  Timer0_SetPrescaleFactor(T0_TIMER_STOPPED);

  BYTES_EQUAL(expected, TCCR0B);
}

TEST(Timer0_ATtiny861, SetMaxTimerCompareValue)
{
  Timer0_SetTimerCompareValue0A(255);
  LONGS_EQUAL(OCR0A, 255);
}

TEST(Timer0_ATtiny861, SetMinTimerCompareValue)
{
  Timer0_SetTimerCompareValue0A(0);
  LONGS_EQUAL(OCR0A, 0);
}

TEST(Timer0_ATtiny861, SetAllEnableTimerCompareInterruptBits)
{
  SET_BITS(expected, BITMASK_T0_COMPARE_INTERRUPT_0A);

  Timer0_SetTimerCompareInterrupt0A(TRUE);

  BYTES_EQUAL(expected, TIMSK);
}

TEST(Timer0_ATtiny861, ClearAllEnableTimerCompareInterruptBits)
{
  TIMSK = 0xff;
  expected = 0xff;
  CLEAR_BITS(expected, BITMASK_T0_COMPARE_INTERRUPT_0A);

  Timer0_SetTimerCompareInterrupt0A(FALSE);

  BYTES_EQUAL(expected, TIMSK);
}
