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
    DDRA  = 0;
    PORTA = 0;
    SpiHw_SetIsTransmittingFlag(FALSE);
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
  BYTES_EQUAL(0, DDRA);
  BYTES_EQUAL(0, PORTA);
  CHECK(!SpiHw_GetIsTransmittingFlag());
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

TEST(SpiHw, PrepareOutputDataSetsIsTransmittingFlag)
{
  uint8_t sampleData = 0xa5;
  SpiHw_PrepareOutputData(sampleData);
  BYTES_EQUAL(sampleData, USIDR);
  CHECK(SpiHw_GetIsTransmittingFlag());
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

TEST(SpiHw, SetisTransmittingFlag)
{
  SpiHw_SetIsTransmittingFlag(TRUE);
  CHECK(SpiHw_GetIsTransmittingFlag());
  SpiHw_SetIsTransmittingFlag(FALSE);
  CHECK(!SpiHw_GetIsTransmittingFlag());
}

TEST(SpiHw, GetUsiCounter)
{
  LONGS_EQUAL(0, SpiHw_GetUsiCounter());

  //Set all bits that arenot in the counter to ensure that they have no effect
  SET_BITMASK_TO(USISR, 0xff, ~(BITMASK_USI_COUNTER));
  //Set bits within the counter
  SET_BITMASK_TO(USISR, 0x0a, BITMASK_USI_COUNTER);
  BYTES_EQUAL(0x0a, SpiHw_GetUsiCounter());
}

TEST(SpiHw, ResetUsiCounter)
{
  //Set all bits that are not in the counter to ensure that they are unaffected
  SET_BITMASK_TO(USISR, 0xff, ~(BITMASK_USI_COUNTER));
  SET_BITMASK_TO(USISR, 0x0a, BITMASK_USI_COUNTER);
  SpiHw_ClearUsiCounter();
  BYTES_EQUAL(0x00, SpiHw_GetUsiCounter());
  BYTES_EQUAL( USISR & ~(BITMASK_USI_COUNTER), 0xf0);
}

TEST(SpiHw, SelectSlavePullsPinLow)
{
  PORTA = 0xff;

  SpiHw_SelectSlave(&PORTA, PINA7);
  BYTES_EQUAL(0x7f, PORTA);
}

TEST(SpiHw, SelectSlaveFailsIfRegisterIsNull)
{
  PORTA = 0xff;

  SpiHw_SelectSlave(NULL, PINA7);
  BYTES_EQUAL(0xff, PORTA);
}

TEST(SpiHw, ReleaseSlavePullsPinHigh)
{
  SpiHw_ReleaseSlave(&PORTA, PINA7);
  BYTES_EQUAL(0x80, PORTA);
}

TEST(SpiHw, ReleaseSlaveFailsIfRegisterIsNull)
{
  SpiHw_ReleaseSlave(NULL, PINA7);
  BYTES_EQUAL(0, PORTA);
}

TEST(SpiHw, AllSlavesAreReleased)
{
  CHECK(!SpiHw_IsAnySlaveSelected());
}

IGNORE_TEST(SpiHw, SlaveIsSelected)
{
  //Set up preconditions
  CHECK(SpiHw_IsAnySlaveSelected());
}
