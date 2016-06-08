#pragma once
#include <Servo.h>
#include "OS_functions.h"
#include "Mapping.h"
#include "Interrupts.h"
#include "Motor.h"
#include "Motor.cpp"
#include "Samples.h"
#include "Ultrasound.h"
#include "Wireless.h"

byte Xsize = 10, Ysize = 10;

void setup() {
  theMap = createTheMatrix(Xsize,Ysize);
  Serial.begin(115200);
  initializeInterrupts();
  head.attach(10, 1400, 1600);
  rightWheel.attach(13, 1250, 1750);
  leftWheel.attach(12, 1250, 1750);
  gripper.attach(11); // attaches servos to pins
  gripperGrab(); // opens gripper
  gapCalibrate();
  head.write(0); // moves head to right
  rightWheel.write(90); // robot stops
  leftWheel.write(90);
  delay(1200); // waits for head to be in position
}

void loop() {
  if (rock == 0) {
    /*if (gap == 0) {
      gapCalibrate();
    }*/
  head.write(180); // turn head right
  roam(); // robot explores the area

  head.write(90); // robot centers head
  delay(500);
  forwardUS = centimetersToTarget();
  Serial.println(forwardUS);

  head.write(0); // turn head left
  delay(1000);
  Serial.print("X: ");
  Serial.println(Xposition, DEC);
  Serial.print("Y: ");
  Serial.println(Yposition, DEC);
  Serial.print("Direction: ");
  Serial.println(currDirection, DEC);
  displayMatrix(theMap, Xsize, Ysize);
  else if (rock) {
    alignToRock();
    grabRock();
    alignToMap();
    // find lab and drop sample
  }
}
