extern "C"
{
  #include "ChipFunctions.h"
  #include <avr/interrupt.h>
  #include <avr/io.h>
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_ChipFunctions.h"

TEST_GROUP(ChipFunctions)
{
  void setup()
  {
    SREG = 0;
    CLKPR = 0b0011;
  }

  void teardown()
  {
  }
};

TEST(ChipFunctions, AllRegistersSetToFactoryDefaultsAfterSetup)
{
  BYTES_EQUAL(0, SREG);
  BYTES_EQUAL(0b0011, CLKPR);
}

TEST(ChipFunctions, HardwareSetup)
{
  uint8_t expectedCLKPR = 0;
  SET_BITMASK_TO(expectedCLKPR, CF_CPU_PRESCALE_FACTOR_1, BITMASK_CF_PRESCALE_FACTOR);
  ChipFunctions_HwSetup();
  BYTES_EQUAL(expectedCLKPR, CLKPR);
}

TEST(ChipFunctions, SetGlobalInterruptBit)
{
  ChipFunctions_EnableGlobalInterrupts();
  BYTES_EQUAL(0x80, SREG);
}

TEST(ChipFunctions, ClearGlobalInterruptBit)
{
  SREG = 0xff;
  ChipFunctions_DisableGlobalInterrupts();
  BYTES_EQUAL(0xff & ~0x80, SREG);
}

TEST(ChipFunctions, SetAllCpuPrescaleFactorBits)
{
  //0x0f isn't a valid prescaler value, so test all bits in two steps
  ChipFunctions_SetCpuPrescaler(CF_CPU_PRESCALE_FACTOR_256);
  BYTES_EQUAL(0x08, CLKPR);
  ChipFunctions_SetCpuPrescaler(CF_CPU_PRESCALE_FACTOR_128);
  BYTES_EQUAL(0x07, CLKPR);
}

TEST(ChipFunctions, ClearAllCpuPrescaleFactorBits)
{
  CLKPR = 0xff;
  ChipFunctions_SetCpuPrescaler(CF_CPU_PRESCALE_FACTOR_1);
  BYTES_EQUAL(0xf0, CLKPR);
}
