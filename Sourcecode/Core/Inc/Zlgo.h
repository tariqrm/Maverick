/*
 * Zlgo.h
 *
 *  Created on: Mar 20, 2025
 *      Author: tmuth
 */

#ifndef INC_ZLGO_H_
#define INC_ZLGO_H_

typedef struct {
    uint8_t x_0;
    uint8_t y_0;
    uint8_t x_1;
    uint8_t y_1;
    uint8_t x_2;
    uint8_t y_2;
    uint8_t x_3;
    uint8_t y_3;
} Surroundings;

void appendDestination(uint8_t xdes,uint8_t ydes, bool middleSquare){

    if (middleSquare == true){
      for (int i=0; i<16; i++){
        for (int j=0; j<16; j++){
            flood[i][j]=254;
        }
    }

    flood[8][8]=0;
    flood[7][8]=0;
    flood[8][7]=0;
    flood[7][7]=0;

    QueueArray_push(queue, 8);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 7);
    }

    else{
    for (int i=0; i<16; i++){
        for (int j=0; j<16; j++){
            flood[i][j]=254;
        }
    }

    flood[ydes][xdes]=0;

    QueueArray_push(queue, ydes);
    QueueArray_push(queue, xdes);

    }
}


uint8_t orientation(uint8_t orient, char turning){
  if (turning== 'L'){
        orient=orient-1;
        if (orient==-1|| orient== 255)
            orient=3;
  }
    else if(turning== 'R'){
        orient=orient+1;
        if (orient==4|| orient==255)
            orient=0;
    }
    else if(turning== 'B'){
        if (orient==0)
            orient=2;
        else if (orient==1)
            orient=3;
        else if (orient==2)
            orient=0;
        else if (orient==3)
            orient=1;
    }

    return(orient);
}

void updateCoordinates(){//uint8_t x, uint8_t y, uint8_t orient){

    if (orient==0)
        y=y+1;
    if (orient==1)
        x=x+1;
    if (orient==2)
        y=y-1;
    if (orient==3)
        x=x-1;

    //return(x,y);
}



void updateWalls(uint8_t x, uint8_t y, uint8_t orient, bool L, bool R, bool F){
    if(L && R && F){
        if (orient==0)
            cells[y][x]= 13;
        else if (orient==1)
            cells[y][x]= 12;
        else if (orient==2)
            cells[y][x]= 11;
        else if (orient==3)
            cells[y][x]= 14;
    }

    else if (L && R && !F){
        if (orient==0 || orient== 2)
            cells[y][x]= 9;
        else if (orient==1 || orient==3)
            cells[y][x]= 10;
    }

    else if (L && F && !R){
        if (orient==0)
            cells[y][x]= 8;
        else if (orient==1)
            cells[y][x]= 7;
        else if (orient==2)
            cells[y][x]= 6;
        else if (orient==3)
            cells[y][x]= 5;
    }

    else if (R && F && !L){
        if (orient==0)
            cells[y][x]= 7;
        else if (orient==1)
            cells[y][x]= 6;
        else if (orient==2)
            cells[y][x]= 5;
        else if (orient==3)
            cells[y][x]= 8;
    }

    else if(F){
        if (orient==0)
            cells[y][x]= 2;
        else if (orient==1)
            cells[y][x]= 3;
        else if (orient==2)
            cells[y][x]= 4;
        else if (orient==3)
            cells[y][x]= 1;
    }

    else if(L){
        if (orient==0)
            cells[y][x]= 1;
        else if (orient==1)
            cells[y][x]= 2;
        else if (orient==2)
            cells[y][x]= 3;
        else if (orient==3)
            cells[y][x]= 4;
    }

    else if(R){
        if (orient==0)
            cells[y][x]= 3;
        else if (orient==1)
            cells[y][x]= 4;
        else if (orient==2)
            cells[y][x]= 1;
        else if (orient==3)
            cells[y][x]= 2;
    }

    else
        cells[y][x]= 15;


}

