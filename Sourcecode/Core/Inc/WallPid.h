/*
 * WallPid.h
 *
 *  Created on: Mar 20, 2025
 *      Author: tmuth
 */

#ifndef INC_WALLPID_H_
#define INC_WALLPID_H_


void leftPid()
{
    leftError = 100 - tof[0];
    leftDiff = leftError - leftLastError;

    if (leftDiff > 50 )
    {
        leftError = 10;
        leftDiff = 5;
    }

    else if (leftDiff < -50)
    {
        leftError = -10;
        leftDiff = -5;
    }

    correction = (leftError * leftP) + (leftDiff * leftD);
    leftLastError = leftError;

    if (correction > 50 )
    {
        correction = 15;
    }

    else if (correction < -50)
    {
        correction = -15;
    }

    leftPwm = leftBase + correction;
    rightPwm = rightBase - correction;
}

void leftSidePid() {
    leftSideError = 50 - tof[1];
    leftSideDiff = leftSideError - leftSideLastError;
    if (leftSideDiff > 25)
    {
        leftSideError = 10;
        leftSideDiff = 5;
    }
    else if (leftSideDiff < -25) {
        leftSideError = -10;
        leftSideDiff = -5;
    }

    correction = (leftSideError * leftP_side) + (leftSideDiff * leftD_side);
    leftSideLastError = leftSideError;

    if (correction > 25)
    {
        correction = 15;
    }
    else if (correction < -25)
    {
        correction = -15;
    }

    leftPwm = leftBase + correction;
    rightPwm = rightBase - correction;
}


void rightPid()
{
    rightError = 95 - tof[4];
    rightDiff = rightError - rightLastError;
    if (rightDiff > 50 )
    {
        rightError = 10;
        rightDiff = 5;
    }

    else if (rightDiff < -50)
    {
        rightError = -10;
        rightDiff = -5;
    }

    correction  = (rightError * rightP) + (rightDiff * rightD);
    rightLastError = rightError;

    if (correction > 50 )
    {
        correction = 15;
    }

    else if (correction < -50)
    {
        correction = -15;
    }

    leftPwm = leftBase - correction;
    rightPwm = rightBase + correction;

}

void rightSidePid() {
    rightSideError = 50 - tof[3];
    rightSideDiff = rightSideError - rightSideLastError;

    if (rightSideDiff > 25)
    {
        rightSideError = 10;
        rightSideDiff = 5;
    }
    else if (rightSideDiff < -25)
    {
        rightSideError = -10;
        rightSideDiff = -5;
    }

    correction = (rightSideError * rightP_side) + (rightSideDiff * rightD_side);
    rightSideLastError = rightSideError;

    if (correction > 25)
    {
        correction = 15;
    }
    else if (correction < -25)
    {
        correction = -15;
    }

    leftPwm = leftBase - correction;
    rightPwm = rightBase + correction;
}


void wallPid()
{
    wallError = tof[0] - (tof[4]-8);
    rightLastError = 58 - tof[4];
    leftLastError = 50 - tof[0];
    wallDiff = wallError - wallLastError;

    if (wallDiff > 50 )
    {
        wallError = 10;
        wallDiff = 5;
    }

    else if (wallDiff < -50)
    {
        wallError = -10;
        wallDiff = -5;
    }

    correction = (wallError * wallP) + (wallDiff * wallD);
    wallLastError = wallError;

    if (correction > 50 )
    {
        correction = 10;
    }

    else if (correction < -50)
    {
        correction = -10;
    }

    leftPwm = leftBase - correction;
    rightPwm = rightBase + correction;

}


void wallSidePid() {
    wallSideError = tof[1] - (tof[3] - 2);
    rightSideLastError = 29 - tof[3];
    leftSideLastError = 25 - tof[1];
    wallSideDiff = wallSideError - wallSideLastError;

    if (wallSideDiff > 25)
    {
        wallSideError = 10;
        wallSideDiff = 5;
    }
    else if (wallSideDiff < -25)
    {
        wallSideError = -10;
        wallSideDiff = -5;
    }

    correction = (wallSideError * wallP_side) + (wallSideDiff * wallD_side);
    wallSideLastError = wallSideError;

    if (correction > 25)
    {
        correction = 10;
    }
    else if (correction < -25)
    {
        correction = -10;
    }

    leftPwm = leftBase - correction;
    rightPwm = rightBase + correction;
}


void encoderPid()
{
    encoderError = leftEncoder - rightEncoder;
    if (encoderError > 50 )
    {
        encoderError = 10;
    }
    else if (encoderError < -50)
    {
        encoderError = -10;
    }
    encoderCorrection = (float)(encoderError * encoderP) + (float)(encoderLastError * encoderD);
    encoderLastError = encoderError;
    leftPwm = leftBase - encoderCorrection;
    rightPwm = rightBase + encoderCorrection -10;
    forward();
}


void wallFollow()
{
    tofPid();
    if (tof[0] <= 135 && tof[4] <= 135)
    {
        wallPid();
        forward();
    }
    else if (tof[0] > 135 && tof[4] <= 135)
    {
        rightPid();
        forward();
    }
    else if (tof[0] <= 135 && tof[4] > 135)
    {
        leftPid();
        forward();
    }
    else if(tof[0] > 135 && tof[4] > 135)
    {
        encoderPid();
    }
}



void wallSideFollow()
{
    tofReadAll();
    if (tof[1] <= 75 && tof[3] <= 75)
    {
        //Serial2.println("wall");
        wallSidePid();
        forward();
    }
    else if (tof[1] > 75 && tof[3] <= 75)
    {
        rightSidePid();
        //Serial2.println("right");
        forward();
    }
    else if (tof[1] <= 75 && tof[3] > 75)
    {
        leftSidePid();
        //Serial2.println("left");
        forward();
    }
    else if(tof[1] > 75 && tof[3] > 75)
    {
        //Serial2.println("encoder");
        encoderPid();
    }
}




#endif /* INC_WALLPID_H_ */
