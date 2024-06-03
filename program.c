/*

 * program.c
 *
 *  Created on: Apr 10, 2024
 *      Author: Yassein Hamed
 */





#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>


unsigned char seconds=0 ;
unsigned char minutes=0 ;
unsigned char hours=0;




void SevenSegment_init(void);
void SevenSegment_Display(void);
void Timer1(void);
void Reset (void);
void Pause(void);
void Resume (void);



int main ()
{
	SevenSegment_init();
	sei(); // SET I-BIT
	Reset (); //Func 2 INIT RESET Button
	Pause();  //Func 2 INIT PAUSE BUTTOM
	Resume ();  //Func 2 INIT RESUME BUTTOM
	Timer1();  //Func 2 INIT TIMER1

	while (1)
	{
		SevenSegment_Display();  //FUNC 2 DISPLAY 7 SEGMENT
	}
}

void SevenSegment_init(void)
{
	DDRC |= 0x0F;   //INITIALIZE FIRST 4-PINS IN PORTC
	DDRA |= 0x3F;   //INITIALIZE FIRST 6-PINS IN PORTA
	PORTC =0;       //TURN OFF PORTC
}



void Timer1(void)
{
	TCNT1 = 0;


	OCR1A = 976;

	TCCR1A |= (1<<FOC1A);
	TCCR1B |= (1<<CS10) | (1<<CS12) | (1<<WGM12);
	TIMSK |= (1<<OCIE1A);
}


void Reset (void)
{
	DDRD &= ~(1<<PD2);  // INPUT PIN
	PORTD |= (1<<PD2);  // ENABLE INTERNAL PULL UP
	GICR |= (1<<INT0);  //ENABLE INTERRUPT FOR INT0
	MCUCR |= (1<<ISC01); // FALLING FLAG
}




/*                                         PAUSE FUNCTION                                          */
void Pause(void)
{
	GICR |= (1<<INT1);  //ENABLE INTERRUPT FOR INT1
	MCUCR |= (1<<ISC10) | (1<<ISC11);  // RISING FLAG
}



/*                                         RESUME FUNCTION                                          */
void Resume (void)
{
	DDRB &= ~(1<<PB2);  // INPUT PIN
	PORTB |= (1<<PB2);  // ENABLE INTERNAL PULL UP
	GICR |= (1<<INT2);  //ENABLE INTERRUPT FOR INT2
	MCUCSR &= ~(1<<ISC2);  // FALLING FLAG
}






void SevenSegment_Display(void)
{
	/* ----- Display 1st digit Minutes ----- */
	PORTA = (1<<PA0);
	PORTC = seconds%10;

	_delay_ms(1);

	/* ----- Display 2nd digit Seconds ----- */
	PORTA = (1<<PA1);
	PORTC = seconds/10;

	_delay_ms(1);

	/* ----- Display 1st digit Minutes ----- */
	PORTA = (1<<PA2);
	PORTC = minutes%10;

	_delay_ms(1);

	/* ----- Display 2nd digit Minutes ----- */
	PORTA = (1<<PA3);
	PORTC = minutes/10;

	_delay_ms(1);

	/* ----- Display 1st digit Hours ----- */
	PORTA = (1<<PA4);
	PORTC = hours%10;

	_delay_ms(1);

	/* ----- Display 2nd digit Hours ----- */
	PORTA = (1<<PA5);
	PORTC = hours/10;

	_delay_ms(1);
}


ISR (TIMER1_COMPA_vect)  // watch increament
{
	seconds++;

	if (seconds == 60)
	{
		minutes++;
		seconds=0;
	}

	if (minutes == 60)
	{
		hours++;
		minutes=0;
	}
}




ISR (INT0_vect)  // RESET BUTTOM
{
	hours = 0;
	minutes = 0;
	seconds = 0;
}





ISR (INT1_vect) // PAUSE BUTTOM
{
	TCCR1B &= ~(1<<CS10);
	TCCR1B &= ~(1<<CS11);
	TCCR1B &= ~(1<<CS12);
}



ISR (INT2_vect)  // RESUME BUTTOM
{
	TCCR1B |= (1<<CS10);
	TCCR1B |= (1<<CS12);
}





