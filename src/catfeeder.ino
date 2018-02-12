/*
 * catfeeder.ino
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

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
static int c10 = 0;
static int tmp = 0;

/*
	TODO :
		- Ajouter un capteur de courant pour controler l'intensité du moteur
		- Ajouter un pont en H a base de relais pour faire tourner le moteur dans les 2 sens
		- Ajouter un module de communication RF
		- Ajouter un système de sauvegarde de codes defauts en eeprom
		- Ajouter un fusible en entrée de l'alimentation
*/

void setup()
{
	// OS
	init_mcubind();
	//print_log(INFO, "Initialisation\n");
	clock_setup();
	boutons_setup(100);  // init du process bouton avec un delai de filtrage de 10ms
	moteur_setup();
	manager_setup();
	menu_setup(2); // temps du welcom screen de 2s

	//-------------------
	// A faire seulement une fois au premier demarrage
	//-------------------
	/* clock tmp;
	 tmp.heures = 0;
	 tmp.minutes =  0;
	 eeprom_ecrire_alarme(tmp, EEPROM_AL1);
	 eeprom_ecrire_alarme(tmp, EEPROM_AL2);
	 eeprom_ecrire_alarme(tmp, EEPROM_AL3);
	 eeprom_ecrire_alarme(tmp, EEPROM_AL4);
	 eeprom_ecrire_alarme(tmp, EEPROM_AL5);
	 */
	
	alarme_setup();
}

void loop()
{
	logs_idle();
	menu_idle();
	logs_idle();
	clock_idle();
	delay(5);
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
