/*
 * alarme.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "alarme.h"

// Nombre max d'alarme configurable
#define MAX_COUNT_ALARM 5

static Alarme_t * al_pool[MAX_COUNT_ALARM];

void alarme_setup(void)
{
	int i;
	// Init de la banque d'alarmes
	for (i = 0; i < MAX_COUNT_ALARM; i++)
	{
		//Alarme_t * al = malloc(sizeof(Alarme_t));
		Alarme_t * al = new Alarme_t();
		al->horaire.heures = 0;
		al->horaire.minutes = 0;
		al->horaire.secondes = 0;
		al->isValid = False;
		al_pool[i] = al;
	}
}

void alarme_every10ms(void)
{
	static int cmp_10ms = 0;

	if (cmp_10ms == 100)
	{
		alarme_every1s();
		cmp_10ms = 0;
	}
	else
	{
		cmp_10ms ++;
	}
	
}

void alarme_every1s(void)
{
	// vérifier toutes les secondes si une alarme déclenche
	// Si plusieurs alarmes déclenchent en meme temps,
	// une seule distribution sera effectuée

	int i;
	clock heure_courante;

	heure_courante = clock_getClock();

	for (i = 0; i < MAX_COUNT_ALARM; i++)
	{
		if (al_pool[i]->horaire.heures == heure_courante.heures
		 && al_pool[i]->horaire.minutes == heure_courante.minutes
		 && al_pool[i]->horaire.secondes == heure_courante.secondes)
		{
			menu_doFeedTheCatEvent();
		}
	}
}

// Méthode permettant de regler une alarme. Retourne 0 en cas d'echec
char alarme_setAlarme(clock p_al, char indice)
{
	if (indice > MAX_COUNT_ALARM - 1)
	{
		return 0;
	}
		
	al_pool[indice]->horaire.heures = p_al.heures;
	al_pool[indice]->horaire.minutes = p_al.minutes;
	al_pool[indice]->horaire.secondes = p_al.secondes;
	al_pool[indice]->isValid = True;
	return 1;
}