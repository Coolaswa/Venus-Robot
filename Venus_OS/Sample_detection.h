#pragma once
#include <Arduino.h>
#include "Motor.h"

bool rockForward = 0;
bool rockInGripper = 0;

int rockAngleEncoder = 0;

void alignToRock() { // changes reference from map to sample
  rock = 0;
  while (rockForward == 0) { // searches sample and turns until it is right in front of it
    rightWheel.write(180);
    leftWheel.write(180);
    if (checkRock) {
      rockForward = 1;
    }
  }
  rockAngleEncoder = rightEncoder;
  rightEncoder = 0;
  leftEncoder = 0;
  rightWheel.write(90); // stops robot
  leftWheel.write(90);
  return;
}

void grabRock() { // grabs sample
  head.write(60); // makes space for the sample
  gripperOpen();
  delay(100);
  rightWheel.write(180);
  leftWheel.write(0);
  delay(800);
  rightWheel.write(90);
  leftWheel.write(90);
  gripperGrab();
  delay(200);
  head.write(90);
  reverse = 1;
  while (rightEncoder > 0) { // moves back into the position it initially came from
    rightWheel.write(0);
    leftWheel.write(180);
  }
  stopRobot();
  reverse = 0;
  rockInGripper = 1;
  return;
}

void alignToMap() { // changes reference from sample to map
  rightEncoder = rockAngleEncoder;
  reverse = 1;
  while (rightEncoder > 0) { // turns back into original angle
  rightWheel.write(0);
  leftWheel.write(0);
  }
  reverse = 0;
  stopRobot();
  return;
}


