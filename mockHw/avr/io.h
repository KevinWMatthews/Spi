#ifndef io_H
#define io_H

#include <stdint.h>

//*** Universial Serial Interface registers ***//
//USI Control Register
// #define USICR _SFR_IO8(0x0D)
extern uint8_t USICR;
#define USITC 0
#define USICLK 1
#define USICS0 2
#define USICS1 3
#define USIWM0 4
#define USIWM1 5
#define USIOIE 6
#define USISIE 7

//USI Status Register
// #define USISR _SFR_IO8(0x0E)
extern uint8_t USISR;
#define USICNT0 0
#define USICNT1 1
#define USICNT2 2
#define USICNT3 3
#define USIDC 4
#define USIPF 5
#define USIOIF 6
#define USISIF 7

//USI Data Register
// #define USIDR _SFR_IO8(0x0F)
extern uint8_t USIDR;
#define USIDR0 0
#define USIDR1 1
#define USIDR2 2
#define USIDR3 3
#define USIDR4 4
#define USIDR5 5
#define USIDR6 6
#define USIDR7 7

#endif
