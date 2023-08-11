/*
 * Switch_Task.cpp
 *
 * Created: 29-07-2023 18:49:39
 * Author : kales
 */ 

#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

int main(void)
{
    // To make PB1 and PB5 as Input
	DDRB &=~(1<<1);
	DDRB &=~(1<<5);
	//To enable both pin Pull up resistors.
	PORTB |=(1<<1);
	PORTB |=(1<<5);
	//To define pin for led -> PD1 and PD5
	DDRD |=(1<<1);
	DDRD |=(1<<5);
	
    while (1) 
    {
		//if condition for led toggle
	//	if((PINB & 0x08)==0) //0000 1000
		if((PINB & 0x02)==0) //0000 0010
		{
			PORTD |=(1<<1);
		}
		else
		{
			PORTD &=~(1<<1);
		}
		if((PINB & 0x20)==0) //0010 000
		{
			PORTD |=(1<<5);
		}
		else
		{
			PORTD &=~(1<<5);
		}
    }
}

