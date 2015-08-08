//******************************************
//***** Watch out!!!
//***** You have 5 seconds to tie the input and output lines together.
//***** Do NOT leave the lines tied together when programming the chip.
//*******************************************

#ifndef F_CPU
#define F_CPU 1000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <stdint.h>
uint8_t inputData = 0;
uint8_t outputData = 0;

void flash_led(int num_flashes)
{
  int i;
  for (i = 0; i < num_flashes * 2; i++)
  {
    _delay_ms(200);
    PORTB ^= (1<<PB3);
  }
  PORTB &= ~(1<<PB3);
  _delay_ms(500);
}

int main(void)
{
  //Configure LED segments
  DDRA = 0xff;
  PORTA = 0;        //Turn on all LED segments for fun.
  DDRB |= (1<<PB3); //Status LED

  // Configure port directions.
  DDRB |= (1<<PB1) | (1<<PB2);  // Outputs: Output and clock lines
  DDRB &= ~(1<<PB0);            // Inputs: Input line

  //Clear the interrupt flag
  USISR = (1<<USIOIF);

  //Enable USI counter overflow interrupts
  //Set to three-wire mode
  //Set external clock that software strobes, i.e. you toggle the clock pin
  //and the chip automatically handles the counter and the bit shift.
  USICR = (1<<USIOIE) | (1<<USIWM0) |
          (1<<USICS1) | (1<<USICLK);

  USIDR = 0b11100000;
  sei();

  _delay_ms(7000);  //You have 7 seconds to wire the input and output lines together!
  while (1)
  {
    //Toggle the clock port pin, increment counter, shift output bit, set output line
    //Nifty!
    USICR |= (1<<USITC);
  }
}

ISR(USI_OVF_vect)
{
  USISR |= (1<<USIOIF); //Clear the interrupt flag
  inputData = USIDR;    //Copy input out of data register
  if (inputData != outputData)
  {
    flash_led(3);
  }
  outputData++;
  USIDR = outputData;  //Put new data in the data register

  _delay_ms(500);
  PORTB ^= (1<<PB3);
  _delay_ms(500);
  PORTB ^= (1<<PB3);
}
