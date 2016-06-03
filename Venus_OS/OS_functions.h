#pragma once
#include <Arduino.h>
#include "Motor.h"
#include "Sample_detection.h"

int failCount = 0;
int forwardUS = 200;
int xDist = 0;
int yDist = 0;

void gapCalibrate() { // determines the value of safe ground
  rightFloor = 0;
  leftFloor = 0;
  int i;
  int right = 0;
  int left = 0;
  for (i = 0; i < 10; i++) { // measures values from ground
  right = analogRead(1);
  left = analogRead(2);
  //Serial.println(rightFloor);
  //Serial.println(leftFloor);
  rightFloor = rightFloor + right;
  leftFloor = leftFloor + left;
  }
  rightFloor = rightFloor/10;
  leftFloor = leftFloor/10;
  return;
}

void calcUSRelLoc() { // determines x and y coordinates of US detected objects relative to the robot itself
  int dist;
  head.write(USPos);
  dist = centimetersToTarget();
  xDist = dist * cos(USPos);
  yDist = dist * sin(USPos);
  USPos = 0;
  return;
}

void roam() {
  //scan(); // robot sweeps head
  if ((gap || forwardUS <= 40) && failCount == 0) {
    turnRight(); // robot turns right
    failCount++;
  }
  else if ((gap || forwardUS <= 40) && failCount == 1) {
    turnLeft(); // robot turns left
    failCount++;
  }
  else {
    driveForward(); // robot moves forward
  }
  failCount = 0;
  if (closeUS == 1) {
    delay(250);
    scan();
    /*Serial.println(xDist);
    Serial.println(yDist);*/
  }
  return;
}
