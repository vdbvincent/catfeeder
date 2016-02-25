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
				#ifdef MDEBUG1
				print_log(DEBUG, F("MT : commande captee\n"));
				#endif
				isRunning = 1;
				cmp = 4;	// valeur de 4 pour démarrer le calcul de la rampe
				state = 1;
			}
	      break;

	    case 1:
	      	// regime transitoire
	      	if (cmp >= DUTYCYCLE)
	      	{
	      		cmp = 0;
	      		state = 2;
	      		pwm_out(DUTYCYCLE);
	      		#ifdef MDEBUG
	      		print_log(DEBUG, F("MT : rampe finie\n"));
	      		#endif
	      	}
	      	else
	      	{
	      		// Tant que la consigne n'est pas atteinte, multiplier le rapport cycle par 2
	      		pwm_out(cmp);
	      		cmp = cmp * 2;
	      	}
	      break;

	    case 2:
	    	// régime établi
	    	if (cmp >= commande * 10)   // commande * 10 pour compenser le cadensement de 100ms
	    	{
	    		#ifdef MDEBUG1
	    		print_log(DEBUG, F("MT : arrete commande\n"));
	    		#endif
	    		// Arret de la commande
	    		pwm_out(0);
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
