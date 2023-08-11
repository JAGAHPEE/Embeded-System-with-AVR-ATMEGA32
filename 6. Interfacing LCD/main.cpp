/*
 * LCD_Display.cpp
 *
 * Created: 02-08-2023 08:03:11
 * Author : kales
 */ 
#define F_CPU 16000000L
#include <avr/io.h>
#include <util/delay.h>

//define rs pin macro low high
#define rs_high() PORTD|=(1<<2);
#define rs_low() PORTD&=~(1<<2);

//define en pin macro low high
#define en_high() PORTD|=(1<<3);
#define en_low() PORTD&=~(1<<3);

#define lcdport PORTB

//Function to Write cmd on LCD
void lcd_cmd(unsigned char cmd)
{
	lcdport = cmd;
	rs_low();	//rs = 0 to select command register
	en_high();
	_delay_ms(1);
	en_low();
}

//Function to Write data on LCD
void lcd_data(unsigned char dat)
{
	lcdport = dat;
	rs_high();	//rs = 0 to select command register
	en_high();
	_delay_ms(1);
	en_low();
}

void lcd_init()
{
	lcd_cmd(0x38);
	_delay_ms(5);
	lcd_cmd(0x02);
	_delay_ms(5);
	lcd_cmd(0x01);
	_delay_ms(5);
	lcd_cmd(0x0c);
	_delay_ms(5);
	lcd_cmd(0x06);
	_delay_ms(5);
	lcd_cmd(0x80);
	_delay_ms(5);
	
	
}
int main(void)
{
	lcd_init();
	DDRB = 0xff;
	DDRD |=(1<<2);
	DDRD |=(1<<3);
	lcd_cmd(0xc0);
	lcd_cmd(0x18);
	lcd_data('W');
	lcd_data('e');
	lcd_data('l');
	lcd_data('c');
	lcd_data('o');
	lcd_data('m');
	lcd_data('e');
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

