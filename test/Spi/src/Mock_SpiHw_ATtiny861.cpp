extern "C"
{
  #include "SpiHw.h"
}

#include "CppUTestExt/MockSupport.h"

void SpiHw_SetSlaveSelect(SpiHw_Slave slave)
{
  mock().actualCall("SpiHw_SetSlaveSelect")
        .withParameter("slave", slave);
}
