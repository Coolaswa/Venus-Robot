#pragma once
#include <Arduino.h>
#include <Servo.h>

byte lastXPos, lastYPos;

//This function is called between loop() when there is data in the serial receive buffer
//The serial buffer is 64 bytes in size, big enough to hold any data we might want to send except for the map

//It waits for the first two bytes, the identifier, which tells the receiver what data is about to be sent through
/*
 * Data values:
 * 1 - Let the other robot know of the position of the other robot
 * 2 - 
 * 3 - 
 * 4 - 
 */
Servo eenServo;
 
void serialEvent() {
  cli();
  eenServo.attach(10);
  byte receivedData[5];
  byte i = 0;
  while (Serial.available()) {
    receivedData[i] = Serial.read();
    i++;
  }
  for(int j = 0; j+1 < i; j++){
    byte identifier = receivedData[j+1];
    if(receivedData[j] == 255 && identifier >= 128 && identifier <= 135){
      if(identifier == 128){ //location transfer
        eenServo.write(0);
        lastXPos = receivedData[j+2];
        lastYPos = receivedData[j+3];
      } else if(identifier == 129){ //write to map
        eenServo.write(180);
      }
    } 
  }
  sei();
  return;
 }

void transferLocation(byte Xpos, byte Ypos) {
 cli();
 Serial.write((byte)255);
 Serial.write((byte)128);
 Serial.write(Xpos);
 Serial.write(Ypos);
 sei();
 return;
}

void writeToMap(byte Xpos, byte Ypos, matrixValue_t value) {
  cli();
  Serial.write((byte)255);
  Serial.write((byte)129);
  Serial.write((byte)Xpos);
  Serial.write((byte)Ypos);
  Serial.write((byte)value);
  sei();
  return;
}
