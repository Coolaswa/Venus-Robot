#pragma once
#include <Arduino.h>
#include "Ultrasound.h"
#include "Interrupts.h"
#include "Samples.h"
#define rightEncoderPin 8
#define leftEncoderPin 7

int pos = 0;
int ultraSoundDist = 0;
int USPos = 0;
int rightFloor = 0;
int leftFloor = 0;

bool gap = 0;
bool closeUS = 0;
bool rock = 0;

Servo rightWheel;
Servo leftWheel;
Servo head;
Servo gripper;

void stopRobot() {
  rightWheel.write(90); // robot stops
  leftWheel.write(90);
  rightEncoder = 0;
  leftEncoder = 0;
  return;
}

void gripperOpen() {
  gripper.write(90); // grippers open
  return;
}

void gripperGrab() {
  gripper.write(1); // grippers close
  return;
}

void checkGap() {
  if (gapRight == 1 || gapLeft == 1) {
    gap = 1;
    gapRight = 0;
    gapLeft = 0;
  }
  return;
}

void checkRock() {
  if (readSamples() >= 60) {
    rock = 1;
  }
  Serial.write(readSamples());
  return;
}

void checkUS() {
  if (ultraSoundDist <= 30) {
    closeUS = 1;
  }
  return;
}

void driveForward() {
  while (rightEncoder < 16) { // robot moves forward
    rightWheel.write(180);
    leftWheel.write(0);

    checkRock();

    int i;
    for (i = 0; i < 10; i++) { // robot looks at the ground to see if there is a gap or not
      if (analogRead(1) > (rightFloor+35)) {
        rightBlackCounter++;
      }
      else {
        rightWhiteCounter++;
      }
      if (analogRead(2) > (leftFloor+35)) {
        leftBlackCounter++;
      }
      else {
        leftWhiteCounter++;
      }
    }
    if (rightBlackCounter > rightWhiteCounter) {
      gapRight = 1;
    }
    if (leftBlackCounter > leftWhiteCounter) {
      gapLeft = 1;
    }

    //Serial.println(rightBlackCounter);
    //Serial.println(rightWhiteCounter);

    rightBlackCounter = 0;
    rightWhiteCounter = 0;
    leftBlackCounter = 0;
    leftWhiteCounter = 0;

    checkGap();
    if (gap) {
      break; // when a gap is encountered the robot immediately stops
    }
    ultraSoundDist = centimetersToTarget(); // robot scans around
    checkUS(); // checks for close objects with US data
  }
  if (gap) {
    rightWheel.write(90);
    leftWheel.write(90);
  }
  if (gap == 0) {
    stopRobot(); // robot stops
  }
  return;
}

void turnRight() {
  reverse = 1;
  while (rightEncoder > 0) {
    rightWheel.write(0);
    leftWheel.write(180);
  }
  gap = 0;
  reverse = 0;
  stopRobot(); // robot stops
  while (rightEncoder <= 6) { // robot turns right
    rightWheel.write(0);
    leftWheel.write(0);
  }
  stopRobot(); // robot stops
  return;
}

void turnLeft() {
  reverse = 1;
  while (rightEncoder > 0) {
    rightWheel.write(0);
    leftWheel.write(180);
  }
  gap = 0;
  reverse = 0;
  stopRobot(); // robot stops
  while (rightEncoder <= 6) { // robot turns left
    rightWheel.write(180);
    leftWheel.write(180);
  }
  stopRobot(); // robot stops
  return;
}

void scan() {
  for (pos = 0; pos < 180; pos += 1) { // robot sweeps head
    head.write(pos);
    ultraSoundDist = centimetersToTarget(); // robot measures distance
    delay(15); // robot waits for the equipment to do its work
    if (ultraSoundDist <= 100) {
      USPos = pos; // saves position with low value, priority to targets to the front
      calcUSRelLoc();
    }
  }
  //delay(100);
  return;
}

