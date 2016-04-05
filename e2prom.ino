/*
 * eeprom.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "EEPROM.h"
#include "e2prom.h"
/*
	TODO : incrementer l'id alarme à chaque ajout dans alarme_setAlarme(). Commencer à 1
	       dans delete_alarme() s'aider de l'indice pour retrouver l'al en fonction de la selection
	       envoyer l'indice a eeprom pour suppression
*/

void eeprom_ecrire_alarme(clock p_cl, uint8_t p_id)
{
	uint8_t addr = 0;
	uint8_t bok = 0;

	switch (p_id)
	{
		case EEPROM_AL1:
	    	addr = EEPROM_ADDR_AL1;
	    break;
	    case EEPROM_AL2:
	    	addr = EEPROM_ADDR_AL2;
	    break;
	    case EEPROM_AL3:
	    	addr = EEPROM_ADDR_AL3;
	    break;
	    case EEPROM_AL4:
	    	addr = EEPROM_ADDR_AL4;
	    break;
	    case EEPROM_AL5:
	    	addr = EEPROM_ADDR_AL5;
	    break;
	    default:
	    	bok = 1;
	}

	if (bok == 0)
	{
		// ecriture de l'heure
		EEPROM.write(addr, p_cl.heures);
		addr ++;
		EEPROM.write(addr, p_cl.minutes);
	}
}

clock eeprom_lire_alarme(uint8_t p_id)
{
	clock cret;
	uint8_t addr = 0;
	uint8_t bok = 0;

	switch (p_id)
	{
		case EEPROM_AL1:
	    	addr = EEPROM_ADDR_AL1;
	    break;
	    case EEPROM_AL2:
	    	addr = EEPROM_ADDR_AL2;
	    break;
	    case EEPROM_AL3:
	    	addr = EEPROM_ADDR_AL3;
	    break;
	    case EEPROM_AL4:
	    	addr = EEPROM_ADDR_AL4;
	    break;
	    case EEPROM_AL5:
	    	addr = EEPROM_ADDR_AL5;
	    break;
	    default:
	    	bok = 1;
	}

	if (bok == 0)
	{
		cret.heures = EEPROM.read(addr);
		addr ++;
		cret.minutes = EEPROM.read(addr);
	}

	return cret;
}