bool isAccessible(uint8_t x, uint8_t y, uint8_t xNext, uint8_t yNext){
    if (xNext==254 || yNext== 254){
      return(false);
    }
    if (x==xNext){
        if(y>yNext){
            if(cells[y][x]==4 || cells[y][x]==5 || cells[y][x]==6 || cells[y][x]==10 || cells[y][x]==11 || cells[y][x]==12 || cells[y][x]==14 || cells[yNext][xNext]==2 || cells[yNext][xNext]==7 || cells[yNext][xNext]==8 || cells[yNext][xNext]==10 || cells[yNext][xNext]==12 || cells[yNext][xNext]==13 || cells[yNext][xNext]==14 )
                return (false);
            else
                return(true);
        }
        else{
            if(cells[y][x]==2 || cells[y][x]==7 || cells[y][x]==8 || cells[y][x]==10 || cells[y][x]==12 || cells[y][x]==13 || cells[y][x]==14 || cells[yNext][xNext]==4 || cells[yNext][xNext]==5 || cells[yNext][xNext]==6 || cells[yNext][xNext]==10 || cells[yNext][xNext]==11 || cells[yNext][xNext]==12 || cells[yNext][xNext]==14 )
                return (false);
            else
                return(true);
        }
    }
    else if (y==yNext){
        if(x>xNext){
            if(cells[y][x]==1 || cells[y][x]==5 || cells[y][x]==8 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==13 || cells[y][x]==14 || cells[yNext][xNext]==3 || cells[yNext][xNext]==6 || cells[yNext][xNext]==7 || cells[yNext][xNext]==9 || cells[yNext][xNext]==11 || cells[yNext][xNext]==12 || cells[yNext][xNext]==13 )
                return (false);
            else
                return (true);
        }
        else{
            if(cells[y][x]==3 || cells[y][x]==6 || cells[y][x]==7 || cells[y][x]==9 || cells[y][x]==11 || cells[y][x]==12 || cells[y][x]==13 || cells[yNext][xNext]==1 || cells[yNext][xNext]==5 || cells[yNext][xNext]==8 || cells[yNext][xNext]==9 || cells[yNext][xNext]==11 || cells[yNext][xNext]==13 || cells[yNext][xNext]==14 )
                return (false);
            else
                return (true);
        }
    }

    // Default return in case neither x == xNext nor y == yNext
    return false;
}



Surroundings getSurrounds(uint8_t x,uint8_t y){

	Surroundings s;

    s.x_3= x-1; // west
    s.y_3=y;

    s.x_0=x; //north
    s.y_0=y+1;

    s.x_1=x+1; // east
    s.y_1=y;

    s.x_2=x; //south
    s.y_2=y-1;

    if(s.x_1>= mazeSize)
    {
        s.x_1=254;
    }
    if(s.y_0>= mazeSize)
    {
        s.y_0=254;
    }
    if(s.x_3== 255)
    {
        s.x_3 =254;
    }
    if(s.y_2== 255)
    {
        s.y_2=254;
    }

    return s;  //(x_0,y_0,x_1,y_1,x_2,y_2,x_3,y_3) order of cells- north,east,south,west
}


void floodFill2(){

	for (int i=0;i<16;i++){
		for (int j=0;j<16;j++){
            flood2[i][j]=0;
        }
	}

    flood2[8][8]=1;
    flood2[7][8]=1;
    flood2[8][7]=1;
    flood2[7][7]=1;

    QueueArray_push(queue, 8);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 8);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 7);
    QueueArray_push(queue, 7);

    while (!QueueArray_isEmpty(queue)) {
        // Pop the current coordinates.
        yrun = QueueArray_pop(queue);
        xrun = QueueArray_pop(queue);

        // Get the surrounding coordinates.
        Surroundings s = getSurrounds(xrun, yrun);

        // Check north neighbor (s.x_0, s.y_0)
        if (cells[s.y_0][s.x_0] != 0) {
            if (flood2[s.y_0][s.x_0] == 0) {
                if (isAccessible(xrun, yrun, s.x_0, s.y_0)) {
                    flood2[s.y_0][s.x_0] = flood2[yrun][xrun] + 1;
                    QueueArray_push(queue, s.y_0);
                    QueueArray_push(queue, s.x_0);
                }
            }
        }
        // Check east neighbor (s.x_1, s.y_1)
        if (cells[s.y_1][s.x_1] != 0) {
            if (flood2[s.y_1][s.x_1] == 0) {
                if (isAccessible(xrun, yrun, s.x_1, s.y_1)) {
                    flood2[s.y_1][s.x_1] = flood2[yrun][xrun] + 1;
                    QueueArray_push(queue, s.y_1);
                    QueueArray_push(queue, s.x_1);
                }
            }
        }
        // Check south neighbor (s.x_2, s.y_2)
        if (cells[s.y_2][s.x_2] != 0) {
            if (flood2[s.y_2][s.x_2] == 0) {
                if (isAccessible(xrun, yrun, s.x_2, s.y_2)) {
                    flood2[s.y_2][s.x_2] = flood2[yrun][xrun] + 1;
                    QueueArray_push(queue, s.y_2);
                    QueueArray_push(queue, s.x_2);
                }
            }
        }
        // Check west neighbor (s.x_3, s.y_3)
        if (cells[s.y_3][s.x_3] != 0) {
            if (flood2[s.y_3][s.x_3] == 0) {
                if (isAccessible(xrun, yrun, s.x_3, s.y_3)) {
                    flood2[s.y_3][s.x_3] = flood2[yrun][xrun] + 1;
                    QueueArray_push(queue, s.y_3);
                    QueueArray_push(queue, s.x_3);
                }
            }
        }
    }
}


