/*
 * catfeeder.ino
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
// Inclusions
#include "catfeeder.h"
#include "mcubind.h"
#include "boutons.h"
#include "logs.h"
#include "clock.h"
#include "alarme.h"
#include "moteur.h"
#include "menu.h"
#include "manager.h"

// Variables globales
static int c10 = 0 ;
static int tmp = 0;


void setup()
{
	// OS
	init_mcubind();
	print_log(INFO, F("Initialisation\n"));
	clock_setup();
	alarme_setup();
	boutons_setup(10);  // init du process bouton avec un delai de filtrage de 10ms
	moteur_setup();
	menu_setup(2); // temps du welcom screen de 2s
	manager_setup();
}

void loop()
{
	static char state = 99;
	
	logs_idle();
	menu_idle();
	manager_idle();
}


void every10ms(void)
{
	// TOUTES LES 10 MILLISECONDES
	switch (c10) 
	{
		case 0:
			clock_every100ms();
		break;
		case 1:
			boutons_every100ms();
		break;
		case 2:
			alarme_every100ms();
		break;
		case 3:
			moteur_every100ms();
		break;
	}
	if (c10 >= 10)
		c10 = 0;
	else
		c10 ++;
}
