#pragma once
#include <Arduino.h>
#include "Motor.h"

bool rockForward = 0;
bool rockInGripper = 0;
bool rockHug = 0;

int rockAngleEncoder = 0;

void alignToRock() { // changes reference from map to sample
  while (rockForward == 0) { // searches sample and turns until it is right in front of it
    rightWheel.write(180);
    leftWheel.write(180);
    rock = 0;
    checkRock();
    if (rock) {
      rockForward = 1;
    }
  }
  rockAngleEncoder = rightEncoder;
  rightEncoder = 0;
  leftEncoder = 0;
  rockForward = 0;
  rightWheel.write(90); // stops robot
  leftWheel.write(90);
  return;
}

void atRock() {
  pinMode(4, INPUT);
  if (analogRead(4) > 650) {
    rockHug = 1;
  }
}

void grabRock() { // grabs sample
  head.write(60); // makes space for the sample
  gripperOpen();
  delay(100);
  while(rockHug == 0 || rightEncoder < 16) {
    rightWheel.write(0);
    leftWheel.write(180);
    atRock();
  }
  rightWheel.write(90);
  leftWheel.write(90);
  gripperGrab();
  delay(200);
  head.write(90);
  reverse = 1;
  while (rightEncoder > 0) { // moves back into the position it initially came from
    rightWheel.write(180);
    leftWheel.write(0);
  }
  stopRobot();
  reverse = 0;
  rockHug = 0;
  rockInGripper = 1;
  return;
}

void alignToMap() { // changes reference from sample to map
  rightEncoder = rockAngleEncoder;
  reverse = 1;
  while (rightEncoder > 0) { // turns back into original angle
  rightWheel.write(180);
  leftWheel.write(180);
  }
  reverse = 0;
  stopRobot();
  rock = 0;
  return;
}


