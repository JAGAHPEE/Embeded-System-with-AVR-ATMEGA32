/*
 * ADC With LCD.cpp
 *
 * Created: 02-08-2023 10:48:04
 * Author : kales
 */ 

#define F_CPU 16000000L
#include <avr/io.h>
#include "avr/interrupt.h"
#include <util/delay.h>

//define rs pin macro low high
#define rs_high() PORTD|=(1<<0);
#define rs_low() PORTD&=~(1<<0);

//define en pin macro low high
#define en_high() PORTD|=(1<<1);
#define en_low() PORTD&=~(1<<1);

#define lcdport PORTB

//Function to Write cmd on LCD
void lcd_cmd(unsigned char cmd)
{
	lcdport = cmd;
	rs_low();	//rs = 0 to select command register
	en_high();
	//_delay_ms(1);
	_delay_ms(50);
	en_low();
}
void lcd_data(unsigned char dat)
{
	lcdport = dat;
	rs_high();	//rs = 0 to select command register
	en_high();
	//_delay_ms(1);
	_delay_ms(50);
	en_low();
}
void lcd_init()
{
	lcd_cmd(0x38);
	_delay_ms(50);
	lcd_cmd(0x02);
	_delay_ms(50);
	lcd_cmd(0x01);
	_delay_ms(50);
	lcd_cmd(0x0c);
	_delay_ms(50);
	lcd_cmd(0x06);
	_delay_ms(50);
	lcd_cmd(0x80);
	_delay_ms(50);	
}
void cursorxy(char x, char y)
{	
	if((x<1||x>2)||(y<1||y>16))	
	{
		x=1;
		y=1;
	}
	if(x==1)
	lcd_cmd(0x7F+y);
	else
	lcd_cmd(0xBF+y);
}
//function to print input value upto the desired igit on lcd
void lcd_print(char row, char col, unsigned int value ,int digits)
{
	unsigned int thousand;
	unsigned int hundred;
	unsigned int unit;
	unsigned int tens;
	unsigned int temp;
	unsigned int million;
	unsigned char flag =0;
	if(row==0||col==0)
	{
		
		cursorxy(1,1);
	}
	else
	{
		
		cursorxy(row,col);
	}
	if(digits ==5||flag == 1)
	{
		million = value/10000+48;
		lcd_data(million);
		flag=1;
	}
	if(digits ==4||flag ==1)
	{
		temp = value/1000;
		thousand = temp%10 + 48;
		lcd_data(thousand);
		flag=1;
	}
	if(digits ==3||flag ==1)
	{
		temp = value/100; 
		hundred =temp%10 + 48;
		lcd_data(hundred);
		flag=1;
	}
	if(digits ==2||flag ==1)
	{
		temp = value/10;
		tens =temp%10+48;
		lcd_data(tens);
		flag=1;
	}
	if(digits ==1||flag ==1)
	{
		unit = value%10+48;
		lcd_data(unit);
	}
	if(digits>5)
	{
		
		lcd_data('e');
	}
}
void lcd_out(char x, char y, char *str)
{
	cursorxy(x,y);
	while(*str){
		lcd_data(*str);
		str++;
	}	
}

int adc_read(char channel)
{
	
	unsigned int result, a;
	ADMUX =channel;
	ADCSRA = 0x80;
	ADCH = 0x00;
	ADCL = 0x00;
	ADCSRA|=(1<<6);
	
	//Wait till conversion ends
	while((ADCSRA&0x40)!=0);
	
	a=ADCL;
	result=ADCH;
	result=result<<8;
	result =result |a;
	return result;
}
int main(void)
{
    /* Replace with your application code */
	unsigned int volt;
	DDRD|=(1<<0)|(1<<1);		//Output pins of RS and EN
	PORTD|=(1<<2)|(1<<3);		//enabling the internal pull up resistors
	GICR=(1<<6)|(1<<7);
	DDRC |= (1<<0);				// Make Led Pin PC0 act as output
	sei();
	char b=0;
	lcd_init();
	lcd_out(1,1,"ADC Test");
    while (1) 
    {
		volt =adc_read(0);
		lcd_print(1,1,volt,4);
		volt = volt*4.88;		//Convert to Millivolt
		lcd_print(2,1,volt,4);
		lcd_out(2,7,"mV");
		_delay_ms(1000);
		
		//If we need to close interrupt after 10 times then
		lcd_cmd(0x01);
		_delay_ms(1000);
		b++;
		if(b==10)
		{
			cli();		// To Disable the Interrupts...
		}
    }
}

ISR(INT0_vect)
{
	PORTC|=(1<<0);
}
ISR(INT1_vect)
{
	PORTC&=~(1<<0);
}