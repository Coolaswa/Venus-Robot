#pragma once
#include <Arduino.h>

#define VERBOSITY 0 //Defines the feedback level over the serial monitor. This needs to be turned off if no serial monitor is used, since strings cost SRAM!
/*
Matrix values - Alias - Description
0 - UNSCANNED - Area unscanned
1 - EMPTY - Area scanned and empty
2 - POSSIBLE_MOUNTAIN - Area has been scanned with the ultrasound sensor and something was detected, but no confirmation yet
3 - MOUNTAIN - Mountain present
4 - BORDER - Region border/hole present
5 - SAMPLE - Possible rock sample location
*/

enum matrixValue_t { UNSCANNED = 0, EMPTY = 1, POSSIBLE_MOUNTAIN = 2, MOUNTAIN = 3, BORDER = 4, SAMPLE = 5};

byte** getTheMatrix(){
  
}

matrixValue_t readFromMatrix (byte** theMatrix, int Xpos, int Ypos) {
  byte Xbyte = (byte) ((float)Xpos * (3.0f/8.0f));
  byte index = (byte)((Xpos *3) - (Xbyte*8));
  byte contents = (byte)theMatrix[Xbyte][Ypos];
  byte nextContents = (byte)theMatrix[Xbyte + 1][Ypos];
  //Serial.print(contents);
  //Serial.print(" ");
  if (index == 6) {
    byte compare = 0b11000000;
    byte byte1 = (compare & contents) >> 6;
    compare = 1;
    byte byte2 = (compare & nextContents) << 2;
    return matrixValue_t(byte1 + byte2);
  } else if (index == 7) {
    byte compare = 128;
    byte byte1 = (compare & contents) >> 7;
    compare = 3;
    byte byte2 = (compare & nextContents) << 1;
    return matrixValue_t(byte1 + byte2);
  } else {
    byte compare = 7 << index;
    compare = (compare & contents) >> index;
    return (matrixValue_t)compare;
  }
}

void writeToMatrix (byte** theMatrix, int Xpos, int Ypos, matrixValue_t value) {
  byte Xbyte = (byte) ((float)Xpos * (3.0f/8.0f));
  byte index = (byte)((Xpos *3) - (Xbyte*8)); 
  byte contents = (byte)theMatrix[Xbyte][Ypos];
  byte nextContents = (byte)theMatrix[Xbyte + 1][Ypos];
  if (index == 6) {
    theMatrix[Xbyte][Ypos] = (0b00111111 & contents) + ((3 & (byte)value) << 6);
    theMatrix[Xbyte + 1][Ypos] = (0b11111110 & nextContents) + ((4 & (byte)value) >> 2);
  } else if (index == 7) {
    theMatrix[Xbyte][Ypos] = (0b01111111 & contents) + ((1 & value) << 7);
    theMatrix[Xbyte + 1][Ypos] = (0b11111100 & nextContents) + ((6 & (byte)value) >> 1);
  } else {
    theMatrix[Xbyte][Ypos] = theMatrix[Xbyte][Ypos] - (readFromMatrix(theMatrix, Xpos, Ypos) << index); 
    theMatrix[Xbyte][Ypos] += ((byte)value) << index;
  }
  return;
}

byte** createTheMatrix(int Xsize, int Ysize) {
  byte** theMatrix;
  if (Xsize % 8 != 0) {
    Xsize += 8 - (Xsize % 8);
    if(VERBOSITY >= 1){
      Serial.print("Total x grid size is not a multiple of 8, increasing to ");
      Serial.println(Xsize);
    }
  } else if(VERBOSITY >=1){
    Serial.print("Total x grid size is a multiple of 8, namely ");
    Serial.println(Xsize);
  }
  int Xbytes = (int)((float)Xsize * (3.0f / 8.0f));
  if(VERBOSITY >= 1){
    Serial.print("Total bytes needed in X direction is "); 
    Serial.println(Xbytes,DEC);
    Serial.print("Amount of rows needed in Y direction is ");
    Serial.println(Ysize);
    Serial.print("This means the total SRAM usage of the map is ");
    Serial.print(Xbytes * Ysize);
    Serial.println(" bytes");
  }
  theMatrix = (byte**) malloc( Xbytes * sizeof(byte*));
  for (int i = 0; i < Ysize; i++) {
    theMatrix[i] = (byte*) malloc( Ysize * sizeof(byte));
  }
  for (int i = 0; i < Xsize; i++) {
    for ( int j = 0; j < Ysize; j++) {
      writeToMatrix(theMatrix, i, j, UNSCANNED);
      Serial.print((byte)readFromMatrix(theMatrix, i, j));
    }
  }
  if(VERBOSITY >= 1){
    Serial.println("Created matrix");
  }
  return theMatrix;
}

void displayMatrix(byte** theMatrix, int Xsize, int Ysize){
  if(VERBOSITY >= 1){
    Serial.println("Displaying matrix");
  }
  for(int i = 0; i < Ysize; i++){
    for(int j = 0; j < Xsize; j++){
      Serial.print((byte)readFromMatrix(theMatrix, j, i));
      //readFromMatrix(theMatrix, j, i);
    }
    Serial.print("\n");
  }
  return;
}


