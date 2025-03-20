/*
 * Variables.h
 *
 *  Created on: Mar 20, 2025
 *      Author: tmuth
 */

#ifndef INC_VARIABLES_H_
#define INC_VARIABLES_H_

int leftBase =180;
int rightBase = 176;
int leftPwm ;
int rightPwm ;

unsigned long leftEncoder = 0;
unsigned long rightEncoder = 0;
unsigned long encoderLeftCount = 0;
unsigned long encoderRightCount = 0;

int dif = 0;
int test = 0;

float tof[5];
int wallAvailable[3];
int cellWalls[3];

int leftWallAvailable = 0;
int rightWallAvailable = 0;
int frontWallAvailable = 0;

int rightWall  = 1;
int leftWall = 1;

float leftP = 0.9;
float leftD = 3.4;
float leftError = 0;
float leftLastError = 0;
float leftDiff = 0;

float rightP = 0.9;
float rightD = 3.4;
float rightError = 0;
float rightLastError = 0;
float rightDiff = 0;

float leftP_side = 0.9;
float leftD_side = 3.4;
float leftSideError = 0;
float leftSideLastError = 0;
float leftSideDiff = 0;

float rightP_side = 0.9;
float rightD_side = 3.4;
float rightSideError = 0;
float rightSideLastError = 0;
float rightSideDiff = 0;


float wallP = 0.8 ;
float wallD = 2.7;
float wallError = 0;
float wallLastError = 0;
float wallDiff = 0;

float wallP_side = 0.8;
float wallD_side = 2.7;
float wallSideError = 0;
float wallSideLastError = 0;
float wallSideDiff = 0;


float encoderP = 0.2 ;
float encoderD = 0;
float encoderError = 0;
float encoderLastError = 0;
float encoderDiff = 0;

float encoderCorrection = 0;
float correction;

char nextMove = 'F';

int start = 0;

int count;


boolean left,right,front;
int cell =0;

int jump = 0;
int preState = 0;
int selectMode = -1;



#endif /* INC_VARIABLES_H_ */
