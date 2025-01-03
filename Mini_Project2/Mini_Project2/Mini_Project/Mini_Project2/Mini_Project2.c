/*
 * Mini_Project2.c
 *
 *  Created on: Sep 18, 2023
 *      Author: M
 */

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

unsigned char sec1 = 0;
unsigned char sec2 = 0;
unsigned char min1 = 0;
unsigned char min2 = 0;
unsigned char hours1 = 0;
unsigned char hours2 = 0;

unsigned char flag = 0;

ISR(TIMER1_COMPA_vect)
{
	flag = 1;
	SREG |= (1<<7);
}
ISR(INT0_vect)
{
	SREG |= (1<<7);
	 sec1 = 0;
	 sec2 = 0;
	 min1 = 0;
	 min2 = 0;
	 hours1 = 0;
	 hours2 = 0;
}

ISR(INT1_vect)
{
	SREG |= (1<<7);
	TCCR1B &= ~(1<<CS10) & ~(1<<CS11) & ~(1<<CS12);
}

ISR(INT2_vect)
{
	SREG |= (1<<7);
	TCCR1B |= (1<<CS10) | (1<<CS12);
}
void Timer1_init_CTC_mode(void)
{
	TCNT1 = 0;

	OCR1A = 1000;

	TIMSK |= (1<<OCIE1A);

	TCCR1A = (1<<FOC1A);
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12);
}

void INT0_init(void)
{
	DDRD &= ~(1<<PD2);
	PORTD |= (1<<PD2);

	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);

	GICR |= (1<<INT0);
}

void INT1_init(void)
{
	DDRD &= ~(1<<PD3);

	MCUCR |= (1<<ISC10) | (1<<ISC11);

	GICR |= (1<<INT1);

}

void INT2_init(void)
{
	DDRB &= ~(1<<PB2);
	PORTB |= (1<<PB2);

	MCUCSR &= ~(1<<ISC2);

	GICR |= (1<<INT2);

}

void seven_segment(void)
{

	PORTA = (PORTA & 0XC0) | (1<<0);
	PORTC = (PORTC & 0XF0) | (0X0F & sec1);

	_delay_ms(2);
	PORTA = (PORTA & 0XC0) | (1<<1);
	PORTC = (PORTC & 0XF0) | (0X0F & sec2);

	_delay_ms(2);
	PORTA = (PORTA & 0XC0) | (1<<2);
	PORTC = (PORTC & 0XF0) | (0X0F & min1);

	_delay_ms(2);
	PORTA = (PORTA & 0XC0) | (1<<3);
	PORTC = (PORTC & 0XF0) | (0X0F & min2);

	_delay_ms(2);
	PORTA = (PORTA & 0XC0) | (1<<4);
	PORTC = (PORTC & 0XF0) | (0X0F & hours1);

	_delay_ms(2);
	PORTA = (PORTA & 0XC0) | (1<<5);
	PORTC = (PORTC & 0XF0) | (0X0F & hours2);

	_delay_ms(2);
}

void display(void)
{
	sec1++;
	if(sec1 == 10)
	{
		sec1 = 0;
		sec2++;
	}

	if(sec2 == 6 && sec1 == 0)
	{
		sec2 = 0;
		min1++;
	}
	if(min1==10)
	{
		min1 = 0;
		min2++;
	}
	if(min2 == 6 && min1 == 0)
	{
		min2 = 0;
		hours1++;
	}
	if(hours1 == 10)
	{
		hours1 = 0;
		hours2=1;
	}
}

int main()
{
	DDRC |= 0X0F;
	PORTC &= 0XF0;

	DDRA |= 0X3F;
	PORTA = 0XC0;

	SREG |= (1<<7);

	Timer1_init_CTC_mode();
	INT0_init();
	INT1_init();
	INT2_init();

	while(1)
	{
		seven_segment();
		if(flag == 1)
		{
			display();
		}
		flag = 0;
	}
}




