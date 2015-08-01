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

TEST(SpiHw, SetAllWiringModeBits)
{
  SET_BIT_NUMBER(expectedBitmask, USIWM1);
  SET_BIT_NUMBER(expectedBitmask, USIWM0);

  SpiHw_SetWireMode(USI_TWO_WIRE_2);
  BYTES_EQUAL(expectedBitmask, USICR);
}

TEST(SpiHw, ClearAllWiringModeBits)
{
  USICR = 0xff;
  expectedBitmask = 0xff;
  CLEAR_BIT_NUMBER(expectedBitmask, USIWM1);
  CLEAR_BIT_NUMBER(expectedBitmask, USIWM0);

  SpiHw_SetWireMode(USI_PARTIAL_DISABLE);
  BYTES_EQUAL(expectedBitmask, USICR);
}

TEST(SpiHw, SetAllClockSourceBits)
{
  SET_BIT_NUMBER(expectedBitmask, USICS1);
  SET_BIT_NUMBER(expectedBitmask, USICS0);
  SET_BIT_NUMBER(expectedBitmask, USICLK);

  SpiHw_SetClockSource(USI_EXTERNAL_NEGATIVE_EDGE_SOFTWARE_STROBE);
  BYTES_EQUAL(expectedBitmask, USICR);
}

TEST(SpiHw, ClearAllClockSourceBits)
{
  USICR = 0xff;
  expectedBitmask = 0xff;
  CLEAR_BIT_NUMBER(expectedBitmask, USICS1);
  CLEAR_BIT_NUMBER(expectedBitmask, USICS0);
  CLEAR_BIT_NUMBER(expectedBitmask, USICLK);

  SpiHw_SetClockSource(USI_NONE);
  BYTES_EQUAL(expectedBitmask, USICR);
}

TEST(SpiHw, PrepareOutputData)
{
  uint8_t sampleData = 0b11001010;
  SpiHw_PrepareOutputData(sampleData);
  BYTES_EQUAL(sampleData, USIDR);
}
