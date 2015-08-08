extern "C"
{
  #include "Spi.h"
  #include "SpiHw.h"
}
#include "Mock_SpiHw_ATtiny861.h"

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
  mock().expectOneCall("SpiHw_SetIsTransmittingFlag")
        .withParameter("isTransmitting", FALSE);

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
  mock().expectOneCall("SpiHw_SetIsTransmittingFlag")
        .withParameter("isTransmitting", FALSE);
  mock().expectOneCall("SpiHw_SaveInputData")
        .andReturnValue(mockUsidr);
  Spi_UsiOverflowInterrupt();
  LONGS_EQUAL(mockUsidr, Spi_GetInputData());
}

TEST(Spi, SpiSendFailsIfTransmissionInProgressFlagIsSet)
{
  uint8_t outputData = 0x42;
  mock().expectOneCall("SpiHw_PrepareOutputData")
        .withParameter("data", outputData)
        .andReturnValue(SPIHW_WRITE_IN_PROGRESS);
  LONGS_EQUAL(SPI_WRITE_IN_PROGRESS, Spi_SendData(outputData));
}

TEST(Spi, SpiSendFailsIfUsiCounterIsNotZero)
{
  uint8_t outputData = 0x42;
  mock().expectOneCall("SpiHw_PrepareOutputData")
        .withParameter("data", outputData)
        .andReturnValue(SPIHW_USI_COUNTER_NONZERO);
  LONGS_EQUAL(SPI_USI_COUNTER_ERROR, Spi_SendData(outputData));
}

TEST(Spi, SpiSendTransmitsAllData)
{
  uint8_t outputData = 0x42;

  mock().expectOneCall("SpiHw_PrepareOutputData")
        .withParameter("data", outputData)
        .andReturnValue(SPIHW_WRITE_STARTED);

  mock().expectOneCall("SpiHw_ToggleUsiClock");
  for (uint8_t i = 0; i < SPI_DATA_REGISTER_SIZE * 2 - 1; i++)
  {
    mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
          .andReturnValue(TRUE);
    mock().expectOneCall("SpiHw_ToggleUsiClock");
  }
  mock().expectOneCall("SpiHw_SetIsTransmittingFlag")
        .withParameter("isTransmitting", FALSE);
  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);

  LONGS_EQUAL(SPI_SUCCESS, Spi_SendData(outputData));
}
