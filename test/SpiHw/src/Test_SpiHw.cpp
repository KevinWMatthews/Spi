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
    USIPP = 0;
    SpiHw_SetIsTransmissionInProgressFlag(FALSE);
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
  BYTES_EQUAL(0, USIPP);
  CHECK(!SpiHw_GetIsTransmissionInProgressFlag());
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

TEST(SpiHw, PrepareOutputDataStartsTransmission)
{
  uint8_t sampleData = 0xa5;
  LONGS_EQUAL(SPIHW_WRITE_STARTED, SpiHw_PrepareOutputData(sampleData));
  BYTES_EQUAL(sampleData, USIDR);
  CHECK(SpiHw_GetIsTransmissionInProgressFlag());
}

TEST(SpiHw, PrepareOutputDataFailsIfWriteInProgress)
{
  uint8_t sampleData = 0xa5;
  SpiHw_SetIsTransmissionInProgressFlag(TRUE);

  LONGS_EQUAL(SPIHW_WRITE_IN_PROGRESS, SpiHw_PrepareOutputData(sampleData));
  BYTES_EQUAL(0, USIDR);
  CHECK(SpiHw_GetIsTransmissionInProgressFlag());
}

TEST(SpiHw, SetPinPositionToPortB)
{
  uint8_t expectedDDRB = 0x01;
  uint8_t expectedUSIPP = 0xff;
  USIPP = 0xff;
  DDRB = 0x01;

  CLEAR_BIT_NUMBER(expectedUSIPP, USIPOS);
  CLEAR_BIT_NUMBER(expectedDDRB, DDB0); //Input:  DI/MISO
  SET_BIT_NUMBER(expectedDDRB, DDB1);   //Output: DO/MOSI
  SET_BIT_NUMBER(expectedDDRB, DDB2);   //Output: USCK

  SpiHw_ConfigureUsiPins(USI_PORTB_PINS);
  BYTES_EQUAL(expectedUSIPP, USIPP);
  BYTES_EQUAL(expectedDDRB, DDRB);
}

TEST(SpiHw, SaveInputData)
{
  USIDR = 0x53;
  BYTES_EQUAL(USIDR, SpiHw_SaveInputData());
}

TEST(SpiHw, ToggleUsiClock)
{
  SET_BIT_NUMBER(expectedBitmask, USITC);
  SpiHw_ToggleUsiClock();
  BYTES_EQUAL(expectedBitmask, USICR);
}


TEST(SpiHw, SpiHw_SetIsTransmissionInProgressFlag)
{
  SpiHw_SetIsTransmissionInProgressFlag(TRUE);
  CHECK(SpiHw_GetIsTransmissionInProgressFlag());
  SpiHw_SetIsTransmissionInProgressFlag(FALSE);
  CHECK(!SpiHw_GetIsTransmissionInProgressFlag());
}
