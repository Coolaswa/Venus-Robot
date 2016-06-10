#pragma once
#include <Arduino.h>

#define pingPin 9

int centimetersToTarget() {
  long duration, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  /*Serial.print("Ping result: ");
  Serial.println(duration / 58);*/
  delay(10);
  return (int)(duration / 58);
}
