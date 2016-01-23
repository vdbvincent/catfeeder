/*
 * alarme.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 *	// TODO : faire des listes chainees et pas des pool en tableau
 *           tester si le tableau est vide en alarme ou minuteur, inutile
 *           de chercher a tester chaque alarme, rendre la main
 */
#include "alarme.h"

// Nombre max d'alarme configurable
#define MAX_COUNT_ALARM 5
// Nombre de minuterie configurable - Estimer la meilleur taille dans le rapport utilité/taille utilisée
#define MAX_COUNT_MINUT 5

// Variables globales
static Alarme_t * al_pool[MAX_COUNT_ALARM];
static Minuteur_t * mi_pool[MAX_COUNT_MINUT];
static Bool mb_AlarmeOff = True;
static Bool mb_MinutOff = True;

void alarme_setup(void)
{
	int i;
	// Init de la banque d'alarmes
	for (i = 0; i < MAX_COUNT_ALARM; i++)
	{
		al_pool[i] = NULL;
	}
	
	// Init de la banque de minuteries à NULL
	for (i = 0; i < MAX_COUNT_MINUT; i++)
	{
		mi_pool[i] = NULL;
	}
	print_log(INFO, "alarme : fin init.\n");
}

void alarme_every100ms(void)
{
	static int cmp_100ms = 0;
	uint8_t i = 0;
	
	// Gestion de l'alarme
	// 1minute = 60 secondes = 60000ms = 600 100ms
	if (cmp_100ms >= 600)
	{
		alarme_every1mn();
		cmp_100ms = 0;
	}
	else
	{
		cmp_100ms ++;
	}
	
	// Gestion de la minuterie
	for (i = 0; i < MAX_COUNT_MINUT; i++)
	{
		// Tester le pointeur
		if (mi_pool[i] != NULL)
		{
			// Tester le temps restant
			if (mi_pool[i]->delai > 0)
			{
				// Décrementer le délai
				mi_pool[i]->delai --;
			}
			else
			{
				print_log(DEBUG, "alarme : declenchement d'un minuteur\n");
				// La minuterie est arrivée à expiration, déclencher le callback
				(*mi_pool[i]->foncteur)();
				delete mi_pool[i];
				mi_pool[i] = NULL;
			}
		}
	}
}

void alarme_every1mn(void)
{
	// vérifier toutes les minutes si une alarme déclenche
	// Si plusieurs alarmes déclenchent en meme temps,
	// une seule distribution sera effectuée

	uint8_t i;
	clock heure_courante;

	heure_courante = clock_getClock();

	for (i = 0; i < MAX_COUNT_ALARM; i++)
	{
		// Tester le pointeur
		if (al_pool[i] != NULL)
		{
			// Tester le temps restant
			if (al_pool[i]->horaire.heures == heure_courante.heures
				 && al_pool[i]->horaire.minutes == heure_courante.minutes)
				 //&& al_pool[i]->horaire.secondes == heure_courante.secondes)
			{
				print_log(DEBUG, "alarme : declenchement d'une alarme\n");
				al_pool[i]->foncteur();
			}
		}
	}

}

// Méthode permettant de regler une alarme. Retourne 0 en cas d'echec
char alarme_setAlarme(clock p_al, uint8_t pos, void (*callback)(void))
{
	char ret = 0;

	if (al_pool[pos] == NULL)
	{
		// Un emplacement vide a été trouvé, creation de l'alarme
		Alarme_t * al = new Alarme_t();
		if (al != NULL)
		{
			al->horaire.heures = p_al.heures;
			al->horaire.minutes = p_al.minutes;
			al->horaire.secondes = p_al.secondes;
			al->foncteur = callback;
			al_pool[pos] = al;

			ret = 1;
			print_log(DEBUG, "alarme : alarme enclenchee\n");
		}
	}
	else
	{
		al_pool[pos]->horaire.heures = p_al.heures;
		al_pool[pos]->horaire.minutes = p_al.minutes;
		al_pool[pos]->horaire.secondes = p_al.secondes;
		al_pool[pos]->foncteur = callback;
		ret = 1;
		print_log(DEBUG, "alarme : alarme enclenchee\n");
	}

	return ret;
}

// Méthode permettant de régler une minuterie. Retourne 0 en cas d'échec
char alarme_setMinuteur(uint32_t p_delai, void (*callback)(void))
{
	char ret = 0;
	int i = 0;
	Bool b_exit = False;
	// Recherche du premier emplacement libre
	while ((i < MAX_COUNT_MINUT) && (b_exit == False))
	{
		if (mi_pool[i] == NULL)
		{
			b_exit = True;
		}
		else
		{
			i ++;
		}
	}
	if (b_exit == True)
	{
		// Un emplacement vide a été trouvé à i
		//Minuteur_t * am = malloc(sizeof(Minuteur_t));
		Minuteur_t * am = new Minuteur_t();
		if (am != NULL)
		{
			am->delai = p_delai * 10;  // délai x 10 car pas de 100ms
			am->foncteur = callback;
			mi_pool[i] = am;
			ret = 1;
			print_log(DEBUG, "alarme : minuteur enclenche\n");
		}
	}
	return ret;
}


char * alarme_getAlarme(uint8_t p_selection)
{
	return "00:00";
}