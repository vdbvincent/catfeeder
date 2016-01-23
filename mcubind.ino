/*
 * mcubind.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "mcubind.h"


// DEFINITIONS

// Vitesse du port série
#define BAUD_RATE 115200


void init_mcubind(void)
{
	init_serial();
	init_timer1();
	adc_init();
	lcd_setup();
	//pwm_init();  // Fait direct dans pwm_out
}


/***************************************************************************/
/*                                                                         */
/*    __201008110080__  Liaison série                                      */
/*                                                                         */
/***************************************************************************/
void init_serial(void)
{
	// Calcul valeur du registre
	#if BAUD_RATE < 57600
		// Basse vitesse
		uint16_t UBRR0_value = ((F_CPU / (8L * BAUD_RATE)) - 1) / 2;
		UCSR0A &= ~(1 << U2X0); // doubleur vitesse off
	#else
		// haute vitesse
		uint16_t UBRR0_value = ((F_CPU / (4L * BAUD_RATE)) - 1) / 2;
		UCSR0A |= (1 << U2X0); // doubleur vitesse on
	#endif

	// Ecriture des registres en 2 fois
	UBRR0H = UBRR0_value >> 8;
	UBRR0L = UBRR0_value;

	// Activer l'émission
	UCSR0B |= 1 << TXEN0;

	// Le reste est en valeur par défaut
	// 8 bit, pas de parité, 1 bit de stop
}

// Envoi d'un caractère
void putchr(char c)
{
	// On attend que ce soit dispo/utilisable
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// Ecriture registre = envoi de l'octet
	UDR0 = c;
}

// Envoi d'une chaine de caractères
void printstr(const char * s)
{
	// On boucle tant qu'il y a des caractères
	while(*s)
	{
	    // on change LF et CR si necessaire
	    if (*s == '\n')
	    	putchr('\r');
	    putchr(*s++);
	}
}


/***************************************************************************/
/*                                                                         */
/*    __201008110080__  TIMER 1                                            */
/*                                                                         */
/***************************************************************************/

static void init_timer1(void)
{
#ifndef FAKEDEV
	cli();          // disable global interrupts
    TCCR1A = 0;     // set entire TCCR1A register to 0
    TCCR1B = 0;     // same for TCCR1B
 
    // set compare match register to desired timer count:
    //OCR1A = 7812;//   0.000064  // 10ms de periode
    OCR1A = 156;
    // turn on CTC mode:
    TCCR1B |= (1 << WGM12);
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR1B |= (1 << CS10);
    TCCR1B |= (1 << CS12);

    // enable timer compare interrupt:
    TIMSK1 |= (1 << OCIE1A);
    // enable global interrupts:
    sei();
#endif
}

ISR(TIMER1_COMPA_vect)
{
    every10ms(); // Appel du sequenceur
}



/***************************************************************************/
/*                                                                         */
/*    __201008110080__  mcubind_virtualport_init()                         */
/*                                                                         */
/***************************************************************************/

