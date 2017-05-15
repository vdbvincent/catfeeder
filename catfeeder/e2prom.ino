/*
 * eeprom.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 *
 */
#include "EEPROM.h"
#include "e2prom.h"

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
		EEPROM.write(addr+1, p_cl.minutes);
	}
}

clock eeprom_lire_alarme(uint8_t p_id)
{
	clock cret;
	uint8_t addr = 0;
	uint8_t bok = 0;

	cret.heures = 0;
	cret.minutes = 0;

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
		cret.minutes = EEPROM.read(addr+1);
	}

	return cret;
}