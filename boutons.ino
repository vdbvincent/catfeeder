/*
 * boutons.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "boutons.h"

// Constantes
const int NBMAXVOIES = 4;
const int BP_G = 0;
const int BP_B = 1;
const int BP_D = 2;
const int BP_H = 3;

// Variables globales
int filtre = 0;
CTXT_FILTRE sgContexte[NBMAXVOIES];

void boutons_setup(int p_delayFiltre_ms)
{
    // Init des contextes
    sgContexte[BP_G].state = NON_PRESSE;
    sgContexte[BP_G].retEtat = AUCUN_EVENEMENT;
    sgContexte[BP_G].icpt = 0;
    sgContexte[BP_D].state = NON_PRESSE;
    sgContexte[BP_D].retEtat = AUCUN_EVENEMENT;
    sgContexte[BP_D].icpt = 0;
    sgContexte[BP_H].state = NON_PRESSE;
    sgContexte[BP_H].retEtat = AUCUN_EVENEMENT;
    sgContexte[BP_H].icpt = 0;
    sgContexte[BP_B].state = NON_PRESSE;
    sgContexte[BP_B].retEtat = AUCUN_EVENEMENT;
    sgContexte[BP_B].icpt = 0;
    // Init boutons
    pinMode(BP_G, INPUT_PULLUP);
    pinMode(BP_B, INPUT_PULLUP);
    pinMode(BP_D, INPUT_PULLUP);
    pinMode(BP_H, INPUT_PULLUP);

    
    // Init du filtrage anti-rebond
    filtre = p_delayFiltre_ms;
}

void sendEvent(char event)
{
  if ( ! isFull_btfifo())
  {
    put_btfifo(event);
  }
}


void boutons_every10ms(void)
{

   

    static int state = 0;
  	char etatBp = AUCUN_EVENEMENT;

    //printstr("test");

    switch(state)
    {
        case 0 : etatBp = lireBouton(BP_G); state = 1; break;
        case 1 : etatBp = lireBouton(BP_D); state = 2; break;
        case 2 : etatBp = lireBouton(BP_H); state = 3; break;
        case 3 : etatBp = lireBouton(BP_B); state = 0; break;

        if (etatBp == EVENEMENT_PRESSE)
        {
                printstr("Bouton PRESSE");
                //sendEvent(BT_H_PRESSE);
        }
        else if (etatBp == EVENEMENT_RELACHE)
        {
                printstr("Bouton RELACHE");
                //sendEvent(BT_H_RELACHE);
        }
    }

#if 0
        switch(state)
        {
            case 0:
                    // Lecture du bouton gauche
            	   etatBp = lireBoutonGauche();
                    if (etatBp == EVENEMENT_PRESSE)
                    {
                            //Serial.println("Bouton gauche PRESSE");
                            sendEvent(BT_G_PRESSE);
                    }
                    else if (etatBp == EVENEMENT_RELACHE)
                    {
                            //Serial.println("Bouton gauche RELACHE");
                            sendEvent(BT_G_RELACHE);
                    }
                    etatBp = AUCUN_EVENEMENT;
                    state = 1;
            break;
            
            case 1:
                    // Lecture du bouton droit
            	etatBp = lireBoutonDroit();
            	if (etatBp == EVENEMENT_PRESSE)
                    {
                            //Serial.println("Bouton droit PRESSE");
                            sendEvent(BT_D_PRESSE);
                    }
                    else if (etatBp == EVENEMENT_RELACHE)
                    {
                            //Serial.println("Bouton droit RELACHE");
                            sendEvent(BT_D_RELACHE);
                    }
                    etatBp = AUCUN_EVENEMENT;
                    state = 2;
            break;
            
            case 2:
                    // Lecture du bouton bas
            	etatBp = lireBoutonBas();
            	if (etatBp == EVENEMENT_PRESSE)
                    {
                            //Serial.println("Bouton bas PRESSE");
                            sendEvent(BT_B_PRESSE);
                    }
                    else if (etatBp == EVENEMENT_RELACHE)
                    {
                            //Serial.println("Bouton bas RELACHE");
                            sendEvent(BT_B_RELACHE);
                    }
                    etatBp = AUCUN_EVENEMENT;
                    state = 3;
            break;
            
            case 3:
                    // Lecture du bouton haut
            	etatBp = lireBoutonHaut();
            	if (etatBp == EVENEMENT_PRESSE)
                    {
                            //Serial.println("Bouton haut PRESSE");
                            sendEvent(BT_H_PRESSE);
                    }
                    else if (etatBp == EVENEMENT_RELACHE)
                    {
                            //Serial.println("Bouton haut RELACHE");
                            sendEvent(BT_H_RELACHE);
                    }
                    etatBp = AUCUN_EVENEMENT;
                    state = 0;
            break;
        }
#endif
}


char lireBouton(uint8_t voie)
{
    char state = sgContexte[voie].state;
    char retEtat = sgContexte[voie].retEtat;
    int icpt = sgContexte[voie].icpt;
    int ret = 0;
        
    ret = analogRead(voie);
    //ret = mcubind_virtualport_read(voie);

    switch(state)
    {
        case NON_PRESSE:
            if (ret < 1)
            {
                retEtat = AUCUN_EVENEMENT;
                state = ENFONCE;
                icpt = 0;
                //Serial.println("NON_PRESSE -> ENFONCE");
            }
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
        break;

        case ENFONCE:
            if (ret < 1)
            {
                icpt ++;
            }
            else if (ret >= 1)
            {
                state = NON_PRESSE;
                //Serial.println("ENFONCE -> NON_PRESSE");
            }
            if (icpt > filtre)
            {
                state = PRESSE;
                //Serial.println("ENFONCE -> PRESSE");
                retEtat = EVENEMENT_PRESSE;
            }
        break;

        case PRESSE:
            if (ret >= 1)
            {
                state = NON_PRESSE;
                //Serial.println("PRESSE -> NON_PRESSE");
                retEtat = EVENEMENT_RELACHE;
            }
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
        break;
    }

    sgContexte[voie].state = state;
    sgContexte[voie].retEtat = retEtat;
    sgContexte[voie].icpt = icpt;
    return retEtat;
}
#if 0
char lireBoutonGauche(void)
{
	static char state = NON_PRESSE;
    static char retEtat = AUCUN_EVENEMENT;
	int ret = 0;
	static int icpt = 0;
        
	// ret = analogRead(BP_G);
    ret = mcubind_virtualport_read(BP_G);

	switch(state)
	{
		case NON_PRESSE:
			if (ret < 1)
			{
                retEtat = AUCUN_EVENEMENT;
				state = ENFONCE;
				icpt = 0;
                //Serial.println("NON_PRESSE -> ENFONCE");
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;

		case ENFONCE:
			if (ret < 1)
			{
				icpt ++;
			}
			else if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("ENFONCE -> NON_PRESSE");
			}
			if (icpt > filtre)
			{
				state = PRESSE;
                //Serial.println("ENFONCE -> PRESSE");
                retEtat = EVENEMENT_PRESSE;
			}
		break;

		case PRESSE:
			if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("PRESSE -> NON_PRESSE");
                retEtat = EVENEMENT_RELACHE;
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;
	}

	return retEtat;
}


char lireBoutonDroit(void)
{
	static char state = NON_PRESSE;
    static char retEtat = AUCUN_EVENEMENT;
	static int ret = 0;
	static int icpt = 0;
        
	// ret = analogRead(BP_D);
    ret = mcubind_virtualport_read(BP_D);

	switch(state)
	{
		case NON_PRESSE:
			if (ret < 1)
			{
                retEtat = AUCUN_EVENEMENT;
				state = ENFONCE;
				icpt = 0;
                //Serial.println("NON_PRESSE -> ENFONCE");
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;

		case ENFONCE:
			if (ret < 1)
			{
				icpt ++;
			}
			else if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("ENFONCE -> NON_PRESSE");
			}
			if (icpt > filtre)
			{
				state = PRESSE;
                //Serial.println("ENFONCE -> PRESSE");
                retEtat = EVENEMENT_PRESSE;
			}
		break;

		case PRESSE:
			if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("PRESSE -> NON_PRESSE");
                retEtat = EVENEMENT_RELACHE;
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;
	}

	return retEtat;
}

char lireBoutonBas(void)
{
	static char state = NON_PRESSE;
    static char retEtat = AUCUN_EVENEMENT;
	static int ret = 0;
	static int icpt = 0;
        
	//ret = analogRead(BP_B);
    ret = mcubind_virtualport_read(BP_B);

	switch(state)
	{
		case NON_PRESSE:
			if (ret < 1)
			{
                retEtat = AUCUN_EVENEMENT;
				state = ENFONCE;
				icpt = 0;
                //Serial.println("NON_PRESSE -> ENFONCE");
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;

		case ENFONCE:
			if (ret < 1)
			{
				icpt ++;
			}
			else if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("ENFONCE -> NON_PRESSE");
			}
			if (icpt > filtre)
			{
				state = PRESSE;
                //Serial.println("ENFONCE -> PRESSE");
                retEtat = EVENEMENT_PRESSE;
			}
		break;

		case PRESSE:
			if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("PRESSE -> NON_PRESSE");
                retEtat = EVENEMENT_RELACHE;
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;
	}

	return retEtat;
}

char lireBoutonHaut(void)
{
	static char state = NON_PRESSE;
    static char retEtat = AUCUN_EVENEMENT;
	static int ret = 0;
	static int icpt = 0;
        
	//ret = analogRead(BP_H);
    ret = mcubind_virtualport_read(BP_H);

	switch(state)
	{
		case NON_PRESSE:
			if (ret < 1)
			{
                retEtat = AUCUN_EVENEMENT;
				state = ENFONCE;
				icpt = 0;
                //Serial.println("NON_PRESSE -> ENFONCE");
			}
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;

		case ENFONCE:
			if (ret < 1)
			{
				icpt ++;
			}
			else if (ret >= 1)
			{
				state = NON_PRESSE;
                //Serial.println("ENFONCE -> NON_PRESSE");
			}
			if (icpt > filtre)
			{
				state = PRESSE;
                //Serial.println("ENFONCE -> PRESSE");
                retEtat = EVENEMENT_PRESSE;
			}
		break;

		case PRESSE:
            if (ret >= 1)
            {
                state = NON_PRESSE;
                //Serial.println("PRESSE -> NON_PRESSE");
                retEtat = EVENEMENT_RELACHE;
            }
            else
            {
                retEtat = AUCUN_EVENEMENT;
            }
		break;
	}

	return retEtat;
}
#endif