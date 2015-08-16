#ifndef io_H
#define io_H

//These variables hail from chip-specific io files.
//Rather than recreate the entire nested file structure,
//I put them all here until I have a reason for a more complex structure.

//Refer to iotnx61.h, iotn861a.h

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
// #define PINA _SFR_IO8(0x19)
#define PINA0 0
#define PINA1 1
#define PINA2 2
#define PINA3 3
#define PINA4 4
#define PINA5 5
#define PINA6 6
#define PINA7 7

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



//***************//
//*** Timer 0 ***//
//***************//
//Timer/Counter0 Control Register A
extern uint8_t TCCR0A;
// #define TCCR0A _SFR_IO8(0x15)
//#define WGM00 0   /* up to at least datasheet rev. B */
#define CTC0  0   /* newer revisions; change not mentioned
                   * in revision history */
#define ACIC0 3
#define ICES0 4
#define ICNC0 5
#define ICEN0 6
#define TCW0 7


//Timer/Counter0 Control Register B
extern uint8_t TCCR0B;
// #define TCCR0B  _SFR_IO8(0x33)
#define CS00    0
#define CS01    1
#define CS02    2
#define PSR0    3
#define TSM     4

//Timer/Counter0 Output Compare Register A
extern uint8_t OCR0A;
// #define OCR0A   _SFR_IO8(0x13)

//Timer/Counter0 Interrupt Mask Register
extern uint8_t TIMSK;
// #define TIMSK   _SFR_IO8(0x39)
#define TICIE0  0
#define TOIE0   1
#define TOIE1   2
#define OCIE0B  3
#define OCIE0A  4
#define OCIE1B  5
#define OCIE1A  6
#define OCIE1D  7


#endif
