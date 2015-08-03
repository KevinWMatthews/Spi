extern "C"
{
  #include "BitManip.h"
  #include "DataTypes.h"
}

//CppUTest includes should be after your system includes
#include "CppUTest/TestHarness.h"
#include "Test_BitManip.h"

TEST_GROUP(BitManip)
{
  uint8_t eightBit;

  void setup()
  {
    eightBit = 0;
  }

  void teardown()
  {
  }
};

TEST(BitManip, SetLeastSignificantBitNumber)
{
  SET_BIT_NUMBER(eightBit, 0);

  BYTES_EQUAL(0x01, eightBit);
}

TEST(BitManip, SetMostSignificantBitNumber)
{
  SET_BIT_NUMBER(eightBit, 7);

  BYTES_EQUAL(0x80, eightBit);
}

TEST(BitManip, ClearLeastSignificantBitNumber)
{
  eightBit = 0xff;
  CLEAR_BIT_NUMBER(eightBit, 0);

  BYTES_EQUAL(0xfe, eightBit);
}

TEST(BitManip, ClearMostSignificantBitNumber)
{
  eightBit = 0xff;
  CLEAR_BIT_NUMBER(eightBit, 7);

  BYTES_EQUAL(0x7f, eightBit);
}

TEST(BitManip, IfBitNumber_False)
{
  eightBit = 0xff;
  eightBit &= ~(1<<4);

  CHECK_FALSE(IF_BIT_NUMBER(eightBit, 4));
}

TEST(BitManip, IfBitNumber_True)
{
  eightBit |= (1<<4);

  CHECK_TRUE(IF_BIT_NUMBER(eightBit, 4));
}

TEST(BitManip, SetLeastSignificantBit)
{
  SET_BITS(eightBit, (1<<0));
  BYTES_EQUAL(0x01, eightBit);
}

TEST(BitManip, SetMostSignificantBit)
{
  SET_BITS(eightBit, (1<<7));
  BYTES_EQUAL(0x80, eightBit);
}

TEST(BitManip, ClearLeastSignificantBit)
{
  eightBit = 0xff;
  CLEAR_BITS(eightBit, (1<<0));
  BYTES_EQUAL(0xfe, eightBit);
}

TEST(BitManip, ClearMostSignificantBit)
{
  eightBit = 0xff;
  CLEAR_BITS(eightBit, (1<<7));
  BYTES_EQUAL(0x7f, eightBit);
}

TEST(BitManip, IfBit_False)
{
  eightBit = 0xff;
  eightBit &= ~(1<<5);
  CHECK_FALSE(IF_BIT(eightBit, (1<<5)));
}

TEST(BitManip, IfBit_True)
{
  eightBit |= (1<<5);
  CHECK_TRUE(IF_BIT(eightBit, (1<<5)));
}

TEST(BitManip, IfBit_True2)
{
  eightBit |= (1<<5);
  CHECK_TRUE(IF_BIT(eightBit, (1<<5)));
}

TEST(BitManip, IfBitmask_NoBitsToCheck)
{
  CHECK_FALSE(IF_BITMASK(0x00, 0x00, 0));
}

TEST(BitManip, IfBitmask_CheckSingleBit_False)
{
  CHECK_FALSE(IF_BITMASK(0x01, 0x00, 0x01));
}

TEST(BitManip, IfBitmask_CheckSingleBit_True)
{
  CHECK_TRUE(IF_BITMASK(0xff, 0x01, 0x01));
}

TEST(BitManip, IfBitmask_CheckAllBits_False)
{
  CHECK_FALSE(IF_BITMASK(0xff, 0x7f, 0xff));
}

TEST(BitManip, IfBitmask_CheckAllBits_True)
{
  CHECK_TRUE(IF_BITMASK(0xff, 0xff, 0xff));
}

TEST(BitManip, IfBitmask_CheckAllBits_BitsVary_True)
{
  CHECK_TRUE(IF_BITMASK(0xaa, 0xaa, 0xff));
}

TEST(BitManip, IfBitmask_CheckAllBits_BitsVary_False)
{
  CHECK_FALSE(IF_BITMASK(0xaa, 0xab, 0xff));
}

