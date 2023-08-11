/*
 * Line Follower Using ATMEGA32.cpp
 *
 * Created: 11-08-2023 09:04:34
 * Author : kales
 */ 
//Black Line Follower Using ATMega32
#include <avr/io.h>
#define F_CPU 16000000L
#include <util/delay.h>

// Functions for Motor Direction Control
void frwd()
{
	PORTB |=(1<<0)|(1<<2);
	PORTB &= ~(1<<1)|(1<<3);
	_delay_ms(500);
}
void stop()
{
	PORTB=0x00;
}
void rgt()
{
	PORTB |=(1<<0)|(1<<3);
	PORTB &= ~(1<<1)|(1<<2);
	_delay_ms(500);
}
void lft()
{
	PORTB |=(1<<1)|(1<<2);
	PORTB &= ~(1<<0)|(1<<3);
	_delay_ms(500);
}
int main(void)
{
	//Output Pins..
	DDRB |=(1<<0)|(1<<1)|(1<<2)|(1<<3);
	//Input Pins..
	DDRC &=~(1<<0)|(1<<1);
	//To Store Sensor Values
	int left_sensor = 0;
	int right_sensor = 0;
    while (1) 
    {
		//Getting Sensor Data and Store in Variable
		left_sensor = PINC & 0x01;
		right_sensor = PINC & 0x02;
		// Left HIGH and Right HIGH
		if((left_sensor==0x01)& (right_sensor == 0x02))
		{
			frwd();
			_delay_ms(100);
		}
		// Left LOW and Right HIGH 
		else if ((left_sensor==0x00)& (right_sensor == 0x02))
		{
			lft();
		}
		// Left HIGH and Right LOW
		else if ((left_sensor==0x01)& (right_sensor == 0x00))
		{
			rgt();
		}
		// Left LOW and Right LOW
		else
		{
			stop();
			_delay_ms(100);
		}
    }
}

