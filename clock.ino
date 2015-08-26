/*
 * clock.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */
#include "clock.h"


// Définition des variables globales
char heures;
char minutes;
char secondes;
int millisecondes;


void clock_setup(void)
{
  //Serial.begin(9600);
  
  heures = 0;
  minutes = 0;
  secondes = 1;
  millisecondes = 0;
}

void clock_every10ms(void)
{
  // incrément des ms
  if (millisecondes >= 100)
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
  
  millisecondes ++;
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
  
  char message[16];
  sprintf(message, "%d:%d:%d", p_myclock.heures, p_myclock.minutes, p_myclock.secondes);
  Serial.println(message);
}