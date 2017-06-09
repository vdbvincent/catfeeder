/*
 * boutons.c
 *
 * Created on : 27 janv. 2017
 * Author : vincent
 *
*/
#include "QueueArray.h"
#include "boutons.h"

/* Définitions */
#define FREQ_SEQ 100  // Fréquence d'appel de la méthode every100ms()

/* Variables globales */
uint16_t filtre = 0;  // Delai de filtrage du filtre anti-rebonds
const uint8_t BP_G = 0;
const uint8_t BP_D = 1;
const uint8_t BP_H = 2;
const uint8_t BP_B = 3;
CTXT_BP sg_bptab[4]; // Tableau regroupant les différents boutons

// variable globale de comptage d'appuies bouton pour associer manager a menu
uint8_t mb_com_apbp = 0;

// Définition de la fifo
QueueArray<char>   f_boutons;

void boutons_setup(uint16_t p_delayFiltre_ms)
{
      // Init du filtrage anti-rebond
      filtre = p_delayFiltre_ms / FREQ_SEQ;

      // Init des contextes
      sg_bptab[BP_G].state = NON_PRESSE;
      sg_bptab[BP_G].retEtat = AUCUN_EVENEMENT;
      sg_bptab[BP_G].icpt = 0;
      sg_bptab[BP_G].pin = MCUBIND_VIRTUALPORT_B_b11;

      sg_bptab[BP_D].state = NON_PRESSE;
      sg_bptab[BP_D].retEtat = AUCUN_EVENEMENT;
      sg_bptab[BP_D].icpt = 0;
      sg_bptab[BP_D].pin = MCUBIND_VIRTUALPORT_B_b12;

      sg_bptab[BP_H].state = NON_PRESSE;
      sg_bptab[BP_H].retEtat = AUCUN_EVENEMENT;
      sg_bptab[BP_H].icpt = 0;
      sg_bptab[BP_H].pin = MCUBIND_VIRTUALPORT_D_b6;

      sg_bptab[BP_B].state = NON_PRESSE;
      sg_bptab[BP_B].retEtat = AUCUN_EVENEMENT;
      sg_bptab[BP_B].icpt = 0;
      sg_bptab[BP_B].pin = MCUBIND_VIRTUALPORT_B_b8;

      // Init des entrées en input
      mcubind_virtualport_init(MCUBIND_VIRTUALPORT_D_b6,  0);
      mcubind_virtualport_init(MCUBIND_VIRTUALPORT_B_b8,  0);
      mcubind_virtualport_init(MCUBIND_VIRTUALPORT_B_b11, 0);
      mcubind_virtualport_init(MCUBIND_VIRTUALPORT_B_b12, 0);
      // Déclaration des pullup interne
      // Pour activer la résistance de pullup interne, lorsque le pin est config en INPUT,
      //il faut ecrire 1 sur le registre PORTD sur la pin correspondante
      mcubind_virtualport_write(MCUBIND_VIRTUALPORT_D_b6,  1);
      mcubind_virtualport_write(MCUBIND_VIRTUALPORT_B_b8,  1);
      mcubind_virtualport_write(MCUBIND_VIRTUALPORT_B_b11, 1);
      mcubind_virtualport_write(MCUBIND_VIRTUALPORT_B_b12, 1);

      // init du nb d'appuies bp
      mb_com_apbp = 0;
}

