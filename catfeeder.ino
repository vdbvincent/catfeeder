/*
 * main.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
// Inclusions
#include "mcubind.h"
//#include <MsTimer2.h> // inclusion de la librairie Timer2
#include "boutons.h"
//#include "lcd.h"
//#include "clock.h"
//#include "moteur.h"
//#include "menu.h"
//#include "alarme.h"
/*

// Variables globales
static int c10 = 0 ;


// debut de la fonction d'interruption Timer2
void InterruptTimer2()
{
	// TOUTES LES 10 MILLISECONDES
	switch (c10) 
	{
		case 0:
			boutons_every10ms();
		break;
		case 1:
			lcd_every10ms();
		break;
		case 2:
			clock_every10ms();
		break;
		case 3:
			menu_every10ms();
		break;
		case 4:
			alarme_every10ms();
		break;
		case 5:
		// do something
		break;
		case 6:
		// do something
		break;
		case 7:
		// do something
		break;
		case 8:
		// do something
		break;
		case 9:
		// do something
		break;
	}
	if (c10 >= 10)
		c10 = 0;
	else
		c10 ++;

}
*/

/*****
TODO :
	Macro d'activation de logs dans chaque source
	Baisser la période  du timer à 10 ms
	partage la lecture d'une ADC

******/

void setup()
{
	init_mcubind();
	printstr("Initialisation\n");
	// init du process bouton avec un delai de filtrage de 2*4=8ms
	boutons_setup(1);
}

void loop()
{
	/*int value = 2;
	char buffer[20];
	value = mcubind_virtualport_read(MCUBIND_VIRTUALPORT_ADC00);
	sprintf(buffer, "valeur = %d v\n", value);
	printstr(buffer);*/
	delay(1000);
}



void every500ms(void)
{
	//printstr("500 ms\n");
	boutons_every10ms();
}