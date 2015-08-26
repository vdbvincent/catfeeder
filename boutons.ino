/*
 * boutons.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "boutons.h"


const int BP_G = 0;
const int BP_B = 1;
const int BP_D = 2;
const int BP_H = 3;

int filtre = 0;

void boutons_setup(int p_delayFiltre_ms)
{
    // Init debug
    Serial.begin(9600);
    Serial.println("Init des boutons");
    
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

        switch(state)
        {
                case 0:
                        // Lecture du bouton gauche
                	etatBp = lireBoutonGauche();
                        if (etatBp == EVENEMENT_PRESSE)
                        {
                                Serial.println("Bouton gauche PRESSE");
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
                                Serial.println("Bouton droit PRESSE");
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
                                Serial.println("Bouton bas PRESSE");
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
                                Serial.println("Bouton haut PRESSE");
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
}


char lireBoutonGauche(void)
{
	static char state = NON_PRESSE;
        static char retEtat = AUCUN_EVENEMENT;
	static int ret = 0;
	static int icpt = 0;
        
	ret = analogRead(BP_G);

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
        
	ret = analogRead(BP_D);

	switch(state)
	{
		case NON_PRESSE:
			if (ret < 1)
			{
                                retEtat = AUCUN_EVENEMENT;
				state = ENFONCE;
				icpt = 0;
                                Serial.println("NON_PRESSE -> ENFONCE");
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
                                Serial.println("ENFONCE -> NON_PRESSE");
			}
			if (icpt > filtre)
			{
				state = PRESSE;
                                Serial.println("ENFONCE -> PRESSE");
                                retEtat = EVENEMENT_PRESSE;
			}
		break;

		case PRESSE:
			if (ret >= 1)
			{
				state = NON_PRESSE;
                                Serial.println("PRESSE -> NON_PRESSE");
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
        
	ret = analogRead(BP_B);

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
        
	ret = analogRead(BP_H);

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