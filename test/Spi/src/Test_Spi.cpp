extern "C"
{
  #include "Spi.h"
  #include "SpiHw.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "Test_Spi.h"

TEST_GROUP(Spi)
{
  void setup()
  {
    mock().strictOrder();
  }

  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  }
};

TEST(Spi, HwSetup)
{
  mock().expectOneCall("SpiHw_SetWireMode")
        .withParameter("wireMode", USI_THREE_WIRE);
  mock().expectOneCall("SpiHw_SetClockSource")
        .withParameter("clockSource", USI_EXTERNAL_POSITIVE_EDGE_SOFTWARE_STROBE);
  mock().expectOneCall("SpiHw_ConfigureUsiPins")
        .withParameter("pinPosition", USI_PORTB_PINS);
  mock().expectOneCall("SpiHw_SetCounterOverflowInterrupts")
        .withParameter("enableInterrupts", TRUE);

  Spi_HwSetup();
}

TEST(Spi, SelectSlave)
{
  mock().expectOneCall("SpiHw_SetSlaveSelect")
        .withParameter("slave", SPI_HW_SLAVE1);
  LONGS_EQUAL(SPI_SUCCESS, Spi_SelectSlave(SPI_SLAVE1));
}

TEST(Spi, UsiCounterOverflowInterrupt)
{
  uint8_t mockUsidr = 42;
  mock().expectOneCall("SpiHw_ClearCounterOverflowInterruptFlag");
  mock().expectOneCall("SpiHw_SaveInputData")
        .andReturnValue(mockUsidr);
  Spi_UsiOverflowInterrupt();
  LONGS_EQUAL(mockUsidr, Spi_GetInputData());
}

TEST(Spi, StartTransmission)
{
  //Check if another transmission is in progress
  //Prep data
  //Send data
}
