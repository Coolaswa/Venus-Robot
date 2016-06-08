#pragma once
#include <Arduino.h>

int readSamples(){
  digitalWrite(5, HIGH);
  delayMicroseconds(200);
  int high = analogRead(1);
  digitalWrite(5, LOW);
  delayMicroseconds(200);
  int low = analogRead(1);
  int diff = low - high;
  Serial.println(diff,DEC);
  return diff;
}

