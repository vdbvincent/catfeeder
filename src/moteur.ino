/*
 * moteur.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#include "moteur.h"

static uint8_t commande = 0;
static uint8_t isRunning = 0;

void moteur_setup(void)
{
	mcubind_virtualport_init(MCUBIND_VIRTUALPORT_D_b2, 1);
	mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b2, 0);
}


void moteur_every100ms(void)
{
	static uint8_t state = 0;
	static uint8_t cmp = 0;

	switch (state)
	{
	    case 0:
	      	// Attente d'une commande
			if (commande != MT_NONE)
			{
				#ifdef MDEBUG
				print_log(DEBUG, "MT : commande captee\n");
				#endif
				isRunning = 1;
				cmp = 0; // pilotage relay
				state = 2; // pilotage relay
				mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b2, 1);  // pilotage relay
			}
	      break;

	    case 2:
	    	// régime établi
	    	if (cmp >= commande * 10)   // commande * 10 pour compenser le cadensement de 100ms
	    	{
	    		#ifdef MDEBUG
	    		print_log(DEBUG, "MT : arrete commande\n");
	    		#endif
	    		// Arret de la commande
	    		mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b2, 0);  // pilotage relay
	    		cmp = 0;
	    		isRunning = 0;
	    		commande = MT_NONE;
	    		state = 0;
	    	}
	    	else
	    	{
	    		cmp ++;
	    	}
	    break;
	}
}


// TODO : dans une version future, retourner un code erreur
Bool moteur_setCmd(uint8_t value)
{
	if (isRunning == 0)
	{
		commande = value;
		return True;
	}
	else
	{
		return False;
	}
}