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
//#include "menu.h"

// Variables globales
static int c10 = 0 ;
static int tmp = 0;


void setup()
{
	// OS
	init_mcubind();
	print_log(INFO, "Initialisation\n");
	clock_setup();
	alarme_setup();
	boutons_setup(10);  // init du process bouton avec un delai de filtrage de 10ms
	moteur_setup();
}

void loop()
{
	static char state = 99;
	
	logs_idle();

	// test du pwm
	// mise en route sur appui bouton
	if (isEmpty_btfifo() == False)
	{
		char toto = get_btfifo();
		if (toto == BT_H_PRESSE)
		{
			//pwm_out(32);
			moteur_setCmd(MT_PETIT);
		}
		else if (toto == BT_G_PRESSE)
		{
			moteur_setCmd(MT_MOYEN);
		}
		else if (toto == BT_B_PRESSE)
		{
			moteur_setCmd(MT_GRAND);
		}
		else if (toto == BT_D_PRESSE)
		{
			moteur_setCmd(MT_NONE);
		}

	}


/*
	switch (state)
	{
		case 0:
			// reglage de l'alarme
			clock mycl;
			mycl.heures = 0;
			mycl.minutes = 0;
			mycl.secondes = 0;

			clock_setClock(mycl);

			state = 1;
		break;

		case 1:
			if (tmp >= 5)
			{
				print_log(DEBUG, "Reglage de l'alarme a 00:01:00\n");
				clock mycl;
				mycl.heures = 0;
				mycl.minutes = 1;
				mycl.secondes = 0;
				if (alarme_setAlarme(mycl, &procAlarme) == 0)
				{
					print_log(DEBUG, "Impossible de regler l'alarme.\n");
				}
				state = 2;
			}
		break;

		case 2:
			if (tmp >= 7)
			{
				print_log(DEBUG, "Reglage du minuteur pour 15s\n");
				
				if (alarme_setMinuteur(15, &procMinu) == 0)
				{
					print_log(DEBUG, "Impossible de regler le minuteur.\n");
				}
				state = 3;
			}
		break;

		case 3:
			if (tmp % 10 == 0)
			{
				print_log(DEBUG, "\n");
			}
		break;
	}

	tmp ++;
*/

	//delay(1000);
}


void every10ms(void)  // maintenant ca doit etre a 10 ms
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

void procAlarme(void)
{
	print_log(DEBUG, "Alarme declenchee !!\n");
}

void procMinu(void)
{
	print_log(DEBUG, "Minuteur declenchee !!\n");
}