void mcubind_virtualport_init(uint8_t virtualPort, uint8_t flagUseAsOutput)
{
	if (flagUseAsOutput == 1)
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_D_b0  : DDRD |= (1 << DDD0) ; break ;
			case MCUBIND_VIRTUALPORT_D_b1  : DDRD |= (1 << DDD1) ; break ;
			case MCUBIND_VIRTUALPORT_D_b2  : DDRD |= (1 << DDD2) ; break ;
			case MCUBIND_VIRTUALPORT_D_b3  : DDRD |= (1 << DDD3) ; break; // _BV(PD3) ; break ;
			case MCUBIND_VIRTUALPORT_D_b4  : DDRD |= (1 << DDD4) ; break ;
			case MCUBIND_VIRTUALPORT_D_b5  : DDRD |= (1 << DDD5) ; break ;
			case MCUBIND_VIRTUALPORT_D_b6  : DDRD |= (1 << DDD6) ; break ;
			case MCUBIND_VIRTUALPORT_D_b7  : DDRD |= (1 << DDD7) ; break ;
			case MCUBIND_VIRTUALPORT_B_b8  : DDRB |= (1 << DDB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b9  : DDRB |= (1 << DDB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b10 : DDRB |= (1 << DDB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b11 : DDRB |= (1 << DDB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b12 : DDRB |= (1 << DDB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b13 : DDRB |= (1 << DDB5) ; break ;
		}
	}
	else
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_D_b0  : DDRD &= ~(1 << DDD0) ; break ;
			case MCUBIND_VIRTUALPORT_D_b1  : DDRD &= ~(1 << DDD1) ; break ;
			case MCUBIND_VIRTUALPORT_D_b2  : DDRD &= ~(1 << DDD2) ; break ;
			case MCUBIND_VIRTUALPORT_D_b3  : DDRD &= ~(1 << DDD3); break; //~_BV(PD3) ; break ;
			case MCUBIND_VIRTUALPORT_D_b4  : DDRD &= ~(1 << DDD4) ; break ;
			case MCUBIND_VIRTUALPORT_D_b5  : DDRD &= ~(1 << DDD5) ; break ;
			case MCUBIND_VIRTUALPORT_D_b6  : DDRD &= ~(1 << DDD6) ; break ;
			case MCUBIND_VIRTUALPORT_D_b7  : DDRD &= ~(1 << DDD7) ; break ;
			case MCUBIND_VIRTUALPORT_B_b8  : DDRB &= ~(1 << DDB0) ; break ;
			case MCUBIND_VIRTUALPORT_B_b9  : DDRB &= ~(1 << DDB1) ; break ;
			case MCUBIND_VIRTUALPORT_B_b10 : DDRB &= ~(1 << DDB2) ; break ;
			case MCUBIND_VIRTUALPORT_B_b11 : DDRB &= ~(1 << DDB3) ; break ;
			case MCUBIND_VIRTUALPORT_B_b12 : DDRB &= ~(1 << DDB4) ; break ;
			case MCUBIND_VIRTUALPORT_B_b13 : DDRB &= ~(1 << DDB5) ; break ;
		}
	}
}

/***************************************************************************/
/*                                                                         */
/*    __201008110090__  mcubind_virtualport_read()                         */
/*                                                                         */
/***************************************************************************/

uint16_t mcubind_virtualport_read(uint8_t virtualPort)
{
	// TODO : faire la lecture des ports numeriques
	switch (virtualPort)
	{
		case MCUBIND_VIRTUALPORT_ADC00 : return (adc_lecture(0));
		case MCUBIND_VIRTUALPORT_ADC01 : return (adc_lecture(1));
		case MCUBIND_VIRTUALPORT_ADC02 : return (adc_lecture(2));
		case MCUBIND_VIRTUALPORT_ADC03 : return (adc_lecture(3));
		case MCUBIND_VIRTUALPORT_ADC04 : return (adc_lecture(4));
		case MCUBIND_VIRTUALPORT_ADC05 : return (adc_lecture(5));
		case MCUBIND_VIRTUALPORT_ADC06 : return (adc_lecture(6));

		case MCUBIND_VIRTUALPORT_D_b3 : return (PIND & (1 << DDD3)); break;
	}

}



/***************************************************************************/
/*                                                                         */
/*    __201008110100__  mcubind_virtualport_write()                        */
/*                                                                         */
/***************************************************************************/

void mcubind_virtualport_write(uint8_t virtualPort, uint8_t value)
{
	// TODO : faire pareil pour le port B (8-13)
	if (value == 1)
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_D_b0 : PORTD |= (1 << DDD0) ; break ;
			case MCUBIND_VIRTUALPORT_D_b1 : PORTD |= (1 << DDD1) ; break ;
			case MCUBIND_VIRTUALPORT_D_b2 : PORTD |= (1 << DDD2) ; break ;
			case MCUBIND_VIRTUALPORT_D_b3 : PORTD |= (1 << DDD3) ; break; //_BV(PD3) ; break ;
			case MCUBIND_VIRTUALPORT_D_b4 : PORTD |= (1 << DDD4) ; break ;
			case MCUBIND_VIRTUALPORT_D_b5 : PORTD |= (1 << DDD5) ; break ;
			case MCUBIND_VIRTUALPORT_D_b6 : PORTD |= (1 << DDD6) ; break ;
			case MCUBIND_VIRTUALPORT_D_b7 : PORTD |= (1 << DDD7) ; break ;
		}
	}
	else
	{
		switch (virtualPort)
		{
			// config du port en I/O (0 = input only)
			case MCUBIND_VIRTUALPORT_D_b0 : PORTD &= ~(1 << DDD0) ; break ;
			case MCUBIND_VIRTUALPORT_D_b1 : PORTD &= ~(1 << DDD1) ; break ;
			case MCUBIND_VIRTUALPORT_D_b2 : PORTD &= ~(1 << DDD2) ; break ;
			case MCUBIND_VIRTUALPORT_D_b3 : PORTD &= ~(1 << DDD3) ; break; //~_BV(PD3) ; break ;
			case MCUBIND_VIRTUALPORT_D_b4 : PORTD &= ~(1 << DDD4) ; break ;
			case MCUBIND_VIRTUALPORT_D_b5 : PORTD &= ~(1 << DDD5) ; break ;
			case MCUBIND_VIRTUALPORT_D_b6 : PORTD &= ~(1 << DDD6) ; break ;
			case MCUBIND_VIRTUALPORT_D_b7 : PORTD &= ~(1 << DDD7) ; break ;
		}
	}
}

