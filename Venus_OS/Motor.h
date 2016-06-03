#pragma once
#include <Arduino.h>
#include "Ultrasound.h"
#include "Interrupts.h"
#define rightEncoderPin 8
#define leftEncoderPin 7
#include "Mapping.h"

int pos = 0;
int ultraSoundDist = 0;
int USPos = 0;
int rightFloor = 0;
int leftFloor = 0;

bool gap = 0;
bool closeUS = 0;

byte Xposition = 10, Yposition = 10; //Variables that keep track of the robots current position

enum direction_t {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3}; //Up is +Y direction, right is +X direction

direction_t currDirection = UP;

byte** theMap;

Servo rightWheel;
Servo leftWheel;
Servo head;
Servo gripper;

void stopRobot() {
  rightWheel.write(90); // robot stops
  leftWheel.write(90);
  rightEncoder = 0;
  leftEncoder = 0;
}

void gripperOpen() {
  gripper.write(90); // grippers open
}

void gripperGrab() {
  gripper.write(1); // grippers close
}

void checkGap() {
  if (gapRight == 1 || gapLeft == 1) {
    gap = 1;
    gapRight = 0;
    gapLeft = 0;
  }
}


void checkUS() {
  if (ultraSoundDist <= 30) {
    closeUS = 1;
  }
}

void checkSides(){
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
    return;
}

void writeBorderToMap(){
        if(rightEncoder < 8){
        switch(currDirection){
          case UP:
            writeToMatrix(theMap, Xposition, Yposition + 1, BORDER);
            break;
          case RIGHT:
            writeToMatrix(theMap, Xposition + 1, Yposition, BORDER);
            break;
          case DOWN:
            writeToMatrix(theMap, Xposition, Yposition - 1, BORDER);
            break;
          case LEFT:
            writeToMatrix(theMap, Xposition - 1, Yposition, BORDER);
            break;
        }
      } else {
        switch(currDirection){
          case UP:
            writeToMatrix(theMap, Xposition, Yposition + 2, BORDER);
            writeToMatrix(theMap, Xposition, Yposition + 1, EMPTY);
            break;
          case RIGHT:
            writeToMatrix(theMap, Xposition + 2, Yposition, BORDER);
            writeToMatrix(theMap, Xposition + 1, Yposition, EMPTY);
            break;
          case DOWN:
            writeToMatrix(theMap, Xposition, Yposition - 2, BORDER);
            writeToMatrix(theMap, Xposition, Yposition -1, EMPTY);
            break;
          case LEFT:
            writeToMatrix(theMap, Xposition - 2, Yposition, BORDER);
            writeToMatrix(theMap, Xposition -1, Yposition, EMPTY);
            break;
        }
      }
}

void updatePosition(){
          switch(currDirection){
          case UP:
            Yposition++;
            break;
          case RIGHT:
            Xposition++;
            break;
          case DOWN:
            Yposition--;
            break;
          case LEFT:
            Xposition--;
            break;
        }
}

void driveForward() {
  while (rightEncoder < 16) { // robot moves forward
    rightWheel.write(180);
    leftWheel.write(0);

    rightBlackCounter = 0;
    rightWhiteCounter = 0;
    leftBlackCounter = 0;
    leftWhiteCounter = 0;

    checkSides();
    
    checkGap();
    if (gap) {
      writeBorderToMap();
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
    updatePosition();
    writeToMatrix(theMap, Xposition, Yposition, EMPTY);
    
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
  if(currDirection < LEFT){
    currDirection = (direction_t)((direction_t)currDirection + (direction_t)RIGHT);
  } else {
    currDirection = UP;
  }
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
  if(currDirection == UP){
    currDirection = LEFT;
  } else {
    currDirection = (direction_t)((direction_t)currDirection - (direction_t)RIGHT);
  }
  return;
}

void scan() {
  for (pos = 0; pos < 180; pos += 1) { // robot sweeps head
    head.write(pos);
    ultraSoundDist = centimetersToTarget(); // robot measures distance
    delay(15); // robot waits for the equipment to do its work
    if (pos >= 120 && USPos > 0) { // robot doesn't care about things to the side when there's someting in front of it
      break;
    }
    if (ultraSoundDist <= 80) {
      USPos = pos; // saves position with low value, priority to targets to the front
    }
  }
  //delay(100);
}
