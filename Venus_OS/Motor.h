#pragma once
#include <Arduino.h>
#include "Ultrasound.h"
#include "Interrupts.h"
#include "Mapping.h"
#include "OS_functions.h"
#include "Samples.h"
#define rightEncoderPin 8
#define leftEncoderPin 7

byte** theMap;

int pos = 0;
int ultraSoundDist = 0;
int USPos = 0;
int rightFloor = 0;
int leftFloor = 0;

int failCount = 0;
int forwardUS = 200;
int xDist = 0;
int yDist = 0;

int tempEncoder = 0;
int backToMapEncoder = 0;

bool gap = 0;
bool closeUS = 0;
bool rock = 0;

byte Xposition = 15; 
byte Yposition = 15; //Variables that keep track of the robots current position

enum direction_t {UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3}; //Up is +Y direction, right is +X direction

direction_t currDirection = UP;

Servo rightWheel;
Servo leftWheel;
Servo head;
Servo gripper;

void gapCalibrate() {
  rightFloor = 0;
  leftFloor = 0;
  int i;
  int right = 0;
  int left = 0;
  for (i = 0; i < 10; i++) {
  right = analogRead(1);
  left = analogRead(2);
  //Serial.println(rightFloor);
  //Serial.println(leftFloor);
  rightFloor = rightFloor + right;
  leftFloor = leftFloor + left;
  }
  rightFloor = rightFloor/10;
  leftFloor = leftFloor/10;
}

void calcUSRelLoc() {
  int dist;
  head.write(USPos);
  dist = centimetersToTarget();
  Serial.print("Distance to mountain: ");
  Serial.println(dist,DEC);
  xDist = (int)((dist * cos((float)((float)USPos * 3.14159265f) / 90.0f))/10); //VERY CPU intensive code!
  yDist = (int)((dist * sin((float)((float)USPos * 3.14159265f) / 90.0f))/10);
  Serial.print("US X-coord: ");
  Serial.println(xDist,DEC);
  Serial.print("US Y-coord: ");
  Serial.println(yDist,DEC);
  USPos = 0;
  return;
}

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
  int i = readSamples();
  if (i > 500 && i < 850) {
    rock = 1;
    Serial.println(readSamples());
  }
  //Serial.write(readSamples());
  return;
}

void checkUS() {
  if (ultraSoundDist <= 32) {
    closeUS = 1;
  }
  return;
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
    rightWheel.write(0);
    leftWheel.write(180);

    

    rightBlackCounter = 0;
    rightWhiteCounter = 0;
    leftBlackCounter = 0;
    leftWhiteCounter = 0;
    
    checkRock();
    Serial.println(readSamples());
    if (rock) {
      
      backToMapEncoder = rightEncoder;
      stopRobot();
      return;
    }
    
    checkSides();
    checkGap();
    if (gap) {
      writeBorderToMap();
      break; // when a gap is encountered the robot immediately stops
    }
    ultraSoundDist = centimetersToTarget(); // robot scans around
    checkUS(); // checks for close objects with US data
        checkRock();
    if (rock) {
      
      backToMapEncoder = rightEncoder;
      stopRobot();
      return;
    }
  }
  if (gap) {
    rightWheel.write(90);
    leftWheel.write(90);
    tempEncoder = rightEncoder;
  }
  if (gap == 0) {
    stopRobot(); // robot stops
    updatePosition();
    writeToMatrix(theMap, Xposition, Yposition, EMPTY);
    updatePosition();
    writeToMatrix(theMap, Xposition, Yposition, EMPTY);
  }
  return;
}

void turnRight() {
  reverse = 1;
  rightEncoder = tempEncoder;
  tempEncoder = 0;
  while (rightEncoder > 0) {
    rightWheel.write(180);
    leftWheel.write(0);
  }
  gap = 0;
  reverse = 0;
  stopRobot(); // robot stops
  while (rightEncoder <= 6) { // robot turns right
    rightWheel.write(180);
    leftWheel.write(180);
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
  rightEncoder = tempEncoder;
  tempEncoder = 0;
  while (rightEncoder > 0) {
    rightWheel.write(180);
    leftWheel.write(0);
  }
  gap = 0;
  reverse = 0;
  stopRobot(); // robot stops
  while (rightEncoder <= 12) { // robot turns left
    rightWheel.write(0);
    leftWheel.write(0);
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
  head.write(0);
  delay(1200);
  for (pos = 0; pos < 180; pos += 15) { // robot sweeps head
    head.write(pos);
    delay(100); // robot waits for the equipment to do its work
    checkRock();
    ultraSoundDist = centimetersToTarget(); // robot measures distance

    if (ultraSoundDist <= 32) {
      USPos = pos; // saves position with low value, priority to targets to the front
      calcUSRelLoc();
      switch(currDirection){
        case UP:
          writeToMatrix(theMap, Xposition - xDist, Yposition + yDist, MOUNTAIN);
        break;
        case RIGHT:
          writeToMatrix(theMap, Xposition + yDist, Yposition + xDist, MOUNTAIN);
        break;
          case DOWN:
          writeToMatrix(theMap, Xposition + xDist, Yposition - yDist, MOUNTAIN);
        case LEFT:
          writeToMatrix(theMap, Xposition - yDist, Yposition - xDist, MOUNTAIN);
        break;
      }
    }
  }
  return;
}

void roam() {
  //scan(); // robot sweeps head
  closeUS = 0;
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
    calcUSRelLoc();
  }
  
}
