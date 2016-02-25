/*
 * boutons.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *  http://modelleisenbahn.triskell.org/spip.php?article59
 *
 *  // TODO : Attention, plusieurs consommateurs de ressource BT_FIFO ! -> veillez a
 *  //        ce qu'ils ne consomment pas en meme temps (archi logicielle) : manager et menu
 *  //        ou mettre en pace un mécanisme de concurrence d'accès (lourd)
 */
#include "boutons.h"

// Fréquence d'appel de la méthode every100ms()
#define FREQ_SEQ 100
/*
une fonction qui retourne en permanence le numéro du poussoir qui a été détecté
une fonction filtre anti rebond qui cherche a stabiliser le num poussoir sur une durée
une fonction qui detecte un evenement presse ou relache
*/

/* Variables globales */
// Delai de filtrage du filtre anti-rebonds
int filtre = 0;
// Port analogique à lire
const uint8_t ci_ADC0 = MCUBIND_VIRTUALPORT_ADC00;
const int ci_NBVALADC = 256;

void boutons_setup(int p_delayFiltre_ms)
{
    // Init du filtrage anti-rebond
    filtre = p_delayFiltre_ms / FREQ_SEQ;
}

static void sendEvent(char event)
{
  if ( ! isFull_btfifo())
  {
    put_btfifo(event);
  }
}

void boutons_every100ms(void)
{
	int bouton = BOUTOU_NONE;  // 4 = valeur par défaut, aucun bouton préssé
	char etatBp = AUCUN_EVENEMENT;
	etatBp = lireEvenement(&bouton);
	
	switch (bouton)
	{
		case BOUTON_HAUT:  // Bouton 1
			if (etatBp == EVENEMENT_PRESSE)
			{
				//print_log(DEBUG, "Bouton haut PRESSE\n");
				sendEvent(BT_H_PRESSE);
			}
			else if (etatBp == EVENEMENT_RELACHE)
			{
				//print_log(DEBUG, "Bouton haut RELACHE\n");
				sendEvent(BT_H_RELACHE);
			}
		break;
		
		case BOUTON_BAS:  // Bouton 2
			if (etatBp == EVENEMENT_PRESSE)
			{
				//print_log(DEBUG, "Bouton bas PRESSE\n");
				sendEvent(BT_B_PRESSE);
			}
			else if (etatBp == EVENEMENT_RELACHE)
			{
				//print_log(DEBUG, "Bouton bas RELACHE\n");
				sendEvent(BT_B_RELACHE);
			}
		break;
		
		case BOUTON_GAUCHE:  // Bouton 3
			if (etatBp == EVENEMENT_PRESSE)
			{
				//print_log(DEBUG, "Bouton gauche PRESSE\n");
				sendEvent(BT_G_PRESSE);
			}
			else if (etatBp == EVENEMENT_RELACHE)
			{
				//print_log(DEBUG, "Bouton gauche RELACHE\n");
				sendEvent(BT_G_RELACHE);
			}
		break;
		
		case BOUTON_DROIT:  // Bouton 4
			if (etatBp == EVENEMENT_PRESSE)
			{
				//print_log(DEBUG, "Bouton droit PRESSE\n");
				sendEvent(BT_D_PRESSE);
			}
			else if (etatBp == EVENEMENT_RELACHE)
			{
				//print_log(DEBUG, "Bouton droit RELACHE\n");
				sendEvent(BT_D_RELACHE);
			}
		break;
		
		default: // Aucun appui
			if (etatBp != AUCUN_EVENEMENT)
			{
				//print_log(DEBUG, "Erreur : evenement apparu sur aucun bouton.\n");
			}
		break;
	}
}

/*
*	Filtre anti rebond
*/
static char lireEvenement(int * p_bouton)
{
    static char state = NON_PRESSE;  // etat de la FSM
    static char retEtat = AUCUN_EVENEMENT;  // event de retour
    uint8_t bouton = 4;  // (0->b0 .. 3->b4 4->rien)
	static uint8_t oldbouton = 4; // mise en mémoire de la lecture précédente
    static int icpt = 0;  // compteur pour le filtrage
	char buffer[64];  // buffer pour former les logs
	 
	// Lecture de l'entrée analogique
	bouton = lireBouton();

	// Gestion du filtre
    switch(state)
    {
        case NON_PRESSE:
			// Ici on attend un appui quelconque
            if (bouton < 4)
            {
                retEtat = AUCUN_EVENEMENT;
                state = ENFONCE;
                icpt = 0;
                //print_log(DEBUG, "NON_PRESSE -> ENFONCE\n");
            }
            else // ret == 4 signifie valeur par défaut, aucun bouton préssé
            {
                retEtat = AUCUN_EVENEMENT;
            }
        break;

        case ENFONCE:
            //if (ret < 1)
			if (oldbouton == bouton)
            {
                icpt ++;
            }
            //else if (ret >= 1)
			else
            {
                state = NON_PRESSE;
                //print_log(DEBUG, "ENFONCE -> NON_PRESSE\n");
            }
            if (icpt > filtre)
            {
                state = PRESSE;
                //print_log(DEBUG, "ENFONCE -> PRESSE\n");
                retEtat = EVENEMENT_PRESSE;
            }
        break;

        case PRESSE:
            //if (ret >= 1)
			if (oldbouton != bouton)
            {
                state = NON_PRESSE;
                //print_log(DEBUG, "PRESSE -> NON_PRESSE\n");
                retEtat = EVENEMENT_RELACHE;
            }
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
        break;
    }

	
	// Sauvegarde de la valeur du bouton dans le ptr de retour
	*p_bouton = oldbouton;

	// Sauvegarde de la valeur lue avant
	oldbouton = bouton;
	 
	// Retourner l'événement
    return retEtat;
}

/*
*	Lecture de l'entrée analogique et détection du numéro de poussoir
*/
static uint16_t lireBouton(void)
{
	static uint16_t cmp = 0;
	uint16_t ret = 0;
	char buffer[64];
    
	// Lecture de l'entrée analogique
	ret = mcubind_virtualport_read(ci_ADC0);

	// Détection du poussoir
	// permet de déduire le numéro en valeur entiere du bouton préssé (0->b0 .. 3->b4)
	ret = (ret + (ci_NBVALADC/2)) / ci_NBVALADC; // valeur lue (0-1023?) + la moitié de l'intervalle divisées par le nombre de valeur dans un intervalle

	cmp ++;
	return ret;
}

/*
*  Gestion de la fifo d'événements boutons
*/
static void put_btfifo(char event)
{
  f_boutons.enqueue(event);
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
