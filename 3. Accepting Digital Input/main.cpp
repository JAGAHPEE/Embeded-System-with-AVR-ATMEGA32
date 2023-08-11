/*
 * Accepting Digital Input.cpp
 *
 * Created: 29-07-2023 13:15:53
 * Author : kales
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

int main(void)
{
	//define output pin for led
	DDRC |=(1<<7);
	
	//define input pin for PORTB0 and PORTD6
	DDRB &=~(1<<0);
	
	DDRD &=~(1<<6);
	
	//to enable internal pull up resistor at pin PB0 simply set pin to high
	PORTB |=(1<<0);
	
    while (1) 
    {
		/*
		if(PINB & 0x01)
		{
			PORTC |=(1<<7);
		}
		else
		{
			PORTC &=~(1<<7);
		}
		*/
		if(!PINB & 0x01) //0000 0001
		{
			PORTC |=(1<<7);
		}
		if(PIND & 0x40 )  //0010 0000
		{
			PORTC &=~(1<<7);
		}
    }
}

