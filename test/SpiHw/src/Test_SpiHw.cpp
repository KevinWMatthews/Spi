extern "C"
{
  #include "SpiHw.h"
  #include <avr/io.h>
  #include "BitManip.h"
  #include "DataTypes.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_SpiHw.h"

TEST_GROUP(SpiHw)
{
  uint8_t expectedBitmask;

  void setup()
  {
    expectedBitmask = 0;
    USICR = 0;
    USISR = 0;
    USIDR = 0;
  }

  void teardown()
  {
  }
};

TEST(SpiHw, RegistersClearedAfterSetup)
{
  BYTES_EQUAL(0, USICR);
  BYTES_EQUAL(0, USISR);
  BYTES_EQUAL(0, USIDR);
}

TEST(SpiHw, ClearCounterOverflowInterruptFlag)
{
  SET_BIT_NUMBER(expectedBitmask, USIOIF);

  SpiHw_ClearCounterOverflowInterruptFlag();
  BYTES_EQUAL(expectedBitmask, USISR);
}

TEST(SpiHw, EnableCounterOverflowInterrupts)
{
  SET_BIT_NUMBER(expectedBitmask, USIOIE);

  SpiHw_SetCounterOverflowInterrupts(TRUE);
  BYTES_EQUAL(expectedBitmask, USICR);
}

TEST(SpiHw, DisableCounterOverflowInterrupts)
{
  USICR = 0xff;
  expectedBitmask = 0xff;
  CLEAR_BIT_NUMBER(expectedBitmask, USIOIE);

  SpiHw_SetCounterOverflowInterrupts(FALSE);
  BYTES_EQUAL(expectedBitmask, USICR);
}