void floodFill3(){

	while (!QueueArray_isEmpty(queue)) {
	    // Pop current y and x values (order must match how you pushed them)
	    yrun = QueueArray_pop(queue);
	    xrun = QueueArray_pop(queue);

	    // Get surrounding cell coordinates as a struct
	    Surroundings s = getSurrounds(xrun, yrun);

	    // Check north neighbor (s.x_0, s.y_0)
	    if (flood[s.y_0][s.x_0] == 254) {
	        if (isAccessible(xrun, yrun, s.x_0, s.y_0)) {
	            flood[s.y_0][s.x_0] = flood[yrun][xrun] + 1;
	            QueueArray_push(queue, s.y_0);
	            QueueArray_push(queue, s.x_0);
	        }
	    }

	    // Check east neighbor (s.x_1, s.y_1)
	    if (flood[s.y_1][s.x_1] == 254) {
	        if (isAccessible(xrun, yrun, s.x_1, s.y_1)) {
	            flood[s.y_1][s.x_1] = flood[yrun][xrun] + 1;
	            QueueArray_push(queue, s.y_1);
	            QueueArray_push(queue, s.x_1);
	        }
	    }

	    // Check south neighbor (s.x_2, s.y_2)
	    if (flood[s.y_2][s.x_2] == 254) {
	        if (isAccessible(xrun, yrun, s.x_2, s.y_2)) {
	            flood[s.y_2][s.x_2] = flood[yrun][xrun] + 1;
	            QueueArray_push(queue, s.y_2);
	            QueueArray_push(queue, s.x_2);
	        }
	    }

	    // Check west neighbor (s.x_3, s.y_3)
	    if (flood[s.y_3][s.x_3] == 254) {
	        if (isAccessible(xrun, yrun, s.x_3, s.y_3)) {
	            flood[s.y_3][s.x_3] = flood[yrun][xrun] + 1;
	            QueueArray_push(queue, s.y_3);
	            QueueArray_push(queue, s.x_3);
	        }
	    }
	}
}

char toMove(uint8_t x, uint8_t y, uint8_t xprev, uint8_t yprev, uint8_t orient) {
    // Get surrounding coordinates.
    Surroundings s = getSurrounds(x, y);

    uint8_t val = flood[y][x];
    (void)val; // Suppress unused variable warning

    uint8_t prev = 254;
    uint8_t minVals[4] = {254, 254, 254, 254};
    uint8_t visited[4] = {2, 2, 2, 2};

    // Check north neighbor (s.x_0, s.y_0)
    if (isAccessible(x, y, s.x_0, s.y_0)) {
        if (s.x_0 == xprev && s.y_0 == yprev)
            prev = 0;
        if (cells[s.y_0][s.x_0] != 0)
            visited[0] = 1;
        else
            visited[0] = 2;
        minVals[0] = flood[s.y_0][s.x_0];
    } else {
        minVals[0] = 254;
    }

    // Check east neighbor (s.x_1, s.y_1)
    if (isAccessible(x, y, s.x_1, s.y_1)) {
        if (s.x_1 == xprev && s.y_1 == yprev)
            prev = 1;
        if (cells[s.y_1][s.x_1] != 0)
            visited[1] = 1;
        else
            visited[1] = 2;
        minVals[1] = flood[s.y_1][s.x_1];
    } else {
        minVals[1] = 254;
    }

    // Check south neighbor (s.x_2, s.y_2)
    if (isAccessible(x, y, s.x_2, s.y_2)) {
        if (s.x_2 == xprev && s.y_2 == yprev)
            prev = 2;
        if (cells[s.y_2][s.x_2] != 0)
            visited[2] = 1;
        else
            visited[2] = 2;
        minVals[2] = flood[s.y_2][s.x_2];
    } else {
        minVals[2] = 254;
    }

    // Check west neighbor (s.x_3, s.y_3)
    if (isAccessible(x, y, s.x_3, s.y_3)) {
        if (s.x_3 == xprev && s.y_3 == yprev)
            prev = 3;
        if (cells[s.y_3][s.x_3] != 0)
            visited[3] = 1;
        else
            visited[3] = 2;
        minVals[3] = flood[s.y_3][s.x_3];
    } else {
        minVals[3] = 254;
    }

    uint8_t minVal = 254;
    uint8_t minCell = 0;
    uint8_t noMovements = 0;

    for (int i = 0; i < 4; i++) {
        if (minVals[i] != 254) {
            noMovements++;
        }
    }

    for (int i = 0; i < 4; i++) {
        if (minVals[i] < minVal) {
            if (noMovements == 1) {
                minVal = minVals[i];
                minCell = i;
            } else {
                if (i != prev) {
                    minVal = minVals[i];
                    minCell = i;
                }
            }
        }
    }

    uint8_t bla[4] = {254, 254, 254, 254};
    for (int i = 0; i < 4; i++) {
        if (minVals[i] == minVal)
            bla[i] = visited[i];
    }

    uint8_t blaCount = 0;
    for (int i = 0; i < 4; i++) {
        if (bla[i] == 2) {
            minCell = i;
            blaCount++;
        }
    }

    if (blaCount == 0) {
        for (int i = 0; i < 4; i++) {
            if (bla[i] == 1)
                minCell = i;
        }
    }

    if (minCell == orient)
        return 'F';
    else if ((minCell == orient - 1) || (minCell == orient + 3))
        return 'L';
    else if ((minCell == orient + 1) || (minCell == orient - 3))
        return 'R';
    else
        return 'B';
}


