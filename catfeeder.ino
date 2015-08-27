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

// Déclaration de fonctions
//void InterruptTimer2();

// Variables globales
static int c10 = 0 ;

void setup()
{
	// init du process bouton avec un delai de filtrage de 4*4=20ms
	boutons_setup(2);
	// Init de l'écran LCD
	lcd_setup();
	// Init de l'horloge
	clock_setup();
	// Init du moteur
	moteur_setup();
	// Init du memu
	menu_setup();
	// Init de l'alarme
	alarme_setup();

	// initialisation interruption Timer 2
	MsTimer2::set(5, InterruptTimer2); // période 5ms 
	MsTimer2::start(); // active Timer2 
}

// debut de la fonction d'interruption Timer2
void InterruptTimer2()
{
	// TOUTES LES 10 MILLISECONDES
	boutons_every10ms(); 
	lcd_every10ms();
	//clock_every10ms();
	menu_every10ms();
	alarme_every10ms();
}

void loop()
{
	// Idle
	clock_every1ms();
	moteur_every1ms();

}