/***************************************************************************/
/*                                                                         */
/*    __201008110040__  adc_init()                                         */
/*                                                                         */
/***************************************************************************/

static void adc_init(void)
{
	// For Aref=AVcc;
	ADMUX=(1<<REFS0);
	//Prescalar div factor = 128
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

/***************************************************************************/
/*                                                                         */
/*    __201008110040__  adc_lecture()                                      */
/*                                                                         */
/***************************************************************************/

static uint16_t adc_lecture(uint8_t voie)
{
	uint8_t low = 0;
    uint8_t high = 0;
    uint16_t ret = 0;
	// choose channel voie must be 0-7
	//voie &= 0x03;
	ADMUX |= voie & 0x03;

	// Start single conversion
	ADCSRA |= (1 << ADSC);
	// The sbi() is a macro to set the bit(the second argument) of the 
	// address(the first argument) to 1.
	// Setting the ADSC bit of ADCSRA to 1 begins the AD conversion.
	//SBI(ADCSRA, ADSC);

	// Wait for convertion to complete
	//while (! (ADCSRA & ( 1 << ADIF) ) );

	// The bit_is_set is a macro that checks if the second argument bit of the
	// first argument is 1. If the conversion finishes, the ADSC of ADCSRA becomes 0.
	while (bit_is_set(ADCSRA, ADSC));
	
	// Clear ADIF by writting 1 to it
	//ADCSRA |= (1 << ADIF);

	//return ADC;

	low = ADCL;
	high = ADCH ;
	ret = (high << 8) | low ;
	return ret;
}


/***************************************************************************/
/*                                                                         */
/*    __201008110041__  pwm_init()                                         */
/*                                                                         */
/***************************************************************************/

static void pwm_init(void)
{
/*
	DDRD |= (1 << DDD3);

	// Set pwm for 50% duty cycle
	OCR0A = 128;

	// Set none-inverting mode
	TCCR0A |= (1 << COM0A1);

	// Set fast mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);

	// set prescaler to 8 and start pwm
	TCCR0B |= (1 << CS01);
*/

	// PD3 an output
	mcubind_virtualport_init(MCUBIND_VIRTUALPORT_D_b3, 1);
	/**
	 * There are quite a number of PWM modes available but for the
	 * sake of simplicity we'll just use the 8-bit Fast PWM mode.
	 * This is done by setting the WGM00 and WGM01 bits.  The 
	 * Setting COM0A1 tells the microcontroller to set the 
	 * output of the OCR0A pin low when the timer's counter reaches
	 * a compare value (which will be explained below).  CS00 being
	 * set simply turns the timer on without a prescaler (so at full
	 * speed).  The timer is used to determine when the PWM pin should be
	 * on and when it should be off.
	 */
	TCCR2A |= _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
	TCCR2B |= _BV(CS20);
}

/***************************************************************************/
/*                                                                         */
/*    __201008110041__  pwm_out()                                          */
/*                                                                         */
/***************************************************************************/

static void pwm_out(uint8_t pwm)
{
	// set prescaler to 8 and start pwm
	//TCCR0B |= (1 << CS01);

	static uint8_t flag = 0;

	if (pwm > 0)
	{
		if (flag == 0)
		{
			pwm_init();
			flag = 1;
		}
		OCR2B = pwm;
	}
	else
	{
		OCR2B = pwm;
		TCCR2A = 0x00;
		flag = 0;
	}

}