char toMove2(void) {
    // Obtain the surrounding cell coordinates for (x, y)
    Surroundings s = getSurrounds(x, y);

    // Get the current flood value at (x, y)
    uint8_t val = flood2[y][x];
    uint8_t minCell = 254;
    char dir; // This will hold the direction to move

    // Check the north neighbor (s.x_0, s.y_0)
    if (isAccessible(x, y, s.x_0, s.y_0)) {
        if (flood2[s.y_0][s.x_0] == val - 1)
            minCell = 0;
    }
    // Check the east neighbor (s.x_1, s.y_1)
    if (isAccessible(x, y, s.x_1, s.y_1)) {
        if (flood2[s.y_1][s.x_1] == val - 1)
            minCell = 1;
    }
    // Check the south neighbor (s.x_2, s.y_2)
    if (isAccessible(x, y, s.x_2, s.y_2)) {
        if (flood2[s.y_2][s.x_2] == val - 1)
            minCell = 2;
    }
    // Check the west neighbor (s.x_3, s.y_3)
    if (isAccessible(x, y, s.x_3, s.y_3)) {
        if (flood2[s.y_3][s.x_3] == val - 1)
            minCell = 3;
    }

    // Determine the direction based on minCell relative to orient.
    if (minCell == orient)
        dir = 'F';
    else if ((minCell == orient - 1) || (minCell == orient + 3))
        dir = 'L';
    else if ((minCell == orient + 1) || (minCell == orient - 3))
        dir = 'R';
    else
        dir = 'B';

    return dir;
}



void center(){
  if (x == 7 && y == 7){
    cells[8][7] = 8;
    cells[8][8] = 7;
    cells[7][8] = 6;
    if (orient == 0){
      cells[7][7] = 1;
    }
    else if (orient == 1){
      cells[7][7] = 4;
    }
  }
  else if (x == 8 && y == 7){
    cells[8][8] = 7;
    cells[7][8] = 6;
    cells[7][7] = 5;
    if (orient == 1){
      cells[8][7] = 2;
    }
    else if (orient == 2){
      cells[8][7] = 1;
    }
  }
  else if (x == 8 && y == 8){
    cells[7][8] = 6;
    cells[7][7] = 5;
    cells[8][7] = 8;
    if (orient == 2){
      cells[8][8] = 3;
    }
    else if (orient == 3){
      cells[8][8] = 2;
    }
  }
  else if (x == 7 && y == 8){
    cells[7][7] = 5;
    cells[8][7] = 8;
    cells[7][8] = 7;
    if (orient == 0){
      cells[7][8] = 3;
    }
    else if (orient == 3){
      cells[7][8] = 4;
    }
  }
}


/*

void showCells(){
  for (int i=13; i>=0; i--){
    for (int j=0; j<14;j++){
      Serial.print(cells[i][j]);
      Serial.print(' ');
    }
    Serial.println(' ');
  }
}

void showFlood(){
  for (int i=13; i>=0; i--){
    for (int j=0; j<14;j++){
      Serial.print(flood[i][j]);
      Serial.print(' ');
    }
    Serial.println(' ');
  }
}

void showFlood2(){
  for (int i=13; i>=0; i--){
    for (int j=0; j<14;j++){
      Serial.print(flood2[i][j]);
      Serial.print(' ');
    }
    Serial.println(' ');
  }
}

*/

#endif /* INC_ZLGO_H_ */
