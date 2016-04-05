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

TODO : doit appeler menu_idle() avec en param les event bouton pour les centraliser ici => non
       doit demander au menu de s'eteindre : désallouer des trucs ou fermer des machins
       puis demander au main d'arreter de l'appeler: eviter un appel de fonction inutile
       si appui sur le btn de reveil, demander au main d'appeler le menu, il va s'init tout seul comme un grand
       => ca impose de gerer la concurrence d'accès à la fifo bouton :/
       => ca permet d'appeler le manager depuis l'it 100ms pour compter le temps et eviter de déclencher des 
          minuteurs qui vont faire foirer la mémoire.

le lcd doit avoir une fonction on() et off() pr piloter le lcd
digitalWrite(PinLCDBacklight, LOW); // Retro-éclairage éteint


// TODO : appeler un menu_reset() pour réinit de toutes les variables sur fermeture ecran
le manager doit etre appeler tout les 100ms
il doit verifier si la fifo bouton est vide ou pas
si elle est vide commencer a compter le temps
a chaque evenement, il doit reset son compteur
si le compteur arrive à 1 minute, demander au menu de se couper. Menu appelera egalement un lcd_off() pour eteindre l'écran
le manager rentre dans un etat d'attente d'evenement de boutons, le menu n'est plus appelé
il doit veiller sur les events (fifo.front())
si appuie bouton droit, demander au menu de se remettre en route et de rallumer l'écran.
*/


void manager_every100ms(void)
{

	static uint8_t etat = ALLUMER_MENU;
	static uint16_t cmp = 0;
	static uint8_t bpCounter = 0;
	static uint8_t oldBpCounter = 0;
	char event;
	
	
	switch (etat)
	{
		case ALLUMER_MENU:
			// Allumer le menu
			menu_on();
			etat = SURVEILLANCE_BP;
			bpCounter = boutons_getBPCounter();
			oldBpCounter = bpCounter;
			#ifdef MDEBUG2
			print_log(DEBUG, "SURVEILLANCE_BP\n");
			#endif
		break;
		
		case ATTENTE_BP:
			bpCounter = boutons_getBPCounter();
			if (bpCounter == oldBpCounter)
			{
				oldBpCounter = bpCounter;
				etat = SURVEILLANCE_BP;
				cmp = 0;
				#ifdef MDEBUG2
				print_log(DEBUG, "SURVEILLANCE_BP\n");
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
				#ifdef MDEBUG2
				print_log(DEBUG, "RESET_CMPT\n");
				#endif
			}
			else if (cmp >= MAX_CMP)
			{
				// Partir en mode sommeil
				etat = SOMMEIL;
				cmp = 0;
				menu_off();
				#ifdef MDEBUG2
				print_log(DEBUG, "SOMMEIL\n");
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
					#ifdef MDEBUG2
					print_log(DEBUG, "ALLUMER_MENU\n");
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
#ifdef MDEBUG1
	print_log(DEBUG, "manager : grande  distrib OK\n");
#endif
	moteur_setCmd(MT_GRAND);
	// si le lcd est allumé, faire apparaitre une popup
}

void manager_procMinut(void)
{
#ifdef MDEBUG
	print_log(DEBUG, "manager : petite distrib NON OK\n");
#endif
	moteur_setCmd(MT_PETIT);
}
