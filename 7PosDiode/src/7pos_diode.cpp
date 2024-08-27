/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>

const int led2 = 2;
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;
const int led6 = 6;
const int led7 = 7;
const int led8 = 8;

void setup()
{
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
}

void loop()
{
  delay(250);
  digitalWrite(2, LOW); digitalWrite(3, HIGH); digitalWrite(4, HIGH); digitalWrite(5, LOW);   //say H
  digitalWrite(6, HIGH); digitalWrite(7, HIGH); digitalWrite(8, HIGH); digitalWrite(9, HIGH); //
  delay(250); 
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW);   //don`t say anything
  digitalWrite(6, LOW); digitalWrite(7, LOW); digitalWrite(8, LOW); digitalWrite(9, LOW);   //
  delay(250);  
  digitalWrite(2, HIGH); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, HIGH);   //say E
  digitalWrite(6, HIGH); digitalWrite(7, HIGH); digitalWrite(8, HIGH); digitalWrite(9, HIGH); //
  delay(250);
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW);   //don`t say anything
  digitalWrite(6, LOW); digitalWrite(7, LOW); digitalWrite(8, LOW); digitalWrite(9, LOW);   //
  delay(250);  
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, HIGH);   //say L
  digitalWrite(6, HIGH); digitalWrite(7, HIGH); digitalWrite(8, LOW); digitalWrite(9, HIGH); //
  delay(250);
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW);   //don`t say anything
  digitalWrite(6, LOW); digitalWrite(7, LOW); digitalWrite(8, LOW); digitalWrite(9, LOW);   //
  delay(250); 
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, HIGH);   //say L
  digitalWrite(6, HIGH); digitalWrite(7, HIGH); digitalWrite(8, LOW); digitalWrite(9, HIGH); //
  delay(250);
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW);   //don`t say anything
  digitalWrite(6, LOW); digitalWrite(7, LOW); digitalWrite(8, LOW); digitalWrite(9, LOW);   //
  delay(250);  
  digitalWrite(2, HIGH); digitalWrite(3, HIGH); digitalWrite(4, HIGH); digitalWrite(5, HIGH);   //say O
  digitalWrite(6, HIGH); digitalWrite(7, HIGH); digitalWrite(8, LOW); digitalWrite(9, HIGH);    //
  delay(250);
  digitalWrite(2, LOW); digitalWrite(3, LOW); digitalWrite(4, LOW); digitalWrite(5, LOW);   //don`t say anything
  digitalWrite(6, LOW); digitalWrite(7, LOW); digitalWrite(8, LOW); digitalWrite(9, LOW); //
  delay(1000);  
}
