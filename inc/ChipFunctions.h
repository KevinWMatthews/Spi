#ifndef ChipFunctions_H
#define ChipFunctions_H


void ChipFunctions_HwSetup(void);

//Specific bit manipulations that are general to the chip
//but specific to the Atmel Tiny26
void ChipFunctions_EnableGlobalInterrupts(void);
void ChipFunctions_DisableGlobalInterrupts(void);

//The ATtiny861 defaults to 8MHz CPU clock with a prescal factor of 8.
typedef enum
{
  CF_CPU_PRESCALE_FACTOR_1   = 0b0000,
  CF_CPU_PRESCALE_FACTOR_8   = 0b0011,  //Factory default
  CF_CPU_PRESCALE_FACTOR_128 = 0b0111,
  CF_CPU_PRESCALE_FACTOR_256 = 0b1000
} Cf_CpuPrescaleFactor;
#define BITMASK_CF_PRESCALE_FACTOR ((1<<CLKPS3) | (1<<CLKPS2) | (1<<CLKPS1) | (1<<CLKPS0))
void ChipFunctions_SetCpuPrescaler(Cf_CpuPrescaleFactor);



#endif
