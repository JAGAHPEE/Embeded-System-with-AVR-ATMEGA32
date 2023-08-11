/*
 * Interrupt to on led.cpp
 *
 * Created: 03-08-2023 08:30:05
 * Author : kales
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include "avr/interrupt.h"

#include <util/delay.h>

int main(void)
{
    /* Replace with your application code */
	PORTD = (1<<2)|(1<<3);	//Enable Internal Pull up Resistor
	GICR = (1<<6)|(1<<7);	//Enable Global Interrupt Control Resistor 6 and 7 bit for INT0 and INT1 enable
	DDRA = 0xff;			// Define PORTA as OUTPUT
	sei();					// Interrupt Enable Function
	
    while (1) 
    {
		_delay_ms(50000);
    }
}
ISR(INT0_vect)
{
	PORTA |= (1<<0);	// Switch 1 pressed LED ON
}
ISR(INT1_vect)
{
	PORTA &= ~(1<<0);	// Switch 2 Pressed LED OFF
}

