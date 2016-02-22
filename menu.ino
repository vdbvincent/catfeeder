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
 	horloge = new clock();
}

void menu_idle(void)
{
	// TODO : gerer les popup
	
	// TODO : gerer les cas dégradés : faire clignoter le menu
	
	// Gestion de l'affichage des menus
	menu_affMenu();
}
	
static void menu_affMenu(void)
{
	static uint8_t state = 0;
	static Select_t select;
	char ret;  // Variable temporaire

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
			select = afficheMenu(&MAIN_MENU);
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

		case 4:  // TODO : envoyer un ordre au manager et avant demander la quantité dans le menu
			//if (feedTheCat() == MENU_NO_EVENT)
			//{
				// Lorsque la distribution est terminée, aller en 2
				state = 2;
			//}
		break;
		
		// REGLAGE HORLOGE

		case 5:
			ret = setAclock();
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

// Méthode permettant de configurer un objet clock
// TODO : doit prendre une struct clock en parametre pour que setAlarme s'en serve en faculatif

// pour regler les alarmes, il faut faire une fsm a part pour:
// recup un tableau d'alarme avec leur nombre
// Appeler une fonction setAlarme dans le menu qui permets d'afficher les différents écran via une fsm
// appeler une fonction dans le lcd regleTrigger avec en param le tableau d'alarmes
// Le resultat sera propagé jusque dans la fsm ici avec

char setAclock(clock * p_clock)
{
	static uint8_t state = 0;
	char retour = MENU_NO_ACTION;
	static Select_t select;

	switch (state)
	{
		case 0:
			clearHorloge();
			if (p_clock == NULL)
			{
				*horloge = clock_getClock();  // recopie les données d'horloge dans la struc horloge
			}
			else
			{
				horloge = p_clock;  // Copie du pointeur p_clock dans horloge. horloge pointe direct sur la struct clock de setAnAlarm
			}
			select = afficheMenu(&CLOCK_HOUR_MENU, horloge->heures);
			state = 1;
		break;

		case 1:
			select = afficheMenu(&CLOCK_HOUR_MENU);
			
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
			select = afficheMenu(&CLOCK_MIN_MENU, horloge->minutes);
			state = 3;
		break;

		case 3 :
			select = afficheMenu(&CLOCK_MIN_MENU);
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
	static uint8_t state = 0;
	static Select_t select;
	char retour = MENU_NO_ACTION;
	
	//static char * tabitem[MAX_COUNT_ALARM + 1];
	//static char * tabitem;
	static char ** item;
	//item = (char**)tabitem;

	static Menu_t * al_menu ;//= {"Regler alarme", item, 0};
	
	static Alarme_t * pt_al = NULL;
	static uint8_t nbAl = 0;
	
	static clock * pclk = NULL;
	
	Alarme_t * pt_al_tmp = NULL;
	char txt[8];
	char * montitre;
	char ret = 0; // variable temporaire

	switch (state)
	{
		case 0:  // creation du menu
			// init des struct
			pt_al = alarme_getAlarme();  // fonction retournant un pointeur sur la premiere alarme config. (liste chainée d'alarmes)
			
			// Allouer le menu
			al_menu = (Menu_t*)malloc(sizeof(Menu_t));
			// Allouer une chaine pour le titre
			montitre = (char*)malloc(16);
			sprintf(montitre, "Regler alarme");

			// Accrocher le titre dans le menu
			al_menu->titre = montitre;

			nbAl = 0;
			// Allouer le tableau
			item = (char **)malloc((MAX_COUNT_ALARM + 1) * sizeof(char *));
			// Allouer la premiere chaine et l'ajouter
			item[nbAl] = (char *)malloc(16);
			sprintf(txt, "Ajouter");
			strncpy(item[nbAl], txt, 8);
			item[nbAl][7] = 0;  // ajouter le \0 terminal

			nbAl ++;

			// Ajout des alarmes en mode texte
			pt_al_tmp = pt_al;
			while (pt_al_tmp)
			{
				// Allocation d'une chaine pour un item
				char * alitem = (char*)malloc(16);
				sprintf(alitem, "%02d:%02d", pt_al_tmp->horaire.heures, pt_al_tmp->horaire.minutes);
				item[nbAl] = alitem,
				
				//sprintf(txt, "%02d:%02d", pt_al_tmp->horaire.heures, pt_al_tmp->horaire.minutes);
				// Allouer la chaine et l'ajouter
				//item[nbAl] = (char *)malloc(16);
				//strncpy(item[nbAl], txt, 6);
				//item[nbAl][5] = 0;  // ajouter le \0 terminal

				// log
				sprintf(txt, "al : %02d:%02d\n", pt_al_tmp->horaire.heures, pt_al_tmp->horaire.minutes);
				print_log(DEBUG, txt);

				nbAl ++;
				pt_al_tmp = pt_al_tmp->suivant;
			}
			al_menu->nbItem = nbAl;
			al_menu->items = item;

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
				retour = MENU_CANCEL;
				state = 99;
			}
		break;
		
		case 2:
			// Ajouter une nouvelle alarme
			//lcd_clear();
			ret = setAclock();
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
					retour = MENU_OK;
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
		pt_al_tmp = pt_al;
		ret = select.selection;
		while (ret > 1)
		{
			pt_al_tmp = pt_al_tmp->suivant;
			ret --;
		}
		// ici pt_al_tmp pointe sur l'alarme à modifier
		pt_al = pt_al_tmp;  // ici pt_al pointe sur l'alarme à modifier
		pclk = new clock();
		pclk->heures = pt_al_tmp->horaire.heures;
		pclk->minutes = pt_al_tmp->horaire.minutes;

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
					pt_al->horaire.heures = horloge->heures;
					pt_al->horaire.minutes = horloge->minutes;
				}
				// Retour en ecran d'acceuil
				state = 99;
				retour = MENU_OK;
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state = 1;
			}
		break;


		case 99:
			lcd_clear();
			state = 0;
			
			// Désallouer le tableau
			for (ret = 0; ret < nbAl; ret ++)
			{
				if (item[ret])
				{
					free (item[ret]);
				}
			}
			free(item);
			// Désallouer le titre
			free(montitre);
			// Désallouer le menu
			free(al_menu);


			nbAl = 0;
			pt_al = NULL;
			pclk = NULL;
		break;
	}
	
	return retour;
}

