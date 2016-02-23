/*
 * alarme.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "alarme.h"


// Variables globales
static Alarme_t * al_first = NULL;  // Toujours init la liste à NULL sinon peut etre considérée comme ayant au moins 1 element
//static llist * ma_liste = NULL;  // TODO : utiliser la linked list

static Minuteur_t * mi_pool[MAX_COUNT_MINUT];
static uint8_t m_nbMin = 0;

void alarme_setup(void)
{
	int i;
	al_first = NULL;
	
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
	// 1minute = 60 secondes = 60000ms = 600.100ms
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
	if (m_nbMin > 0)  // m_nbMin représente le nb de minuteurs
	{
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
					if (m_nbMin > 0)
						m_nbMin --;
					mi_pool[i] = NULL;
				}
			}
		}
	}
}

void alarme_every1mn(void)
{
	// vérifier toutes les minutes si une alarme déclenche
	uint8_t i;
	clock heure_courante;
	Alarme_t * al_tmp = NULL;
	al_tmp = al_first;

	heure_courante = clock_getClock();
	
	while (al_tmp != NULL)
	{
		if (al_tmp->horaire.heures == heure_courante.heures
		 && al_tmp->horaire.minutes == heure_courante.minutes)
		{
			print_log(DEBUG, "alarme : declenchement d'une alarme\n");
			al_tmp->foncteur();
		}
		al_tmp = al_tmp->suivant;
	}
}

// Méthode permettant de regler une alarme. Retourne 0 en cas d'echec
char alarme_setAlarme(clock p_al, void (*callback)(void))
{
	char ret = 0;
	Alarme_t * al_tmp = NULL;
	
	// Création de la nouvelle alarme
	Alarme_t * new_al = NULL;
	new_al = new Alarme_t();
	
	new_al->horaire.heures = p_al.heures;
	new_al->horaire.minutes = p_al.minutes;
	new_al->horaire.secondes = p_al.secondes;
	new_al->foncteur = callback;
	new_al->suivant = NULL;
	
	// Ajout de l'alarme
	al_tmp = al_first;
	if (al_tmp == NULL)
	{
		// Ajout de la premiere alarme
		al_first = new_al;
	}
	else
	{
		while(al_tmp->suivant != NULL)
		{
			al_tmp = al_tmp->suivant;
		}
		// ici al_tmp pointe sur le dernier element
		al_tmp->suivant = new_al;
	}

	ret = 1;
	print_log(DEBUG, "alarme : alarme enclenchee\n");

	return ret;
}

// Méthode permettant de régler une minuterie. Retourne 0 en cas d'échec
char alarme_setMinuteur(uint16_t p_delai, void (*callback)(void))
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
			am->delai = p_delai * 10;  // délai en sec x 10 car pas de 100ms
			am->foncteur = callback;
			mi_pool[i] = am;
			m_nbMin ++;
			ret = 1;
			print_log(DEBUG, "alarme : minuteur enclenche\n");
		}
	}
	return ret;
}

Alarme_t * alarme_getAlarme(void)
{
	return al_first;
}


// Fonction permettant de supprimer une alarme en fonction
// de sa position dans la liste chainée
Bool alarme_delAlarme(uint8_t p_selection)
{
	Bool b_return = True;
	uint8_t indice = 0;
	Alarme_t * al_tmp = al_first;
	Alarme_t * al_tmp_old = al_tmp;
	
	if (al_tmp == NULL)
	{
		// La liste est vide, rien a supprimer
		b_return = False;
	}

	if (b_return == True)
	{
		if (p_selection == 0)
		{
			// Dans le cas particulier de supprimer le premier element
			al_first = al_first->suivant;
			delete al_tmp;
			al_tmp = NULL;
		}
		else
		{
			// Sinon chercher le bon élément
			while (al_tmp->suivant != NULL && indice < p_selection)
			{
				al_tmp_old = al_tmp;
				al_tmp = al_tmp->suivant;
				indice ++;
			}
			if (indice != p_selection) // on a pas trouvé le bon indice
			{
				b_return = False;
			}
			else
			{
				// ici on a trouvé notre alarme pointée par al_tmp
				// Au pire on est sur le dernier élément non null car la cond de sortie est elem->suivant
				// Au mieux on est sur le premier non null;
				// al_tmp_old pointe sur l'élément -1 au mieux, au pire, si c'est le premier, il pointe sur le premier element comme al_tmp
				al_tmp_old = al_tmp->suivant;
				delete al_tmp;
				al_tmp = NULL;
			}
			/*while (al_tmp != NULL && indice < p_selection)
			{
				al_tmp_old = al_tmp;  // Se souvenir de l'élément précédent
				al_tmp = al_tmp->suivant;
				indice ++;
			}
			if (al_tmp == NULL)
			{
				b_return = False;
			}
			
			if (b_return == True)
			{
				// ici on a trouvé notre alarme pointée par al_tmp
				al_tmp_old->suivant = al_tmp->suivant;  // Shunter al_tmp
				delete al_tmp;                          // Supprimer al_tmp
				al_tmp = NULL;
			}*/
		}
	}
	return b_return;
}
