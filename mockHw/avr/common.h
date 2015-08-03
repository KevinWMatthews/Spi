#ifndef common_H
#define common_H

#include <stdint.h>

/* Status Register */
extern uint8_t SREG;
// #ifndef SREG
// #  if __AVR_ARCH__ >= 100
// #    define SREG _SFR_MEM8(0x3F)
// #  else
// #    define SREG _SFR_IO8(0x3F)
// #  endif
// #endif


/* SREG bit definitions */
#ifndef SREG_C
#  define SREG_C  (0)
#endif
#ifndef SREG_Z
#  define SREG_Z  (1)
#endif
#ifndef SREG_N
#  define SREG_N  (2)
#endif
#ifndef SREG_V
#  define SREG_V  (3)
#endif
#ifndef SREG_S
#  define SREG_S  (4)
#endif
#ifndef SREG_H
#  define SREG_H  (5)
#endif
#ifndef SREG_T
#  define SREG_T  (6)
#endif
#ifndef SREG_I
#  define SREG_I  (7)
#endif

#endif
