extern "C"
{
  #include "SpiHw.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_SpiHw.h"

TEST_GROUP(SpiHw)
{
  void setup()
  {
  }

  void teardown()
  {
  }
};

TEST(SpiHw, WiringCheck)
{
  FAIL("Start here!");
}
