#pragma once
#include <Arduino.h>
#include "Motor.h"

bool rockForward = 0;
bool rockHug = 0;

int rockAngleEncoder = 0;

void alignToRock() { // changes reference from map to sample
  rock = 0;
  while (rockForward == 0) { // searches sample and turns until it is right in front of it
    rightWheel.write(180);
    leftWheel.write(180);
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
  //Serial.println(analogRead(4));
  if (analogRead(4) > 200) {
    rockHug = 1;
  }
}

void grabRock() { // grabs sample
  head.write(60); // makes space for the sample
  gripperOpen();
  delay(100);
  rightWheel.write(0);
  leftWheel.write(180);
  while (rockHug == 0 || rightEncoder < 16) {
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
  bool lr = 0;
  if ((16 - rockAngleEncoder) < rockAngleEncoder) {
    rightEncoder = 16 - rockAngleEncoder;
    lr = 1;
  }
  else {
    rightEncoder = rockAngleEncoder;
    lr = 0;
  }

  reverse = 1;
  while (rightEncoder > 0) { // turns back into original angle
    if (lr = 0) {
      rightWheel.write(180);
      leftWheel.write(180);
    }
    if (lr = 1) {
      rightWheel.write(0);
      leftWheel.write(0);
    }
  }
  rightEncoder = backToMapEncoder;
  backToMapEncoder = 0;
  while (rightEncoder > 0) {
    rightWheel.write(180);
    leftWheel.write(0);
  }
  rockAngleEncoder = 0;
  reverse = 0;
  stopRobot();
  rock = 0;
  return;
}

