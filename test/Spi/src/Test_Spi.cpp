extern "C"
{
  #include "Spi.h"
  #include "SpiHw.h"
  #include <avr/io.h>
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"
#include "Test_Spi.h"

TEST_GROUP(Spi)
{
  RegisterPointer ddr;
  RegisterPointer port;
  uint8_t         bit;
  SpiSlaveSelectPin slave;

  void setup()
  {
    slave = NULL; //Until we have destroy()
    ddr  = &DDRA;
    port = &PORTA;
    bit  = PINA1;
    DDRA  = 0;
    PORTA = 0;
    mock().strictOrder();
  }

  void teardown()
  {
    mock().checkExpectations();
    mock().clear();
  }

  void expectSlaveSetup(RegisterPointer ddr, RegisterPointer port, uint8_t bit)
  {
    mock().expectOneCall("SpiHw_ReleaseSlave")
          .withParameter("port", (uint8_t *)port)
          .withParameter("bit", bit);
    mock().expectOneCall("SpiHw_SetPinAsOutput")
          .withParameter("dataDirectionRegister", (uint8_t *)ddr)
          .withParameter("bit", bit);
  }

  void expectCompleteTransmit(SpiSlaveSelectPin slave, uint8_t outputData)
  {
    //Sanity checks
    mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
          .andReturnValue(FALSE);
    mock().expectOneCall("SpiHw_IsAnySlaveSelected")
          .andReturnValue(FALSE);
    mock().expectOneCall("SpiHw_GetUsiCounter")
          .andReturnValue(0);
    //Load data register
    mock().expectOneCall("SpiHw_PrepareOutputData")
          .withParameter("data", outputData);
    //Select slave
    mock().expectOneCall("SpiHw_SelectSlave")
          .withParameter("port", (uint8_t *)Spi_GetSlavePortPointer(slave))
          .withParameter("bit", Spi_GetSlaveBit(slave));

    //Toggle clock and check flag
    mock().expectOneCall("SpiHw_ToggleUsiClock");
    for (uint8_t i = 0; i < SPIHW_DATA_REGISTER_SIZE * 2 - 1; i++)
    {
      mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
            .andReturnValue(TRUE);
      mock().expectOneCall("SpiHw_ToggleUsiClock");
    }
    //USI overflow interrupt
    mock().expectOneCall("SpiHw_ClearCounterOverflowInterruptFlag");
    mock().expectOneCall("SpiHw_SetIsTransmittingFlag")
          .withParameter("isTransmitting", FALSE);
    mock().expectOneCall("SpiHw_SaveInputData");
    //Flag check
    mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
          .andReturnValue(FALSE);
    //Release slave
    mock().expectOneCall("SpiHw_ReleaseSlave")
          .withParameter("port", (uint8_t *)Spi_GetSlavePortPointer(slave))
          .withParameter("bit", Spi_GetSlaveBit(slave));
  }
};



//HwSetup
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


//Interrupt/GetInputData
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


//Slave select
TEST(Spi, SlaveSetupSelectPinFailsIfDdrIsNull)
{
  slave = Spi_SlaveSetup(NULL, port, bit);
  POINTERS_EQUAL(NULL, slave);
}

TEST(Spi, SlaveSetupSelectPinFailsIfPortIsNull)
{
  slave = Spi_SlaveSetup(ddr, NULL, bit);
  POINTERS_EQUAL(NULL, slave);
}

TEST(Spi, SlaveSetupSelectFailsIfPinBitGreaterTooLarge)
{
  slave = Spi_SlaveSetup(ddr, port, 8);
  POINTERS_EQUAL(NULL, slave);
}

TEST(Spi, SlaveSetupSelectPinSetsDdrAndPortBits)
{
  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);
  CHECK(slave != NULL);
  LONGS_EQUAL(bit, Spi_GetSlaveBit(slave));
  POINTERS_EQUAL(port, Spi_GetSlavePortPointer(slave));
}

//Accessors
TEST(Spi, GetSlaveBitFailsIfSlaveIsNull)
{
  LONGS_EQUAL(0, Spi_GetSlaveBit(NULL));
}

TEST(Spi, GetPortRegisterPointerFailsIfSlaveIsNull)
{
  POINTERS_EQUAL(NULL, Spi_GetSlavePortPointer(NULL));
}

//Release
TEST(Spi, ReleaseSlave)
{
  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  mock().expectOneCall("SpiHw_ReleaseSlave")
        .withParameter("port", (uint8_t *)port)
        .withParameter("bit", bit);

  Spi_ReleaseSlave(slave);
}

TEST(Spi, ReleaseFailsWithNullSlave)
{
  Spi_ReleaseSlave(NULL);
}



//Select
TEST(Spi, SelectSlave)
{
  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  mock().expectOneCall("SpiHw_SelectSlave")
        .withParameter("port", (uint8_t *)port)
        .withParameter("bit", bit);

  Spi_SelectSlave(slave);
}

TEST(Spi, SelectFailsWithNullSlave)
{
  Spi_SelectSlave(NULL);
}



//SendData
TEST(Spi, SpiSendFailsIfSlaveIsNull)
{
  Spi_SendData(NULL, 66);
}

TEST(Spi, SpiSendFailsIfTransmissionInProgressFlagIsSet)
{
  uint8_t outputData = 0x42;

  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(TRUE);

  LONGS_EQUAL(SPI_WRITE_IN_PROGRESS, Spi_SendData(slave, outputData));
}

TEST(Spi, SpiSendFailsIfAnotherSlaveIsSelected)
{
  uint8_t outputData = 0x42;

  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_IsAnySlaveSelected")
        .andReturnValue(TRUE);

  LONGS_EQUAL(SPI_SLAVE_SELECTED, Spi_SendData(slave, outputData));
}

TEST(Spi, SpiSendFailsIfUsiCounterIsNotZero)
{
  uint8_t outputData = 0x42;

  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  mock().expectOneCall("SpiHw_GetIsTransmittingFlag")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_IsAnySlaveSelected")
        .andReturnValue(FALSE);
  mock().expectOneCall("SpiHw_GetUsiCounter")
        .andReturnValue(0x01);

  LONGS_EQUAL(SPI_USI_COUNTER_NONZERO, Spi_SendData(slave, outputData));
}

TEST(Spi, SpiSendTogglesClockUntilIsTransmittingFlagIsCleared)
{
  uint8_t outputData = 0x42;

  expectSlaveSetup(ddr, port, bit);
  slave = Spi_SlaveSetup(ddr, port, bit);

  expectCompleteTransmit(slave, outputData);

  LONGS_EQUAL(SPI_SUCCESS, Spi_SendData(slave, outputData));
}

TEST(Spi, SendDataToSeveralSlaves)
{
  uint8_t outputData  = 0x42;
  uint8_t outputData2 = 0x43;
  SpiSlaveSelectPin slave2;

  expectSlaveSetup(ddr, port, PINA0);
  slave = Spi_SlaveSetup(ddr, port, PINA0);

  expectSlaveSetup(ddr, port, PINA1);
  slave2 = Spi_SlaveSetup(ddr, port, PINA1);

  expectCompleteTransmit(slave, outputData);

  LONGS_EQUAL(SPI_SUCCESS, Spi_SendData(slave, outputData));

  expectCompleteTransmit(slave2, outputData2);
  LONGS_EQUAL(SPI_SUCCESS, Spi_SendData(slave2, outputData2));
}
