#include <Arduino.h>

int forwardCount = 0;
int path[512];
int pathLoc = 0;
int xPath = 0;
int yPath = 0;
int directionFaced = 0;

void resetPath() {
  forwardCount = 0;
  int i;
  for (i = 0; i < 511; i++) {
    path[i] = 0;
  }
  pathLoc = 0;
  xPath = 0;
  yPath = 0;
}

void oneForward() {
  forwardCount++;
  return;
}

void wentRight() {
  directionFaced++;
  if (directionFaced > 3) {
    directionFaced = directionFaced - 4;
  }
  if (directionFaced > 1) {
    path[pathLoc] = -forwardCount;
  }
  if (directionFaced < 2) {
    path[pathLoc] = forwardCount;
  }
  
  pathLoc++;
  return;
}

void wentLeft() {
  directionFaced = directionFaced - 2;
  if (directionFaced < 0) {
    directionFaced = directionFaced + 4;
  }
  if (directionFaced > 1) {
    path[pathLoc] = -forwardCount;
  }
  if (directionFaced < 2) {
    path[pathLoc] = forwardCount;
  }
  pathLoc--;
  path[pathLoc] = forwardCount;
  pathLoc++;
  return;
}

void foundRock() {
  path[pathLoc] = forwardCount;
  return;
}

void determinePath() {
  int i;
  for (i = 1; i <= 511; i + 2) {
    xPath = xPath + path[i];
  }
  for (i = 0; i <= 511; i + 2) {
    yPath = yPath + path[i];
  }
  return;
}