void boutons_every100ms(void)
{
	// lire les entrées bouton et générer des évènements

      lireEvenement(BP_G);
      if (sg_bptab[BP_G].retEtat == EVENEMENT_PRESSE)
      {
            sendEvent(BT_G_PRESSE);
            #ifdef BDEBUG
            print_log(INFO, "BT_G_PRESSE\n");
            #endif
      }
      else if (sg_bptab[BP_G].retEtat == EVENEMENT_RELACHE)
      {
            sendEvent(BT_G_RELACHE);
            #ifdef BDEBUG
            print_log(INFO, "BT_G_RELACHE\n");
            #endif
      }
      

      lireEvenement(BP_D);
      if (sg_bptab[BP_D].retEtat == EVENEMENT_PRESSE)
      {
            sendEvent(BT_D_PRESSE);
            #ifdef BDEBUG
            print_log(INFO, "BT_D_PRESSE\n");
            #endif
      }
      else if (sg_bptab[BP_D].retEtat == EVENEMENT_RELACHE)
      {
            sendEvent(BT_D_RELACHE);
            #ifdef BDEBUG
            print_log(INFO, "BT_D_RELACHE\n");
            #endif
      }

      
      lireEvenement(BP_H);
      if (sg_bptab[BP_H].retEtat == EVENEMENT_PRESSE)
      {
            sendEvent(BT_H_PRESSE);
            #ifdef BDEBUG
            print_log(INFO, "BT_H_PRESSE\n");
            #endif
      }
      else if (sg_bptab[BP_H].retEtat == EVENEMENT_RELACHE)
      {
            sendEvent(BT_H_RELACHE);
            #ifdef BDEBUG
            print_log(INFO, "BT_H_RELACHE\n");
            #endif
      }


      lireEvenement(BP_B);
      if (sg_bptab[BP_B].retEtat == EVENEMENT_PRESSE)
      {
            sendEvent(BT_B_PRESSE);
            #ifdef BDEBUG
            print_log(INFO, "BT_B_PRESSE\n");
            #endif
      }
      else if (sg_bptab[BP_B].retEtat == EVENEMENT_RELACHE)
      {
            sendEvent(BT_B_RELACHE);
            #ifdef BDEBUG
            print_log(INFO, "BT_B_RELACHE\n");
            #endif
      }
}

static void lireEvenement(uint8_t indice)
{
	char state = sg_bptab[indice].state;
	char retEtat = sg_bptab[indice].retEtat;
	int icpt = sg_bptab[indice].icpt;
	uint16_t ret = 0;

	// Lecture d'un bouton
	ret = mcubind_virtualport_read(sg_bptab[indice].pin);
	ret = !ret;  // inversement de l'entrée car montage en pullup. Donc 5v en permanence sur l'entrée sans appuie bouton


	switch (state)
	{
	    case NON_PRESSE :
	    	if (ret < 1)
	    	{
	    		// Ici il ne s'est rien passé
	    		retEtat = AUCUN_EVENEMENT;
	    	}
	    	else
	    	{
	    		// Appuie détecté
	    		retEtat = AUCUN_EVENEMENT;  // on continu de retourner aucun event car on est qu'au début du filtre
	    		state = ENFONCE;
	    		icpt = 0;
	    	}
	      
		break;

	    case ENFONCE :
	    	if (ret >= 1) // On vérifie que le bouton soit toujours pressé
	    	{
	    		icpt ++;
	    	}
	    	else if (ret < 1)
	    	{
	    		state = NON_PRESSE; // Le bouton a été reléché, on retourne au début
	    	}
	    	
	    	if (icpt >= filtre)
	    	{
	    		// Le filtre a été passé, on considére le bouton pressé
	    		state = PRESSE;
	    		retEtat = EVENEMENT_PRESSE;
	    		icpt = 0;
	    	}
	    break;

	    case PRESSE :
	    {
	    	if (ret >= 1)
	    	{
	    		// Tant que le bouton est préssé
	    		retEtat = AUCUN_EVENEMENT;
	    	}
	    	else
	    	{
	    		// Evenement relaché capté
	    		retEtat = EVENEMENT_RELACHE;
	    		state = NON_PRESSE;
	    	}
	    }
	}

	// Sauvagarde du contexte
	sg_bptab[indice].state = state;
	sg_bptab[indice].retEtat = retEtat;
	sg_bptab[indice].icpt = icpt;
}


static void sendEvent(char event)
{
  if ( ! isFull_btfifo())
  {
    put_btfifo(event);
  }
}

/*
*  Gestion de la fifo d'événements boutons
*/
static void put_btfifo(char event)
{
  f_boutons.enqueue(event);
  if (mb_com_apbp < 255)
  {
  	mb_com_apbp++;
  }
  else
  {
  	mb_com_apbp = 0;
  }
}

Bool isEmpty_btfifo(void)
{
  return (Bool)f_boutons.isEmpty();
}

char get_btfifo(void)
{
  return f_boutons.dequeue();
}

Bool isFull_btfifo(void)
{
  return (Bool)f_boutons.isFull();
}

uint8_t boutons_getBPCounter(void)
{
	return mb_com_apbp;
}