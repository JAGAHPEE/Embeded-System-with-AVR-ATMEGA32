/*
 * GccApplication1.cpp
 *
 * Created: 30-07-2023 10:12:07
 * Author : kales
 */ 

#include <avr/io.h>
#define  F_CPU 16000000L
#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	DDRC |=(1<<7);
	
    while (1) 
    {
		PORTC |=(1<<7);
		_delay_ms(1000);
		PORTC &=~(1<<7);
		_delay_ms(1000);
    }
}

