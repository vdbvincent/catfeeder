/*
 * menu.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "menu.h"

// Variables globales
static uint8_t ui_tempoDem = 2;  // Valeur par défaut 300 = 2s
uint8_t g_tmp = 0;
clock horloge;
static uint8_t state_affmenu = 0;
static uint8_t state_setAclock = 0;
static uint8_t state_setAnAlarm = 0;
static uint8_t mb_actif = 1; // Booleen indiquant si le menu est à afficher (oui par défaut)

void menu_setup(uint8_t p_tempoDem)
{
	// Configuration de la tempo de l'écran de démarrage
 	ui_tempoDem = p_tempoDem;
	//horloge = (clock*)malloc(sizeof(clock));
	horloge.heures = 0;
	horloge.minutes = 0;
	horloge.secondes = 0;
	menu_on();
}

void menu_idle(void)
{
	// Gestion de l'affichage des menus
	if (mb_actif == 1)
		menu_affMenu();
	else if (mb_actif == 0)
		menu_off();
	else if (mb_actif == 2)
		menu_on();
}

void menu_on(void)
{
	lcd_on();
	clearCmdButtons();
	mb_actif = 1;
}

void menu_setMenuOff(void)
{
	mb_actif = 0;
}

void menu_SetMenuOn(void)
{
	mb_actif = 2;
}

void menu_off(void)
{
	lcd_off();
	clearHorloge();
	clearCmdButtons();
	// reset les variables state car peut s'eteindre en cours de menu
	state_affmenu = 2;
	state_setAclock = 0;
	state_setAnAlarm = 0;
	mb_actif = 3;
}

void menu_affMenu(void)
{
	
	static Select_t select;
	char ret;  // Variable temporaire
	static clock clocktmp;
	clocktmp.heures = 0;
	clocktmp.minutes = 0;
	clocktmp.secondes = 62;

	switch (state_affmenu)
	{
	    	// ECRAN DEMARRAGE
		case 0:
			g_tmp = 0;
			welcomeScreen();
			alarme_setMinuteur(ui_tempoDem, &procGtmp); // Affiche pour 2 seconde
			state_affmenu = 1;
			#ifdef MEDEBUG
			print_log(INFO, "menu: 0->1\n");
			#endif
		break;

	    	case 1:
			if (g_tmp != 0)  // Fin de l'ecran de demarrage
			{
				clearCmdButtons();
				lcd_clear();
				clock_reset();
				state_affmenu = 2;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 1->2\n");
				#endif
			}
		break;

		// ECRAN D'ACCEUIL

		case 2:
			afficheHome(1);  // Forcer l'affichage complet
			state_affmenu = 21;
			#ifdef MEDEBUG
			print_log(INFO, "menu: 2->21\n");
			#endif
		break;

		case 21:
			afficheHome();
			// Attente de l'appuie du bouton droit
			if ( ! isEmpty_btfifo())
			{
				char event = get_btfifo();
				if (event == BT_D_PRESSE)
				{
					// Affiche du menu
					state_affmenu = 3;
					lcd_clear();
					#ifdef MEDEBUG
					print_log(INFO, "menu: 21->3\n");
					#endif
				}
			}
		break;


	    	// MENU PRINCIPAL

		case 3:
			select = afficheMenu(&MAIN_MENU, 255);  // TODO : tester si en mode sommeil avec menu ouvert et reallumer -> bug ?
			if (select.retour == SELECT_OK)
			{
				switch (select.selection)
				{
					case 0:
						// Regler l'horloge
						state_affmenu = 5;
						clearHorloge();
						lcd_clear();
						#ifdef MEDEBUG
						print_log(INFO, "menu: 3->5\n");
						#endif
					break;

					case 1:
						// Régler alarme
						state_affmenu = 6;
						clearHorloge();
						lcd_clear();
						#ifdef MEDEBUG
						print_log(INFO, "menu: 3->6\n");
						#endif
					break;

					case 2 :
						// Donner à manger
						state_affmenu = 4;
						lcd_clear();
						#ifdef MEDEBUG
						print_log(INFO, "menu: 3->4\n");
						#endif
					break;
				}
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				lcd_clear();
				state_affmenu = 2;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 3->2\n");
				#endif
			}

		break;


		// DONNER A MANGER

		case 4:
			if (giveFood() != MENU_NO_ACTION)
			{
				lcd_clear();
				state_affmenu = 2;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 4->2\n");
				#endif
			}
		break;
		
		// REGLAGE HORLOGE
		case 5:
			ret = setAclock(clocktmp);
			if (ret == MENU_OK)
			{
				// Mise a jour des données d'horodatage
				clocktmp.heures = horloge.heures;
				clocktmp.minutes = horloge.minutes;
				clocktmp.secondes = horloge.secondes;
				
				clock_setClock(clocktmp);
				// Retour en ecran d'acceuil
				lcd_clear();
				state_affmenu = 2;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 5->2\n");
				#endif
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state_affmenu = 3;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 5->2\n");
				#endif
			}
		break;
		
		// REGLAGE ALARME

		case 6:
			ret = setAnAlarm();
			if (ret == MENU_OK || ret == MENU_CANCEL)
			{
				// dans tous les cas retourner au menu principale
				//lcd_clear();
				state_affmenu = 3;
				#ifdef MEDEBUG
				print_log(INFO, "menu: 6->3\n");
				#endif
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
	
	select = afficheMenu(&FEED_MENU, 255);
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
	//static uint8_t state = 0;
	char retour = MENU_NO_ACTION;
	static Select_t select;
	clock ctmp;

	switch (state_setAclock)
	{
		case 0:
			clearHorloge();
			if (p_clock.secondes >= 61)
			{
				//*horloge = clock_getClock();  // recopie les données d'horloge dans la struc horloge
				ctmp = clock_getClock();
				horloge.heures = ctmp.heures;
				horloge.minutes = ctmp.minutes;
				horloge.secondes = ctmp.secondes;
				//horloge = clock_getClock();
			}
			else
			{
				horloge.heures = p_clock.heures;
				horloge.minutes = p_clock.minutes;
				horloge.secondes = p_clock.secondes;
			}
			select = afficheMenu(&CLOCK_HOUR_MENU, horloge.heures);
			state_setAclock = 1;
		break;

		case 1:
			select = afficheMenu(&CLOCK_HOUR_MENU, 255);
			
			if (select.retour == SELECT_OK)
			{
				horloge.heures = select.selection;
				lcd_clear();
				state_setAclock = 2;
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				retour = MENU_CANCEL;
				state_setAclock = 0;
			}
			break;
			
		case 2 :
			select = afficheMenu(&CLOCK_MIN_MENU, horloge.minutes);
			state_setAclock = 3;
		break;

		case 3 :
			select = afficheMenu(&CLOCK_MIN_MENU, 255);
			if (select.retour == SELECT_OK)
			{
				horloge.minutes = select.selection;
				lcd_clear();
				state_setAclock = 0;
				retour = MENU_OK;
				horloge.secondes = 0;
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran précédent
				lcd_clear();
				state_setAclock = 1;
			}
		break;
	}
	return retour;
}



// retourne MENU_NO_ACTION, MENU_CANCEL, MENU_OK
char setAnAlarm(void)
{
	// gestion fsm
	static Select_t select;
	char retour = MENU_NO_ACTION;
	
	// menu
	static clock pclk;
	pclk.heures = 0;
	pclk.minutes = 0;
	pclk.secondes = 62;
	char alitem[8];
	char txt[8];

	// alarme
	static uint8_t nbAl = 0;
	static uint8_t tabselect[MAX_COUNT_ALARM];
	uint8_t indice = 0;
	
	char ret = 0; // variable temporaire
	uint8_t i = 0; // indice de boucle

	static char tmpret = MENU_NO_ACTION;

	switch (state_setAnAlarm)
	{
		case 0:  // creation du menu
			// init du nombre d'entrées dans le menu d'alarmes
			nbAl = 0;
			nbAl ++;  // La premiere entrée est le texte "ajouter"

			while (i < MAX_COUNT_ALARM)
			{
			    pclk = alarme_getAlarme(i);
			    if (pclk.heures != 0 || pclk.minutes != 0)
			    {
					sprintf(SSALARM_MENU_ITEMS[nbAl], "%02d:%02d  ", pclk.heures, pclk.minutes);
					nbAl ++;
					// Retenir l'indice de l'alarme pour la selection
					tabselect[indice] = i; /// TODO : utiliser nbAl-1 ? pour gagner 8 bits
					indice ++;
			    }
			    i++;
			}
			SSALARM_MENU.nbItem = nbAl;
			indice = 0;
			state_setAnAlarm = 1;
		break;
		
		case 1:
			select = afficheMenu(&SSALARM_MENU, 255);
			
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
						state_setAnAlarm = 2;
					}
				}
				else
				{
					// Modifier une alarme
					lcd_clear();
					state_setAnAlarm = 4;
				}
			}
			else if (select.retour == SELECT_CANCEL)
			{
				// retour à l'écran d'acceuil
				tmpret = MENU_CANCEL;
				state_setAnAlarm = 99;
			}
		break;
		
		case 2:
			// Ajouter une nouvelle alarme
			pclk.secondes = 62;
			ret = setAclock(pclk);
			if (ret == MENU_OK)
			{
				if (horloge.heures == 0 && horloge.minutes == 0)
				{
					lcd_popup("Ajout interdit");
					state_setAnAlarm = 1;
				}
				else
				{
					// Ajout de l'alarme
					clock ctmp;
					ctmp.heures = horloge.heures;
					ctmp.minutes = horloge.minutes;
					ctmp.secondes = horloge.secondes;
					manager_setAlarme(ctmp);
					lcd_popup("Alarme ajoutee");

					// Retour en ecran d'acceuil
					state_setAnAlarm = 99;
					tmpret = MENU_OK;
					clearHorloge();
				}
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state_setAnAlarm = 1;
			}
		break;
		
		
		case 4: // Selection d'une alarme
			{
				pclk = alarme_getAlarme(tabselect[select.selection-1]);  // supprimer l'indice du "ajouter"
				
				pclk.secondes = 0;
				ret = setAclock(pclk);

				state_setAnAlarm = 5;
			}
		break;
		
		case 5:
			ret = setAclock(pclk);
			if (ret == MENU_OK)
			{
				// Si les valeurs sont à 0, supp l'alarme
				if (horloge.heures == 0 && horloge.minutes == 0)
				{
					// Supprimer l'alarme
					alarme_delAlarme(tabselect[select.selection-1]);
					//alarme_delAlarme(i);
					lcd_popup("Alarme supprimee");
				}
				else
				{
					clock ctmp;
					ctmp.heures = horloge.heures;
					ctmp.minutes = horloge.minutes;
					ctmp.secondes = horloge.secondes;
					alarme_modify(ctmp, tabselect[select.selection-1]);
					//alarme_delAlarme(i);
					lcd_popup("Alarme modifiee");
				}
				// Retour en ecran d'acceuil
				state_setAnAlarm = 99;
				tmpret = MENU_OK;
			}
			else if (ret == MENU_CANCEL)
			{
				// Réglage annulé par l'utilisateur. Retour au menu
				lcd_clear();
				state_setAnAlarm = 1;
			}
		break;


		case 99:
			//lcd_clear();
			state_setAnAlarm = 0;
			
			// Désallouer le tableau
			/*for (ret = 1; ret < 6; ret ++)
			{
				//SSALARM_MENU_ITEMS[ret][0] = '\0';
				sprintf(SSALARM_MENU_ITEMS[ret], "");
			}*/
			nbAl = 0;
			retour = tmpret;
		break;
	}
	
	return retour;
}

void clearHorloge(void)
{
	horloge.heures = 0;
	horloge.minutes = 0;
	horloge.secondes = 0;	
}