void clearHorloge(void)
{
	horloge->heures = 0;
	horloge->minutes = 0;
	horloge->secondes = 0;
}


// TODO : a mettre dans un fsm a part qui chapote un peu tou

/*
// Méthode permettant de controler la distribution de nourriture. Retourne 0 lorsque c'est fini. 1 sinon.
char feedTheCat(void)
{
  static char state = 0;
  static char event = NO_EVENT;
  char retour = MENU_EVENT;
  
  switch (state)
  {
    case 0:
      // Envoyer un evenement de nourriture au moteur
      if ( ! isFull_mtfifo())
      {
      	lcd_clear();
        put_mtfifo(1); // TODO :
        lcd_popup("WAIT FOR START", 99);
        state = 1;
      }
      else
      {
        lcd_popup("ERROR FEEDING", 1); // 2sec  // TODO :
      }
    break;
    
    case 1:
      // Attente du demarrage du moteur
      if ( ! isEmpty_lcdfifo())
      {
        event = get_lcdfifo();
        if (event == BEGIN)
        {
          lcd_clear();
          lcd_popup("Please wait ...", 99);
        }
        if (event == END)
        {
          lcd_clear();

          state = 0;
          clearCmdButtons(); // Nettoyer la fifo d'evenement bouttons pour virer les parasites
          retour = MENU_NO_EVENT;
        }
      }
    break;
  }

  return retour;
}*/
