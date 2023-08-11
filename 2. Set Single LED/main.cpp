/*
 * Set_single_led.cpp
 *
 * Created: 29-07-2023 12:06:27
 * Author : kales
 */ 
#define ledon PORTB |=(1<<5)
#define ledof PORTB &=~(1<<5)
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
    DDRB |=(1<<5);
	//DDRB |=(1<<3);
    while (1) 
    {
/*
		PORTB |=(1<<3);
		_delay_ms(1000);
		PORTB &=~(1<<3);
		_delay_ms(1000);
*/
		ledon;
		_delay_ms(500);
		ledof;
		_delay_ms(500);
    }
}

