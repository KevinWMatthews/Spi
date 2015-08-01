extern "C"
{
  #include "SpiHw.h"
  #include <avr/io.h>
  #include "BitManip.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_SpiHw.h"

TEST_GROUP(SpiHw)
{
  void setup()
  {
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

TEST(SpiHw, ClearOverflowInterruptFlag)
{
  uint8_t expectedBitmask = 0;
  SET_BIT_NUMBER(expectedBitmask, USIOIF);

  SpiHw_ClearOverflowInterruptFlag();
  BYTES_EQUAL(expectedBitmask, USISR);
}
