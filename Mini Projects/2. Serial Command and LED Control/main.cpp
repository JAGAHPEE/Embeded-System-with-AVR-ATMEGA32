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
#define rs_high() PORTD|=(1<<4);
#define rs_low() PORTD&=~(1<<4);

//define en pin macro low high
#define en_high() PORTD|=(1<<5);
#define en_low() PORTD&=~(1<<5);

#define lcdport PORTB
char a;
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
void uart_init(long Baudrate)
{
	//define led pin to toggle on as command received is a
	DDRB |=(1<<0);
	
	long ubrvalue;
	//ubrvalue = FCPU+Baudrate*8L
	ubrvalue =F_CPU;
	ubrvalue = ubrvalue/(Baudrate*16L);
	//ubrvalue = (FCPU + Baudrate*8L)/(Baudrate*16L);
	ubrvalue -=1;
	//set Baud rate
	UBRRH = (unsigned char )(ubrvalue>>8);
	UBRRL = (unsigned char)ubrvalue;
	/*set frame format
	 Asynchronous mode
	 no parity 
	 1 stop bit 
	 char size
	 */
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	/*Enable Interrupt
	RXCIE - Receive complete
	UDRIE - Data register empty
	Enable the Receiver and Transmitter
	*/
	UCSRB = (1<<RXEN)|(1<<TXEN);
	//UCSRB = (1<<RXCIE)|(1<<TXEN)|(1<<RXEN);
}

char uart_read()
{
	//wait until a data is available
	//while(!(UCSRA &(1<<RXC)));
	_delay_ms(100);
	//Now USART hat got data from host
	return UDR;
}

void uart_read_text(void *buff,uint16_t len)
{
	
	uint16_t i;
	for(i=0;i<len;i++)
	{
		((char *)buff)[i]=uart_read();
	}
}
void uart_write(char data)
{	// wait for Tx to become empty so its ready to receive raw data
	while(!(UCSRA & (1<<UDRE)));
	UDR=data;
}

void uart_write_text(char *str)
{
	while((*str)!='\0')
	{
		uart_write((*str));
		str++;
	}
}
int main(void)
{
    /* Replace with your application code */
	uart_init(9600);
	unsigned int volt;
	DDRD|=(1<<4)|(1<<5);		//Output pins of RS and EN
	PORTD|=(1<<2)|(1<<3);		//enabling the internal pull up resistors
	GICR=(1<<6)|(1<<7);
	DDRC = 0xff;				// Make Led Port PC act as output
	sei();
	lcd_init();
	lcd_out(1,1,"ADC Test");
	
    while (1) 
    {
		
		a =  uart_read();
		if(a > 60)
		{
			uart_write(a);
		}
		
		_delay_ms(100);
		volt =adc_read(0);
		lcd_print(1,1,volt,4);
		volt = volt*4.88;		//Convert to Millivolt
		lcd_print(2,1,volt,4);
		lcd_out(2,7,"mV");
//		_delay_ms(100);
		
		//If we need to close interrupt after 10 times then
//		lcd_cmd(0x01);
//		_delay_ms(1000);
		
		if (a =='A')
		{
			PORTC|=(1<<0);
		}
		else if (a=='a')
		{
			PORTC&=~(1<<0);
		}
		else if (a=='b')
		{
			PORTC&=~(1<<1);
		}
		else if (a=='B')
		{
			PORTC|=(1<<1);
		}
		else if (a=='C')
		{
			PORTC|=(1<<2);
		}
		else if (a=='c')
		{
			PORTC&=~(1<<2);
		}
		else if (a=='D')
		{
			PORTC|=(1<<3);
		}
		else if (a=='d')
		{
			PORTC&=~(1<<3);
		}
		else if (a=='E')
		{
			PORTC|=(1<<4);
		}
		else if (a=='e')
		{
			PORTC&=~(1<<4);
		}
		else if (a=='F')
		{
			PORTC|=(1<<5);
		}
		else if (a=='f')
		{
			PORTC&=~(1<<5);
		}
		else if (a=='G')
		{
			PORTC|=(1<<6);
		}
		else if (a=='g')
		{
			PORTC&=~(1<<6);
		}
		else if (a=='H')
		{
			PORTC|=(1<<7);
		}
		else if (a=='h')
		{
			PORTC&=~(1<<7);
		}
    }
}

ISR(INT0_vect)
{
	PORTC = 0xff;
}
ISR(INT1_vect)
{
	PORTC = 0x00;
}