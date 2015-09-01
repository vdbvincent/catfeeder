/*
 * main.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
// Inclusions
#include <MsTimer2.h> // inclusion de la librairie Timer2
#include "boutons.h"
#include "lcd.h"
#include "clock.h"
#include "moteur.h"
#include "menu.h"
#include "alarme.h"

// Variables globales
static int c10 = 0 ;

void setup()
{
	// init du process bouton avec un delai de filtrage de 2*4=8ms
	boutons_setup(2);
	// Init de l'écran LCD
	lcd_setup();
	// Init de l'horloge
	clock_setup();
	// Init du moteur
	moteur_setup();
	// Init du memu
	menu_setup(300);  // 2sec de tempo pour l'écran de démarrage
	// Init de l'alarme
	alarme_setup();

	// initialisation interruption Timer 2
	MsTimer2::set(1, InterruptTimer2); // période 10ms 
	MsTimer2::start(); // active Timer2 
}

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

void loop()
{
	// Idle
	//clock_every1ms();  // trop rapide
	moteur_every1ms();
}