/*
 * menu.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "menu.h"

static unsigned int ui_tempoDem = 2;  // Valeur par défaut 300 = 2s

uint8_t g_tmp = 0;

void menu_setup(unsigned int p_tempoDem)
{
	// Configuration de la tempo de l'écran de démarrage
  ui_tempoDem = p_tempoDem;
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
	clock horloge;
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
						lcd_clear();
					break;

					case 1:
						// Régler alarme
						state = 6;
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

		case 4:  // TODO : envoyer un ordre a la fonction qui gere les modes de fonctionnement
			//if (feedTheCat() == MENU_NO_EVENT)
			//{
				// Lorsque la distribution est terminée, aller en 2
				state = 2;
			//}
		break;
		
		// REGLAGE HORLOGE

		case 5:
			ret = setAclock(&horloge);
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
			select = afficheMenu(ALARME_MENU);
			if (select.retour == SELECT_OK)
			{
				state = 7;
				lcd_clear();
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour au menu
				lcd_clear();
				state = 3;
			}
		break;

		case 7:
			// Appeler SetAclock pour conf une alarme et la stocker dans l'objet alarme à l'indice select.selection
			ret = setAclock(&horloge);
			if (ret == MENU_OK)
			{
				// Mise a jour des données d'horodatage
				//manager_setAlarme(horloge, select.selection);
				//alarme_setAlarme(horloge, select.selection, mycallback); // TODO : appeler une fonction setalarme de la tache manager
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
			else
			{
				// Aucune action a faire ici.
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
char setAclock(clock * p_cHorloge)
{
	static char state = 1;
	char retour = MENU_NO_ACTION;
	static Select_t select;
	
	switch (state)
	{
		case 1:
		select = afficheMenu(CLOCK_HOUR_MENU);

		if (select.retour == SELECT_OK)
		{
			p_cHorloge->heures = select.selection;
			lcd_clear();
			state = 2;
		}
		else if (select.retour == SELECT_CANCEL)
		{
			// retour à l'écran d'acceuil
			retour = MENU_CANCEL;
		}
		break;
		
		case 2 :
		select = afficheMenu(CLOCK_MIN_MENU);

		if (select.retour == SELECT_OK)
		{
			p_cHorloge->minutes = select.selection;
			lcd_clear();
			state = 1;
			retour = MENU_OK;
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