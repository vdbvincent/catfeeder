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

static uint8_t state = 0;

void manager_setup(void)
{

}


/*
TODO :
- si l'heure n'a pas été réglé, partir en cas dégradé
sinon laisser faire les menus et réagir aux alarmes
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
//lcd.noDisplay(); // désactive l'affichage texte

au début faut laisser faire, genre attendre 5 minutes
ensuite soit l'utilisateur n'a pas config horloge + alarme en config 2 ou 3 par défaut et
le signaler sur l'écran avec un symbole de mode par defaut ou un message.

pour le moment le besoin :
eteindre l'écran et gérer les boutons.

// TODO : appeler un menu_reset() pour réinit de toutes les variables sur fermeture ecran
*/


void manager_idle(void)
{
	static uint8_t etat = 0;
	char event_bt = NO_EVENT;
	
	/*if ( ! isEmpty_btfifo())
	{
		event = get_btfifo();
	}
	menu_idle(event);*/
	
	switch (etat)
	{
		case 0:
			// surveiller
		break;
		
		case 1:
		break;
	}
	
	/*if ((state == 0) && (clock_isInit() == 0))
	{
		// Si l'heure n'est pas réglée
		// il doit y avoir 1 distrib MOYEN / 4h
		print_log(DEBUG, "manager : distrib prevue dans 60s\n");
		alarme_setMinuteur(ui_tempoDist, &procGtmp);
		state = 1;
	}*/

}

void manager_setAlarme(clock p_horloge)
{
	alarme_setAlarme(p_horloge, &manager_procAlarme);
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
	if (state == 1)
	{
		// faire tourner le moteur
#ifdef MDEBUG
		print_log(DEBUG, "manager : petite distrib NON OK\n");
#endif
		moteur_setCmd(MT_PETIT);
	}
}