TEST(BitManip, SetBitmask_BitsToSetLimitsValueActuallySet)
{
  SET_BITMASK_TO(eightBit, 0x3c, 0x18);
  BYTES_EQUAL(0x18, eightBit);
}

TEST(BitManip, SetBitmask_WillClearBits)
{
  eightBit = 0xff;
  SET_BITMASK_TO(eightBit, 0x00, 0x18);
  BYTES_EQUAL(eightBit, 0xe7);
}

TEST(BitManip, RightMostBit_NoBitsSet)
{
  BYTES_EQUAL(0, RIGHTMOST_BIT_NUMBER(0x00));
}

TEST(BitManip, RightMostBit_LeastSignificantBitSet)
{
  BYTES_EQUAL(0, RIGHTMOST_BIT_NUMBER(0x01));
}

TEST(BitManip, RightMostBit_MostSignificantBitSet)
{
  BYTES_EQUAL(7, RIGHTMOST_BIT_NUMBER(0x80));
}

TEST(BitManip, RightMostBit_SeveralBitsSet)
{
  BYTES_EQUAL(3, RIGHTMOST_BIT_NUMBER(0x18));
}

TEST(BitManip, SetBitmaskShiftValue_NoNewValueNoBitmaskSetsNothing)
{
  BYTES_EQUAL(0x00, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0x00));
}

TEST(BitManip, SetBitmaskShiftValue_NoBitmaskSetsNothing)
{
  BYTES_EQUAL(0x00, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0x00));
}

TEST(BitManip, SetBitmaskShiftValue_NoNewValueSetsNothing)
{
  BYTES_EQUAL(0x00, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0xff));
}

TEST(BitManip, SetBitmaskShiftValue_SetEntireBitmask)
{
  BYTES_EQUAL(0xff, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0xff));
}

TEST(BitManip, SetBitmaskShiftValue_SetLeastSignificantBit)
{
  BYTES_EQUAL(0x01, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x01, 0x01));
}

TEST(BitManip, SetBitmaskShiftValue_etMostSignificantBit)
{
  BYTES_EQUAL(0x80, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x01, 0x80));
}

TEST(BitManip, SetBitmaskShiftValue_SetBitsWithinButNotOutsideBitmask)
{
  BYTES_EQUAL(0x14, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0x14));
}

TEST(BitManip, SetBitmaskShiftValue_SetFewerBitsThanInBitmask)
{
  BYTES_EQUAL(0x42, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x42, 0xff));
}

TEST(BitManip, SetBitmaskShiftValue_NoNewValueNoBitmaskClearsNothing)
{
  eightBit = 0xff;
  BYTES_EQUAL(0xff, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0x00));
}

TEST(BitManip, SetBitmaskShiftValue_AllNewValueBitSetFullBitmaskClearsNothing)
{
  eightBit = 0xff;
  BYTES_EQUAL(0xff, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0xff));
}

TEST(BitManip, SetBitmaskShiftValue_NoBitmaskClearsNothing)
{
  eightBit = 0xff;
  BYTES_EQUAL(0xff, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0x00));
}

TEST(BitManip, SetBitmaskShiftValue_ClearEntireBitmask)
{
  eightBit = 0xff;
  BYTES_EQUAL(0x00, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0xff));
}

TEST(BitManip, SetBitmaskShiftValue_ClearLeastSignificantBit)
{
  eightBit = 0xff;
  BYTES_EQUAL(0xfe, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0x01));
}

TEST(BitManip, SetBitmaskShiftValue_ClearMostSignificantBit)
{
  eightBit = 0xff;
  BYTES_EQUAL(0x7f, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0x80));
}

TEST(BitManip, SetBitmaskShiftValue_ClearWithinButNotOutsideBitmask)
{
  eightBit = 0xff;
  BYTES_EQUAL(0xc3, SHIFT_AND_SET_BITMASK_TO(eightBit, 0x00, 0x3c));
}

TEST(BitManip, SetBitmaskShiftValue_ClearFewerBitsThanBitmask)
{
  eightBit = 0xc3;
  BYTES_EQUAL(0xff, SHIFT_AND_SET_BITMASK_TO(eightBit, 0xff, 0x3c));
}
