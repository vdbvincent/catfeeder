/*
 * menu.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "menu.h"

// Variables globales
static unsigned int ui_tempoDem = 2;  // Valeur par défaut 300 = 2s
uint8_t g_tmp = 0;
clock * horloge;

void menu_setup(unsigned int p_tempoDem)
{
	// Configuration de la tempo de l'écran de démarrage
 	ui_tempoDem = p_tempoDem;
 	//horloge = new clock();
	horloge = (clock*)malloc(sizeof(clock));
}

void menu_idle(void)
{
	// Gestion de l'affichage des menus
	menu_affMenu();
}
	
static void menu_affMenu(void)
{
	static uint8_t state = 0;
	static Select_t select;
	char ret;  // Variable temporaire
	static clock clocktmp;
	clocktmp.heures = 0;
	clocktmp.minutes = 0;
	clocktmp.secondes = 62;

	switch (state)
	{
	    // ECRAN DEMARRAGE
	    case 0:
	      	welcomeScreen();
	      	alarme_setMinuteur(ui_tempoDem, &procGtmp); // Affiche pour 2 seconde
			state = 1;
	    break;

	    case 1:
	    	if (g_tmp != 0)  // Fin de l'ecran de demarrage
	    	{
	    		clearCmdButtons();
				lcd_clear();
				clock_reset();
				state = 2;
	    	}
	      	break;

	    // ECRAN D'ACCEUIL

	    case 2:
	    	afficheHome(1);  // Forcer l'affichage complet
	    	state = 21;
	    break;

	    case 21:
	    	afficheHome();
			// Attente de l'appuie du bouton gauche
			if ( ! isEmpty_btfifo())
			{
				char event = get_btfifo();
				if (event == BT_D_PRESSE)
				{
					// Affiche du menu
					state = 3;
					lcd_clear();
				}
			}
		    break;


	    // MENU PRINCIPAL

		case 3:
			select = afficheMenu(MAIN_MENU);
			if (select.retour == SELECT_OK)
			{
				switch (select.selection)
				{
					case 0:
						// Regler l'horloge
						state = 5;
						clearHorloge();
						lcd_clear();
					break;

					case 1:
						// Régler alarme
						state = 6;
						clearHorloge();
						lcd_clear();
					break;

					case 2 :
						// Donner à manger
						state = 4;
						lcd_clear();
					break;
				}
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				lcd_clear();
				state = 2;
			}

		break;


		// DONNER A MANGER

		case 4:
			if (giveFood() != MENU_NO_ACTION)
			{
				lcd_clear();
				state = 2;
			}
		break;
		
		// REGLAGE HORLOGE

		case 5:
			ret = setAclock(clocktmp);
			if (ret == MENU_OK)
			{
				// Mise a jour des données d'horodatage
				clock_setClock(horloge);
				// Retour en ecran d'acceuil
				lcd_clear();
				state = 2;
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state = 3;
			}
		break;
		
		// REGLAGE ALARME

		case 6:
			ret = setAnAlarm();
			if (ret == MENU_OK || ret == MENU_CANCEL)
			{
				// dans tous les cas retourner au menu principale
				lcd_clear();
				state = 3;
			}
		break;
	}
}


void procGtmp(void)
{
	g_tmp = 1;
}


void clearCmdButtons(void)
{
  // vider la fifo
  while (! isEmpty_btfifo())
  {
    get_btfifo();
  }
}


char giveFood(void)
{
	char retour = MENU_NO_ACTION;
	static Select_t select;
	
	select = afficheMenu(FEED_MENU);
	if (select.retour == SELECT_OK)
	{
		// Select.selection contient l'indice sur la quantité
		switch(select.selection)
		{
			case 0: moteur_setCmd(MT_PETIT); break;
			case 1:	moteur_setCmd(MT_MOYEN); break;
			case 2:	moteur_setCmd(MT_GRAND); break;
		}
		lcd_popup("feeding ...");
	}
	else if (select.retour == SELECT_CANCEL)
	{
		// retour à l'écran d'acceuil
		retour = MENU_CANCEL;
	}
	return retour;
}

// Méthode permettant de configurer un objet clock
char setAclock(clock p_clock)
{
	static uint8_t state = 0;
	char retour = MENU_NO_ACTION;
	static Select_t select;

	switch (state)
	{
		case 0:
			clearHorloge();
			if (p_clock.secondes >= 61)
			{
				*horloge = clock_getClock();  // recopie les données d'horloge dans la struc horloge
			}
			else
			{
				horloge->heures = p_clock.heures;
				horloge->minutes = p_clock.minutes;
				horloge->secondes = p_clock.secondes;
			}
			select = afficheMenu(CLOCK_HOUR_MENU, horloge->heures);
			state = 1;
		break;

		case 1:
			select = afficheMenu(CLOCK_HOUR_MENU);
			
			if (select.retour == SELECT_OK)
			{
				horloge->heures = select.selection;
				lcd_clear();
				state = 2;
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				retour = MENU_CANCEL;
				state = 0;
			}
			break;
			
		case 2 :
			select = afficheMenu(CLOCK_MIN_MENU, horloge->minutes);
			state = 3;
		break;

		case 3 :
			select = afficheMenu(CLOCK_MIN_MENU);
			if (select.retour == SELECT_OK)
			{
				horloge->minutes = select.selection;
				lcd_clear();
				state = 0;
				retour = MENU_OK;
				horloge->secondes = 0;
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran précédent
				lcd_clear();
				state = 1;
			}
		break;
	}
	return retour;
}


// retourne MENU_NO_ACTION, MENU_CANCEL, MENU_OK
char setAnAlarm(void)
{
	// gestion fsm
	static uint8_t state = 0;
	static Select_t select;
	char retour = MENU_NO_ACTION;
	
	// menu
	//static char ** item;
	//static Menu_t * al_menu = NULL;
	static char * item[6];
	static Menu_t al_menu;
	static clock pclk;
	pclk.heures = 0;
	pclk.minutes = 0;
	pclk.secondes = 62;
	char txt[11];
	//char * montitre;
	//char * alitem;
	char alitem[6];

	// alarme
	Alarme_t * pt_al = NULL;
	static uint8_t nbAl = 0;
	static Alarme_t * pt_al_tmp = NULL;
	
	char ret = 0; // variable temporaire

	static char tmpret = MENU_NO_ACTION;

	switch (state)
	{
		case 0:  // creation du menu
			// init des struct
			pt_al = alarme_getAlarme();  // fonction retournant un pointeur sur la premiere alarme config. (liste chainée d'alarmes)
			
			// Allouer le menu
			//al_menu = (Menu_t*)malloc(sizeof(Menu_t));
			//if (al_menu == NULL)
			//{
			//	state = 99;
			//	retour = MENU_CANCEL;
			//	break;
			//}
			// Allouer une chaine pour le titre
			//montitre = (char*)malloc(16);
			//if (montitre == NULL)
			//{
			//	state = 99;
			//	retour = MENU_CANCEL;
			//	break;
			//}
			//sprintf(montitre, "Regler alarme");

			// Accrocher le titre dans le menu
			//al_menu->titre = montitre;

			al_menu.titre = "Regler alarme";

			nbAl = 0;
			// Allouer le tableau
			//item = (char **)malloc((MAX_COUNT_ALARM + 1) * sizeof(char *));
			//if (item == NULL)
			//{
			//	state = 99;
			//	retour = MENU_CANCEL;
			//	break;
			//}
			// Allouer la premiere chaine et l'ajouter
			/*item[nbAl] = (char *)malloc(16);
			if (item[nbAl] == NULL)
			{
				state = 99;
				retour = MENU_CANCEL;
				break;
			}
			sprintf(txt, "Ajouter");
			strncpy(item[nbAl], txt, 8);
			item[nbAl][7] = 0;  // ajouter le \0 terminal
			*/
			item[0] = "Ajouter";
			nbAl ++;

			// Ajout des alarmes en mode texte
			pt_al_tmp = pt_al;
			while (pt_al_tmp)
			{
				// Allocation d'une chaine pour un item
				/*alitem = (char*)malloc(6);
				if (alitem == NULL)
				{
					state = 99;
					retour = MENU_CANCEL;
					break;
				}*/
				sprintf(alitem, "%02d:%02d", pt_al_tmp->horaire.heures, pt_al_tmp->horaire.minutes);
				//item[nbAl] = alitem;
				strncpy(item[nbAl], alitem, sizeof(alitem));
				//alitem = NULL;
				
				// log
				//sprintf(txt, "al : %02d:%02d\n", pt_al_tmp->horaire.heures, pt_al_tmp->horaire.minutes);
				//print_log(DEBUG, txt);

				nbAl ++;
				pt_al_tmp = pt_al_tmp->suivant;
			}
			al_menu.nbItem = nbAl;
			al_menu.items = item;

			state = 1;
			
		break;
		
		case 1:
			select = afficheMenu(al_menu);
			
			if (select.retour == SELECT_OK)
			{
				// ici on a selectionné qlq chose dans le menu : soit ajouter une nouvelle alarme soit une alarme existante
				if (select.selection == 0)
				{
					// Ajouter une nouvelle alarme
					if (nbAl >= MAX_COUNT_ALARM + 1)  // +1 pour prendre en compte l'indice du 'ajouter'
					{
						lcd_popup("Ajout interdit");
					}
					else
					{
						lcd_clear();
						state = 2;
					}
				}
				else
				{
					// Modifier une alarme
					lcd_clear();
					state = 4;
				}
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				tmpret = MENU_CANCEL;
				state = 99;
			}
		break;
		
		case 2:
			// Ajouter une nouvelle alarme
			//lcd_clear();
			pclk.secondes = 62;
			ret = setAclock(pclk);
			if (ret == MENU_OK)
			{
				if (horloge->heures == 0 && horloge->minutes == 0)
				{
					lcd_popup("Ajout interdit");
					state = 1;
				}
				else
				{
					// Ajout de l'alarme
					manager_setAlarme(*horloge);
					// Retour en ecran d'acceuil
					state = 99;
					tmpret = MENU_OK;
				}
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state = 1;
			}
		break;
		
		
		case 4: // Selection d'une alarme
		
		// on a le numéro de selection, il faut récupérer la bonne alarme.
		// faut parcourir la liste chainée en comptant le nombre d'objets
		pt_al_tmp = alarme_getAlarme();

		if (pt_al_tmp == NULL)
		{
			// erreur de pointeur
			state = 99;
			tmpret = MENU_CANCEL;
			lcd_popup("pt_al_tmp null");
			#ifdef MDEBUG1
			print_log(DEBUG, "pt_al_tmp null\n");
			#endif
			break;
		}

		ret = select.selection;
		ret --; // supprimer l'indice du "ajouter"
		while (ret > 0 && pt_al_tmp->suivant != NULL)
		{
			pt_al_tmp = pt_al_tmp->suivant;
			ret --;
		}
		
		if (pt_al_tmp == NULL)
		{
			// erreur d'indice détectée
			state = 99;
			tmpret = MENU_CANCEL;
			lcd_popup("pt_al null");
			#ifdef MDEBUG1
			print_log(DEBUG, "ptr null\n");
			#endif
			break;
		}
		//pclk = (clock*)malloc(sizeof(clock));
		pclk.heures = pt_al_tmp->horaire.heures;
		pclk.minutes = pt_al_tmp->horaire.minutes;
		pclk.secondes = 0;
		ret = setAclock(pclk);
		//free(pclk);

		state = 5;
		break;
		
		case 5:
			ret = setAclock(pclk);
			if (ret == MENU_OK)
			{
				// Si les valeurs sont à 0, supp l'alarme
				if (horloge->heures == 0 &&  horloge->minutes == 0)
				{
					// Supprimer l'alarme
					alarme_delAlarme(select.selection - 1); // -1 car l'indice 0 est le txt "Ajouter"
				}
				else
				{				
					// modifier en live l'alarme
					pt_al_tmp->horaire.heures = horloge->heures;
					pt_al_tmp->horaire.minutes = horloge->minutes;
				}
				// Retour en ecran d'acceuil
				state = 99;
				tmpret = MENU_OK;
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state = 1;
				/*if (pclk != NULL)
				{
					delete pclk;
					pclk = NULL;
				}*/
			}
		break;


		case 99:
			lcd_clear();
			state = 0;
			
			// Désallouer le tableau
			/*for (ret = 0; ret < nbAl; ret ++)
			{
				if (item[ret] != NULL)
				{
					free (item[ret]);
					item[ret] = NULL;
				}
			}
			free(item);
			item = NULL;*/
			// Désallouer le titre
			//free(montitre);
			//montitre = NULL;
			// Désallouer le menu
			//free(al_menu);
			//al_menu = NULL;

			nbAl = 0;
			pt_al = NULL;
			/*if (pclk != NULL)
			{
				delete pclk;
				pclk = NULL;
			}*/
			retour = tmpret;
		break;
	}
	
	return retour;
}

void clearHorloge(void)
{
	if (horloge != NULL)
	{
		horloge->heures = 0;
		horloge->minutes = 0;
		horloge->secondes = 0;
	}
}
