/*
 * Serial_Communication_Char.cpp
 *
 * Created: 07-08-2023 08:01:52
 * Author : kales
 */ 

#include <avr/io.h>
#define F_CPU 16000000
#include <util/delay.h>

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
	while(!(UCSRA &(1<<RXC)));
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
	/*	while (1) 
		{
			uart_write_text("Hello World \r\n");
			_delay_ms(2000);
		}
	*/
	char a;
	while(1)
	{
		a= uart_read();
		_delay_ms(1000);
		uart_write(a+1);
		//if A is received then LED ON
		//else OFF
		if(a=='A')
		{
			PORTB|=(1<<0);
		}
		else
		{
			PORTB&=~(1<<0);
		}
		
	}
}

