/*
 * alarme.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "alarme.h"

#define delais_sec 5
#define delais (10*delais_sec)

// Variables globales
static Alarme_t al_pool[MAX_COUNT_ALARM];
//static Minuteur_t * mi_pool[MAX_COUNT_MINUT];
static Minuteur_t mi_pool[MAX_COUNT_MINUT];
static uint8_t m_nbMin = 0;

void alarme_setup(void)
{
	int i;
	clock ctmp;
	uint8_t indice = 0;
	
	// Init de la banque de minuteries à NULL
	for (i = 0; i < MAX_COUNT_MINUT; i++)
	{
		mi_pool[i].delai = 0;
	}

	for (i = 0; i < MAX_COUNT_ALARM; i++)
	{
		al_pool[i].horaire.heures = 0;
		al_pool[i].horaire.secondes = 0;
		al_pool[i].horaire.minutes = 0;
	}

	// Charger les alarmes stockées en eeprom
	indice = 0;
	ctmp = eeprom_lire_alarme(indice);
	indice ++;
	while(indice <= 5)
	{
		#ifdef ADEBUG
			char tabtmp2[32];
			sprintf(tabtmp2, "al : init %d:%d\n", ctmp.heures, ctmp.minutes);
			//print_log(DEBUG, tabtmp2);
			printstr(tabtmp2);
			#endif

		if (ctmp.heures != 0 || ctmp.minutes != 0)
		{
			

			alarme_setAlarme(ctmp, &manager_procAlarme, 0);  // Toujours appeler la methode du manager
		}
		ctmp = eeprom_lire_alarme(indice);
		indice ++;
	}
}

void alarme_every100ms(void)
{
	static uint8_t cmp_100ms = 0;
	uint8_t i = 0;
	
	// Gestion de l'alarme
	// 1minute = 60 secondes = 60000ms = 600.100ms
	// 5secondes = 5000ms = 50 * 100ms
	
	if (cmp_100ms == 0)
	{
		alarme_every1mn();
	}
	cmp_100ms ++;  // incrementer le compteur
	if (cmp_100ms >= delais)
	{
		cmp_100ms = 0;
	}
	
	// Gestion de la minuterie
	if (m_nbMin > 0)  // m_nbMin représente le nb de minuteurs
	{
		for (i = 0; i < MAX_COUNT_MINUT; i++)
		{
			// Tester le pointeur
			//if (mi_pool[i] != NULL)
			if (mi_pool[i].delai > 0)
			{
				// Tester le temps restant
				if (mi_pool[i].delai > 1)
				{
					// Décrementer le délai
					mi_pool[i].delai --;
				}
				else
				{
					#ifdef ADEBUG
					print_log(DEBUG, "al : set timer\n");
					#endif
					// La minuterie est arrivée à expiration, déclencher le callback
					(*mi_pool[i].foncteur)();
					//delete mi_pool[i];
					//mi_pool[i] = NULL;
					mi_pool[i].delai = 0;
					mi_pool[i].foncteur = NULL;

					if (m_nbMin > 0)
						m_nbMin --;
				}
			}
		}
	}
}

void alarme_every1mn(void)
{
	// vérifier toutes les minutes si une alarme déclenche
	uint8_t i = 0;
	uint8_t tmp = 0;
	clock heure_courante;

	heure_courante = clock_getClock();

	while (i < MAX_COUNT_ALARM)
	{
		if (al_pool[i].horaire.heures != 0 || al_pool[i].horaire.minutes != 0)
		{
			if (al_pool[i].horaire.heures == heure_courante.heures
			 && al_pool[i].horaire.minutes == heure_courante.minutes)
			{
				if (heure_courante.secondes > (al_pool[i].horaire.secondes - 5)
				 && heure_courante.secondes < (al_pool[i].horaire.secondes + 5))
				{
					#ifdef ADEBUG
					print_log(DEBUG, "al : declenchement alarme\n");
					#endif
					(*al_pool[i].foncteur)();
					tmp ++;
					al_pool[i].foncteur = NULL;
				}
			}
		}
		i ++;
	}
	#ifdef ADEBUG
	if (tmp != 0)
	{
		print_log(DEBUG, "al : + alarmes\n");
	}
		
	#endif
}

// Méthode permettant de regler une alarme. Retourne 0 en cas d'echec
char alarme_setAlarme(clock p_al, void (*callback)(void), uint8_t p_toMemory)
{
	char ret = 0;
	uint8_t indice = 0;

	// Recherche de la premiere position vide
	while ((al_pool[indice].horaire.heures != 0 || al_pool[indice].horaire.minutes != 0) && indice < MAX_COUNT_ALARM)
	{
		indice ++;
	}

	if (indice < MAX_COUNT_ALARM)
	{
		// ici indice pointe sur le premier element vide du tableau al_pool
		// Ajout de l'alarme
		//al_pool[indice].horaire = p_al;
		al_pool[indice].horaire.secondes = p_al.secondes;
		al_pool[indice].horaire.minutes = p_al.minutes;
		al_pool[indice].horaire.heures = p_al.heures;
		al_pool[indice].foncteur = callback;
		
		if (p_toMemory > 0)
		{
			// Enregistrement en eeprom
			eeprom_ecrire_alarme(al_pool[indice].horaire, indice);
		}

		#ifdef ADEBUG
		char tabtmp[32];
		sprintf(tabtmp, "al : set %d:%d\n", al_pool[indice].horaire.heures, al_pool[indice].horaire.minutes);
		print_log(DEBUG, tabtmp);
		#endif

		ret = 1;
	}

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
		//if (mi_pool[i] == NULL)
		if (mi_pool[i].delai == 0)
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
		mi_pool[i].delai = p_delai * 10;  // Délais en sec x 10 car ps de 100ms
		mi_pool[i].foncteur = callback;
		m_nbMin ++;
		ret = 1;
		#ifdef ADEBUG
		print_log(DEBUG, "al : minuteur set\n");
		#endif
	}
	return ret;
}

//Alarme_t * alarme_getAlarme(void)
clock alarme_getAlarme(uint8_t indice)
{
	if (indice < 0 || indice >= MAX_COUNT_ALARM)
	{
		clock tmp;
		tmp.heures = 0;
		tmp.minutes = 0;
		tmp.secondes = 0;
	}
	else
	{
		#ifdef ADEBUG
		char tabtmp2[32];
		sprintf(tabtmp2, "al : get %d:%d\n", al_pool[indice].horaire.heures, al_pool[indice].horaire.minutes);
		print_log(DEBUG, tabtmp2);
		#endif
		return al_pool[indice].horaire;
	}
}


// Fonction permettant de supprimer une alarme en fonction
// de sa position dans la liste chainée
Bool alarme_delAlarme(uint8_t p_selection)
{
	Bool b_return = True;

	if (p_selection < MAX_COUNT_ALARM)
	{
		if (al_pool[p_selection].horaire.heures != 0 || al_pool[p_selection].horaire.minutes != 0)
		{
			// Suppression de l'alarme
			al_pool[p_selection].horaire.heures = 0;
			al_pool[p_selection].horaire.minutes = 0;
			al_pool[p_selection].horaire.secondes = 0;
			// Propager la suppression dans l'eeprom
			eeprom_ecrire_alarme(al_pool[p_selection].horaire, p_selection);

			#ifdef ADEBUG
			char tabtmp[32];
			sprintf(tabtmp, "al : mod %d:%d\n", al_pool[p_selection].horaire.heures, al_pool[p_selection].horaire.minutes);
			print_log(DEBUG, tabtmp);
			#endif
		}
		else
		{
			b_return = False;
		}
	}
	else
	{
		b_return = False;
	}

	return b_return;
}

void alarme_modify(clock p_clock, uint8_t p_indice)
{
	uint8_t i = 0;

	if (p_indice < MAX_COUNT_ALARM)
	{
		if (p_clock.heures != 0 || p_clock.minutes != 0)
		{
		    al_pool[p_indice].horaire.heures = p_clock.heures;
			al_pool[p_indice].horaire.minutes = p_clock.minutes;
			al_pool[p_indice].horaire.secondes = p_clock.secondes;
			// Propager la modification dans l'eeprom
			eeprom_ecrire_alarme(al_pool[p_indice].horaire, p_indice);
		}
	}
}
