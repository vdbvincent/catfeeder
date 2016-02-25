/*
 * manager.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "manager.h"

// Tempo de 4 heures
#ifdef MDEBUG
uint16_t ui_tempoDist = 60;
#else
uint16_t ui_tempoDist = 14400;
#endif

#define MAX_CMP  600 // 600 car on est appelé toutes les 100ms et on veut 1 minute


void manager_setup(void)
{
	// Configurer le port en sortie
	// Allumer l'écran
}


/*
TODO :
- eteindre le LCD apres 1 minute d'inactivité
- Donner l'ordre au menu de clignoter si l'heure n'est pas réglée
- Quand l'ecran est eteind :
	* bouton haud : reveil
	* bouton gauche : petite ration
	* bouton bas : moyenne ration
	* bouton droit : grande ration

TODO : doit appeler menu_idle() avec en param les event bouton pour les centraliser ici

le lcd doit avoir une fonction on() et off() pr piloter le lcd
digitalWrite(PinLCDBacklight, LOW); // Retro-éclairage éteint

pour le moment le besoin :
eteindre l'écran et gérer les boutons.

// TODO : appeler un menu_reset() pour réinit de toutes les variables sur fermeture ecran
le manager doit etre appeler tout les 100ms
il doit verifier si la fifo bouton est vide ou pas
si elle est vide commencer a compter le temps
a chaque evenement, il doit reset son compteur
si le compteur arrive à 1 minute, demander au menu de se couper. Menu appelera egalement un lcd_off() pour eteindre l'écran
le manager rentre dans un etat d'attente d'evenement de boutons, le menu n'est plus appelé (shunt dans menu_idle())
il doit veiller sur les events (fifo.front())
si appuie bouton droit, demander au menu de se remettre en route et de rallumer l'écran.
*/


void manager_100ms(void)
{
	static uint8_t etat = 0;
	static uint8_t cmp = 0;
	char event;
	
	/*if ( ! isEmpty_btfifo())
	{
		event = get_btfifo();
	}*/
	
	
	switch (etat)
	{
		case 0:
			// Allumer le menu
			menu_on();
			state = 1;
		break;
		
		case 1:
			if (isEmpty_btfifo())  // Si aucune action
			{
				state = 2;
				cmp = 0;
			}
		break;
		
		case 2:
			// Compter le temps
			if (! isEmpty_btfifo())
			{
				// Une action a été enregistrée
				cmp = 0;
				state = 1;
			}
			else if (cmp >= MAX_CMP)
			{
				// Partir en mode sommeil
				state = 3;
				cmp = 0;
				menu_off();
			}
			else
			{
				cmp ++;
			}
		break;
		
		case 3:
			// Surveiller l'appuie d'un bouton et rallumer l'ecran
			event = get_btfifo();
			if (event == BT_D_PRESSE)
			{
				// Rallumer l'écran
				state = 0;
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
		break;
		
		case 10:
			// Surveiller l'inactivité
			if (event == NO_EVENT)
			{
				// Déclencher un minuteur
				state = 2;
			}
		break;
		
		case 20:
			// Attendre le déclenchement d'un muinuteur
			
			// ou si appui bouton enlever le minuteur et repartie en état 1 ?
		break;
	}
}

void manager_setAlarme(clock p_horloge)
{
	alarme_setAlarme(p_horloge, &manager_procAlarme);
}

void manager_procAlarme(void)
{
	// faire tourner le moteur
#ifdef MDEBUG1
	print_log(DEBUG, F("manager : grande  distrib OK\n"));
#endif
	moteur_setCmd(MT_GRAND);
	// si le lcd est allumé, faire apparaitre une popup
}

void manager_procMinut(void)
{
	if (state == 1)
	{
		// faire tourner le moteur
#ifdef MDEBUG
		print_log(DEBUG, F("manager : petite distrib NON OK\n"));
#endif
		moteur_setCmd(MT_PETIT);
	}
}
