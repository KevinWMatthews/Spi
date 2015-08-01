#ifndef io_H
#define io_H

#include <stdint.h>

//*** Port pins ***//
extern uint8_t DDRB;
// #define DDRB _SFR_IO8(0x17)
#define DDB0 0
#define DDB1 1
#define DDB2 2
#define DDB3 3
#define DDB4 4
#define DDB5 5
#define DDB6 6
#define DDB7 7

// #define PORTB _SFR_IO8(0x18)
extern uint8_t PORTB;
#define PORTB0 0
#define PORTB1 1
#define PORTB2 2
#define PORTB3 3
#define PORTB4 4
#define PORTB5 5
#define PORTB6 6
#define PORTB7 7



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

//USI Pin Position
extern uint8_t USIPP;
// #define USIPP _SFR_IO8(0x11)
#define USIPOS 0


#endif
