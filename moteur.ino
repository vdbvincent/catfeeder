/*
 * lcd.c
 *
 *  Created on: 31 juil. 2015
 *      Author: vincent
 */

#include "moteur.h"

const int IN_A = 12;
const int IN_B = 13;
const int IN_C = 4;
const int IN_D = 5;


void moteur_setup(void)
{
    // Init debug
    Serial.begin(9600);
 
    // Init des sorties
    Serial.println("Init des sorties moteur");
    pinMode(IN_A, OUTPUT);
    pinMode(IN_B, OUTPUT);
    pinMode(IN_C, OUTPUT);
    pinMode(IN_D, OUTPUT);
}

void sendMoteurEvent(char event)
{
  if (! isFull_lcdfifo())
  {
    put_lcdfifo(event);
  }
}

void moteur_every1ms(void)
{
  static int state = 0;
  static int cmp = 0;
  char event = NO_EVENT; 
  
  if ((event == NO_EVENT) && (! isEmpty_mtfifo()))
  {
    // Reception de la quantité
    event = get_mtfifo();
    Serial.println("Moteur : event recu");
  }
  
  
  switch (state)
  {
    
    // Attente de demande de nourriture.
    case 0:
      step0();
      if (event != NO_EVENT)
      {
        state = 1;
        cmp = 0;
        sendMoteurEvent(BEGIN);
      }
      
    break;
    
    
    case 1:
      step1();
      state = 2;
    break;
    
    case 2:
      step2();
      state = 3;
    break;
    
    case 3:
      step3();
      state = 4;
    break;
    
    case 4:
      step4();
      state = 5;
    break;
    
    case 5:
      step5();
      state = 6;
    break;
    
    case 6:
      step6();
      state = 7;
    break;
    
    case 7:
      step7();
      state = 8;
    break;
    
    case 8:
      step8();
      
      event = NO_EVENT;
      
      if (cmp < 512)
      {
        cmp ++;
        state = 1;
      }
      else
      {
        state = 0;
        sendMoteurEvent(END);
        Serial.println("Moteur : rotation finie");
      }
      
    break;
    
  }
}


//revolve clockwise
void clockwiserotate(void)
{
  step1();
  step2();
  step3();
  step4();
  step5();
  step6();
  step7();
  step8();
}

//revolve counterclockwise
void counterclockwiserotate(void)
{ 
  step1();
  step7();
  step6();
  step5();
  step4();
  step3();
  step2();
  step1();
}

void step0(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, LOW);
}

void step1(void)
{
  digitalWrite(IN_A, HIGH);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, LOW);
  delay(2);
}
void step2(void)
{
  digitalWrite(IN_A, HIGH);
  digitalWrite(IN_B, HIGH);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, LOW);
  delay(2);
}
void step3(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, HIGH);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, LOW);
  delay(2);
}
void step4(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, HIGH);
  digitalWrite(IN_C, HIGH);
  digitalWrite(IN_D, LOW);
  delay(2);
}
void step5(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, HIGH);
  digitalWrite(IN_D, LOW);
  delay(2);
}
void step6(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, HIGH);
  digitalWrite(IN_D, HIGH);
  delay(2);
}
void step7(void)
{
  digitalWrite(IN_A, LOW);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, HIGH);
  delay(2);
}
void step8(void)
{
  digitalWrite(IN_A, HIGH);
  digitalWrite(IN_B, LOW);
  digitalWrite(IN_C, LOW);
  digitalWrite(IN_D, HIGH);
  delay(2);
}
