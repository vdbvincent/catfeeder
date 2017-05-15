/*
 * e2prom.h
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
 
#ifndef _E2PROM_H_
#define _E2PROM_H_

#include "common.h"
#include "logs.h"
#include "clock.h"

// Addresse en EEPROM des alarmes en décimal
// 4 octets par alarmes
#define EEPROM_ADDR_AL1	0
#define EEPROM_ADDR_AL2	4
#define EEPROM_ADDR_AL3	8
#define EEPROM_ADDR_AL4	12
#define EEPROM_ADDR_AL5	16

#define EEPROM_AL1		0//1
#define EEPROM_AL2		1//2
#define EEPROM_AL3		2//3
#define EEPROM_AL4		3//4
#define EEPROM_AL5		4//5

clock eeprom_lire_alarme(uint8_t p_id);
void eeprom_ecrire_alarme(clock p_cl, uint8_t p_id);

#endif /* _E2PROM_H_ */