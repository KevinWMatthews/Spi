#ifndef io_H
#define io_H

//These variables hail from chip-specific io files.
//Rather than recreate the entire nested file structure,
//I put them all here until I have a reason for a more complex structure.

#include <avr/common.h>
#include <stdint.h>


//*** CPU prescaler ***//
extern uint8_t CLKPR;
// #define CLKPR _SFR_IO8(0x28)
#define CLKPS0 0
#define CLKPS1 1
#define CLKPS2 2
#define CLKPS3 3
#define CLKPCE 7


//*** Global Interrupts ***//
//SREG is in common.h


//*** Port pins ***//
extern uint8_t DDRA;
// #define DDRA _SFR_IO8(0x1A)
#define DDA0 0
#define DDA1 1
#define DDA2 2
#define DDA3 3
#define DDA4 4
#define DDA5 5
#define DDA6 6
#define DDA7 7

extern uint8_t PORTA;
// #define PORTA _SFR_IO8(0x1B)
#define PORTA0 0
#define PORTA1 1
#define PORTA2 2
#define PORTA3 3
#define PORTA4 4
#define PORTA5 5
#define PORTA6 6
#define PORTA7 7



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
