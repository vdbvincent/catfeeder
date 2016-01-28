/*
 * manager.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "manager.h"

// Tempo de 4 heures
#ifdef DEBUG
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
*/


void manager_idle(void)
{
	if ((clock_isInit() == 0) && (state == 0))
	{
		// Si l'heure n'est pas réglée
		// il doit y avoir 1 distrib MOYEN / 4h
		print_log(DEBUG, "manager : distrib prevue dans 60s\n");
		alarme_setMinuteur(ui_tempoDist, &procGtmp);
		state = 1;
	}

}

void manager_setAlarme(clock p_horloge, uint8_t p_selection)
{
	alarme_setAlarme(p_horloge, p_selection, &manager_procAlarme);
}

void manager_procAlarme(void)
{
	// faire tourner le moteur
#ifdef DEBUG
	print_log(DEBUG, "manager : grande  distrib OK\n");
#endif
	moteur_setCmd(MT_GRAND);
}

void manager_procMinut(void)
{
	if (state == 1)
	{
		// faire tourner le moteur
#ifdef DEBUG
		print_log(DEBUG, "manager : petite distrib NON OK\n");
#endif
		moteur_setCmd(MT_PETIT);
	}
}