extern "C"
{
  #include "Spi.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_Spi.h"

TEST_GROUP(Spi)
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

TEST(Spi, WiringCheck)
{
  FAIL("Start here!");
}
