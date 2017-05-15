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
#include "e2prom.h"

// Variables globales
static int c10 = 0;
static int tmp = 0;


void setup()
{
	//clock tmp;
	//char txt[8];
	
	// OS
	init_mcubind();
	print_log(INFO, "Initialisation\n");
	clock_setup();
	alarme_setup();
	boutons_setup(10);  // init du process bouton avec un delai de filtrage de 10ms
	moteur_setup();
	menu_setup(2); // temps du welcom screen de 2s
	manager_setup();


	//-------------------
	// A faire seulement une fois au premier demarrage
	//-------------------
	// tmp.heures = 0;
	// tmp.minutes =  0;
	// eeprom_ecrire_alarme(tmp, EEPROM_AL1);
	// eeprom_ecrire_alarme(tmp, EEPROM_AL2);
	// eeprom_ecrire_alarme(tmp, EEPROM_AL3);
	// eeprom_ecrire_alarme(tmp, EEPROM_AL4);
	// eeprom_ecrire_alarme(tmp, EEPROM_AL5);
	
	// tmp = eeprom_lire_alarme(EEPROM_AL5);
	// sprintf(txt, "%02d:%02d", tmp.heures, tmp.minutes);
	// print_log(DEBUG, txt);
	//-------------------
}

void loop()
{
	logs_idle();
	menu_idle();

	delay(10);
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
		case 4:
			manager_every100ms();
		break;
	}
	if (c10 >= 10)
		c10 = 0;
	else
		c10 ++;
}