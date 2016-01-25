/*
 * clock.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "clock.h"

// Fréquence d'appel de la méthode every100ms()
#define FREQ_SEQ 100

// Définition des variables globales
char heures = 0;
char minutes = 0;
char secondes = 0;
int millisecondes = 0;
uint8_t mb_clockInit = 0;  // Permet de garder en mémoire la mise à l"heure

void clock_setup(void)
{
  clock_reset();
}

void clock_every100ms(void)
{

  // incrément des ms
  if (millisecondes >= 1000)
  {
    millisecondes = 0;
    secondes ++;
  }
  
  //  incrément des secondes
  if (secondes >= 60)
  {
    secondes = 0;
    minutes ++;
  }
  
  // incrément des minutes
  if (minutes >= 60)
  {
    minutes = 0;
    heures ++;
  }
  
  if (heures >= 24)
  {
    heures = 0;
    minutes = 0;
    secondes = 0;
    millisecondes = 0;
  }
  
  millisecondes += FREQ_SEQ;
}


// GETTER  //  SETTER
clock clock_getClock(void)
{
  clock myclock;
  myclock.heures = heures;
  myclock.minutes = minutes;
  myclock.secondes = secondes;
  return myclock;
}

void clock_setClock(clock p_myclock)
{
  heures = p_myclock.heures;
  minutes = p_myclock.minutes;
  secondes = p_myclock.secondes;
  mb_clockInit = 1; // Permet de garder en mémoire la mise à l"heure
  
  char message[40];
  sprintf(message, "clock : Mise a l'heure : %02d:%02d:%02d\n", heures, minutes, secondes);
  print_log(DEBUG, message);
}

void clock_reset(void)
{
  heures = 0;
  minutes = 0;
  secondes = 1;
  millisecondes = 0;
}

uint8_t clock_isInit(void)
{
  return mb_clockInit;
}