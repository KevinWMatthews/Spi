extern "C"
{
  #include "Spi.h"
  #include "SpiHw.h"
  #include <avr/io.h>
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
    DDRA  = 0;
    PORTA = 0;
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
  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(TRUE);
  LONGS_EQUAL(SPI_WRITE_IN_PROGRESS, Spi_SendData(outputData));
}

TEST(Spi, SpiSendFailsIfAnotherSlaveIsSelected)
{
  uint8_t outputData = 0x42;
  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_IsAnySlaveSelected")
        .andReturnValue(TRUE);
  LONGS_EQUAL(SPI_SLAVE_SELECTED, Spi_SendData(outputData));
}

TEST(Spi, SpiSendFailsIfUsiCounterIsNotZero)
{
  uint8_t outputData = 0x42;
  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_IsAnySlaveSelected")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_GetUsiCounter")
        .andReturnValue(0x01);
  LONGS_EQUAL(SPI_USI_COUNTER_NONZERO, Spi_SendData(outputData));
}

TEST(Spi, SpiSendTransmitsAllData)
{
  uint8_t outputData = 0x42;

  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_IsAnySlaveSelected")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_GetUsiCounter")
        .andReturnValue(0);
  mock().expectOneCall("SpiHw_PrepareOutputData")
        .withParameter("data", outputData);
  // mock().expectOneCall("SpiHw_SelectSlave")
  //       .withParameter("slave", SPIHW_SLAVE_1);

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
  // mock().expectOneCall("SpiHw_ReleaseSlave")
  //       .withParameter("slave", SPIHW_SLAVE_1);

  LONGS_EQUAL(SPI_SUCCESS, Spi_SendData(outputData));
}

TEST(Spi, SetupSlaveSelectPinFailsIfDdrIsNull)
{
  SpiSlaveSelectPin slaveSelect;
  slaveSelect = Spi_SlaveSetup(NULL, &PORTA, PINA0);
  BYTES_EQUAL(0, DDRA);
  BYTES_EQUAL(0, PORTA);
  POINTERS_EQUAL(NULL, slaveSelect);
}

TEST(Spi, SetupSlaveSelectPinFailsIfPortIsNull)
{
  SpiSlaveSelectPin slaveSelect;
  slaveSelect = Spi_SlaveSetup(&DDRA, NULL, PINA0);
  BYTES_EQUAL(0, DDRA);
  BYTES_EQUAL(0, PORTA);
  POINTERS_EQUAL(NULL, slaveSelect);
}

TEST(Spi, SetupSlaveSelectFailsIfPinBitGreaterTooLarge)
{
  SpiSlaveSelectPin slaveSelect;
  slaveSelect = Spi_SlaveSetup(&DDRA, &PORTA, 8);
  BYTES_EQUAL(0, DDRA);
  BYTES_EQUAL(0, PORTA);
  POINTERS_EQUAL(NULL, slaveSelect);
}

TEST(Spi, SetupSlaveSelectPinSetsDdrAndPortBits)
{
  RegisterPointer ddr = &DDRA;
  RegisterPointer portToSet = &PORTA;
  uint8_t pinToSet = PINA0;

  mock().expectOneCall("SpiHw_ReleaseSlave")
        .withParameter("port", (uint8_t *)portToSet)
        .withParameter("bit", pinToSet);

  SpiSlaveSelectPin slaveSelect;
  slaveSelect = Spi_SlaveSetup(ddr, portToSet, pinToSet);
  CHECK(slaveSelect != NULL);
}

// TEST(Spi, SelectSlave)
// {
//   uint8_t pinToSet = PINA0;

//   SpiSlaveSelectPin slaveSelect;
//   PORTA = 0xff;

//   slaveSelect = Spi_SlaveSetup(&DDRA, &PORTA, PINA0);
//   Spi_SelectSlave(slaveSelect);
//   BYTES_EQUAL(0xff & ~(1<<pinToSet), PORTA);
// }

// TEST(Spi, ReleaseSlave)
// {
//   uint8_t pinToSet = PINA0;

//   SpiSlaveSelectPin slaveSelect;

//   slaveSelect = Spi_SlaveSetup(&DDRA, &PORTA, PINA0);
//   Spi_SelectSlave(slaveSelect);
//   Spi_ReleaseSlave(slaveSelect);
//   BYTES_EQUAL((1<<pinToSet), PORTA);
// }
