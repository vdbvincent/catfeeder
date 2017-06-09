/*
 * manager.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "manager.h"
#include "menu.h"
#include "moteur.h"

// Tempo de 4 heures
#ifdef MANDEBUG
uint16_t ui_tempoDist = 60;
#else
uint16_t ui_tempoDist = 14400;
#endif

#define MAX_CMP  300 /*50 c pas mal*/ // 600 car on est appelé toutes les 100ms et on veut 1 minute


void manager_setup(void)
{
	
	// Allumer le menu
	//menu_on();
}


void manager_every100ms(void)
{
	static uint8_t etat = ALLUMER_MENU;  // TODO : Décommenter

	static uint16_t cmp = 0;
	static uint8_t bpCounter = 0;
	static uint8_t oldBpCounter = 0;
	char event;
	
	switch (etat)
	{
		case ALLUMER_MENU:
			// Allumer le menu
			menu_SetMenuOn();
			etat = SURVEILLANCE_BP;
			bpCounter = boutons_getBPCounter();
			oldBpCounter = bpCounter;
			#ifdef MANDEBUG
			print_log(DEBUG, "manager : SURVEILLANCE_BP\n");
			#endif
		break;
		
		case ATTENTE_BP:
			bpCounter = boutons_getBPCounter();
			if (bpCounter == oldBpCounter)
			{
				oldBpCounter = bpCounter;
				etat = SURVEILLANCE_BP;
				cmp = 0;
				#ifdef MANDEBUG
				print_log(DEBUG, "manager : SURVEILLANCE_BP\n");
				#endif
			}
		break;
		
		case SURVEILLANCE_BP:
			// Compter le temps
			bpCounter = boutons_getBPCounter();
			if (bpCounter != oldBpCounter)
			{
				// Une action a été enregistrée
				cmp = 0;
				oldBpCounter = bpCounter;
				//etat = ATTENTE_BP;
				#ifdef MANDEBUG
				print_log(DEBUG, "manager : RESET_CMPT\n");
				#endif
			}
			else if (cmp >= MAX_CMP)
			{
				// Partir en mode sommeil
				etat = SOMMEIL;
				cmp = 0;
				menu_setMenuOff();
				#ifdef MANDEBUG
				print_log(DEBUG, "manager : SOMMEIL\n");
				#endif
			}
			else
			{
				cmp ++;
			}
		break;
		
		case SOMMEIL:
                  // Surveiller l'appuie d'un bouton et rallumer l'ecran
                  if ( ! isEmpty_btfifo())
                  {
                        event = get_btfifo();
                        if (event == BT_D_PRESSE)
                        {
                              // Rallumer l'écran
                              etat = ALLUMER_MENU;
                              #ifdef MANDEBUG
                              print_log(DEBUG, "manager : ALLUMER_MENU\n");
                              #endif
                        }
                        else if (event == BT_G_PRESSE)
                        {
                              // distrib moyen
                              moteur_setCmd(MT_MOYEN);
                        }
                        else if (event == BT_H_PRESSE)
                        {
                              moteur_setCmd(MT_GRAND);
                        }
                        else if (event == BT_B_PRESSE)
                        {
                              moteur_setCmd(MT_PETIT);
                        }
                  }
		break;
	}
}

void manager_setAlarme(clock p_horloge)
{
	alarme_setAlarme(p_horloge, &manager_procAlarme, 1);
}

void manager_procAlarme(void)
{
	// faire tourner le moteur
#ifdef MANDEBUG
	print_log(DEBUG, "manager : grande  distrib OK\n");
#endif
	moteur_setCmd(MT_GRAND);
}

void manager_procMinut(void)
{
#ifdef MANDEBUG
	print_log(DEBUG, "manager : petite distrib OK\n");
#endif
	moteur_setCmd(MT_PETIT);
}
