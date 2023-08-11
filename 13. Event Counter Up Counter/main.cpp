/*
 * Event Counter Up Counter.cpp
 *
 * Created: 04-08-2023 14:51:11
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

#define lcdport PORTC

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
void timer_delay(unsigned long k)
{
	unsigned int i,j;
	for(i=0;i<1000;i++)		//1000ms delay
	{
		for(j=0;j<10;j++)	//1ms delay
		{						
			//100 us delay
			TCNT0=155;
			TCCR0=0x02;	//0000 0010 means pre scaler of 8, means 8mhz system clock we get 1mhz timing clock.
			// 1mhz clock = timer will increment every 1usec from 155 to 255
			//100 pulse at 1mhz == 100 usec
		
			while((TIFR&0x01)==0);	// polling  overflow clock
			TCCR0=0x00;				// stopping the timer
			TIFR|=(1<<0);			//write 1 to 0th bit of TIFR so that we can clear the flag
		}
	}
}

int main(void)
{
    /* Replace with your application code */
	int count=0;
	
	DDRC = 0xff;	// LCD data pins as output
	
	DDRD |= (1<<2)|(1<<3);	//Enable RS pin as OUTPUT
	
	lcd_init();
	lcd_out(1,1,"Event Counter");
	
	DDRB &= ~(1<<0);		//Input pin PB0 to get input of signal generator
	PORTB|=(1<<0);
	
	TCNT0 = 0;		//Reset Start count to 0
	TCCR0 = 0x07;	//Start the counter 0000 0111
    while (1) 
    {
		count = TCNT0;
		lcd_print(2,1,count,5);
		timer_delay(1000);
    }
}


