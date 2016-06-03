#include <Arduino.h>

int readSamples(){
  digitalWrite(5, HIGH);
  delayMicroseconds(200);
  int high = analogRead(0);
  digitalWrite(5, LOW);
  delayMicroseconds(200);
  int low = analogRead(0);
  int diff = low - high;
  Serial.println(diff,DEC);
  return diff;
}

