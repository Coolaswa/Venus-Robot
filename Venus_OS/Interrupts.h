#pragma once
#include <Arduino.h> 
#include "Motor.h"

volatile bool encoderSevenOn, twoBlack = 0, threeBlack = 0;
volatile bool gapRight = 0;
volatile bool gapLeft = 0;
volatile bool reverse = 0;

volatile int rightBlackCounter = 0;
volatile int rightWhiteCounter = 0;
volatile int leftBlackCounter = 0;
volatile int leftWhiteCounter = 0;
volatile int rightEncoder = 0;
volatile int leftEncoder = 0;

void initializeInterrupts(){
  pinMode(7,INPUT);
  pinMode(8,INPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  cli();
  PCICR = 5; //Enable interrupts for PortB and PortD
  //PCMSK2 = 0b10001100;
  PCMSK2 = 128;
  PCMSK0 = 1;
  sei();
  if(digitalRead(7)){
    encoderSevenOn = true;
  }
}

ISR(PCINT0_vect){ //Pin 8
    if (reverse == 0) {
      rightEncoder++; // adds one to the right encoder value
    }
    else {
      rightEncoder--;
    }
}

ISR(PCINT2_vect){ //Pin 2,3,7
  if(encoderSevenOn && !digitalRead(7)){
    encoderSevenOn = false;
    leftEncoder++; // adds one to the left encoder value
  } else if(!encoderSevenOn && digitalRead(7)){
    encoderSevenOn = true;
    leftEncoder++; // adds one to the left encoder value
  } else if(!twoBlack && digitalRead(2)){
    twoBlack = true;
    gapRight = 1;
    //Serial.write('b');
  } else if(twoBlack && !digitalRead(2)){
    twoBlack = false;
    gapRight = 0;
    //Serial.write('b');
  } else if(!threeBlack && digitalRead(3)){
    threeBlack = true;
    gapLeft = 1;
    //Serial.write('a');
  } else if(threeBlack && !digitalRead(3)){
    threeBlack = false;
    gapLeft = 0;
    //Serial.write('a');
  }
}

