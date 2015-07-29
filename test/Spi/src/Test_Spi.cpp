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

TEST(Spi, SelectSlave)
{
  mock().expectOneCall("SpiHw_SetSlaveSelect")
        .withParameter("slave", SPI_HW_SLAVE1);
  LONGS_EQUAL(SPI_SUCCESS, Spi_SelectSlave(SPI_SLAVE1));
}
