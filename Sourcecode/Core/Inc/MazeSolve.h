/*
 * MazeSolve.h
 *
 *  Created on: Mar 20, 2025
 *      Author: tmuth
 */

#ifndef INC_MAZESOLVE_H_
#define INC_MAZESOLVE_H_

/*
void eepromClear(){
  for (int i=0 ;i< 250; i++){
    EEPROM.write(i,0);
  }
}

void loadCells(){
  for (int i=0;i<14;i++){
    for (int j=0; j<14;j++){
      cells[j][i]= EEPROM.read(i*14+j);
    }
  }
}

void writeCells(){
  for (int i=0;i<14;i++){
    for (int j=0; j<14;j++){
      EEPROM.write(i*14+j ,cells[j][i]);
    }
  }
}

*/


void calculatePath(bool runningNext){
  floodFill2();

  uint8_t xprevious=x;
  uint8_t yprevious=y;
  x=0;
  y=0;
  uint8_t oldOrient= orient;
  orient=0;

  cellCount=0;

    while(flood2[y][x]!=1){
        toMove2();
        //QueueArray_push(pathQueue, (uint8_t)dir);


        if (runningNext== true){
        	QueueArray_push(pathQueue, (uint8_t)dir);
        }

        //Serial2.println(dir);

        if (dir=='L'){
            orient = orientation(orient,'L');
        }

        else if (dir=='R'){
            orient = orientation(orient,'R');
        }

        else if (dir=='B'){
            orient = orientation(orient,'L');
            orient = orientation(orient,'L');
        }


        xprev=x;
        yprev=y;
        updateCoordinates();


  }


        x= xprevious;
        y= yprevious;
        orient=oldOrient;
        buzz();
}


void traverse(uint8_t xdes, uint8_t ydes, bool middleSquare, bool shortPath, bool smooth){

  uint8_t currentx= x;
  uint8_t currenty= y;

  if (shortPath== false){

  appendDestination(xdes,ydes,middleSquare);
  floodFill3();
  checkWallsCell();
  updateWalls(x, y, orient, L, R, F);

  while(flood[y][x]!=0){
    //Serial2.println();
    //Serial2.print("x : ");
    //Serial2.print(x);
    //Serial2.print(", y : ");
    //Serial2.println(y);

    checkWallsCell();
    updateWalls(x, y, orient, L, R, F);
    appendDestination(xdes,ydes,middleSquare);
    floodFill3();
    dir= toMove(x,y,xprev,yprev,orient);

    //Serial2.print("toMove() : ");
    //Serial2.println(dir);


        if (dir=='L'){
            orient = orientation(orient,'L');
            if(x==0 || x== currentx){
              if(y==0 || y== currenty){
                leftAboutTurn();
                HAL_Delay(500);
                cellStart();
                currentx=0;
                currenty=0;
              }
              else{
                leftTurn();
              }
            }
            else{
            leftTurn();}
        }

        else if (dir=='R'){
          orient = orientation(orient,'R');
            if(x==0 || x== currentx){
              if(y==0 || y== currenty){
                rightAboutTurn();
                HAL_Delay(500);
                cellStart();
                currentx=0;
                currenty=0;
              }
              else{
                rightTurn();
              }
            }
            else{
            rightTurn();}
        }

        else if (dir=='B'){
            orient = orientation(orient,'L');
            orient = orientation(orient,'L');
            if((x==0 && y==0)||(x== currentx && y== currenty)){
              turnBack();
              HAL_Delay(500);
              cellStart();
              currentx=0;
                currenty=0;
              }
              else{
              cellBack();
              }
        }
        else{
          if((x==0 && y==0)||(x== currentx && y== currenty)){
            cellStart();
            currentx=0;
            currenty=0;
          }
          else{
          cellForward();
          }
        }
        xprev=x;
        yprev=y;
        updateCoordinates();
  }
  }

  else{

        F= false;
        R= false;
        L= false;

    if (smooth == true){
        calculatePath(true);

        while (!QueueArray_isEmpty(pathQueue))
        {

        dir= (char)QueueArray_pop(pathQueue);

        if (dir=='L'){
            orient = orientation(orient,'L');
            if((x==0 && y==0)||(x== currentx && y== currenty)){
              leftAboutTurn();
              HAL_Delay(500);
              cellStart();
              currentx=0;
              currenty=0;
            }
            else{
            leftSmoothTurn();}

        }

        else if (dir=='R'){
            orient = orientation(orient,'R');
            if((x==0 && y==0)||(x== currentx && y== currenty)){
              rightAboutTurn();
              HAL_Delay(500);
              cellStart();
              currentx=0;
              currenty=0;
            }
            else{
            rightSmoothTurn();}
        }

        else if (dir=='B'){
            orient = orientation(orient,'L');
            orient = orientation(orient,'L');
            if((x==0 && y==0)||(x== currentx && y== currenty)){
              turnBack();
              HAL_Delay(500);
              cellStart();
              currentx=0;
              currenty=0;
            }
            else{
            cellBack();}
          }

        else{
          if((x==0 && y==0)||(x== currentx && y== currenty)){
              cellStart();
              currentx=0;
              currenty=0;
            }
            else{
            cellForward();}
          }

          xprev=x;
          yprev=y;
          updateCoordinates();

      }


    }

    else{

    calculatePath(true);

    while (!QueueArray_isEmpty(pathQueue)){
    checkWallsCell();
    dir= (char)QueueArray_pop(pathQueue);

        if (dir=='L'){
            orient = orientation(orient,'L');
            if((x==0 && y==0)){
              leftAboutTurn();
              HAL_Delay(500);
              cellStart();
            }
            else{
            leftTurn();}

        }

        else if (dir=='R'){
            orient = orientation(orient,'R');
            if((x==0 && y==0)){
              rightAboutTurn();
              HAL_Delay(500);
              cellStart();
            }
            else{
            rightTurn();}
        }

        else if (dir=='B'){
            orient = orientation(orient,'L');
            orient = orientation(orient,'L');
            if((x==0 && y==0)){
              turnBack();
              HAL_Delay(500);
              cellStart();
            }
            else{
            cellBack();}
          }

        else{
          if((x==0 && y==0)){
              cellStart();
            }
            else{
            cellForward();}
          }

          xprev=x;
          yprev=y;
          updateCoordinates();

        }
    }
  }
  }


void fixOrientation(){
  while(orient!=0){
    leftAboutTurn();
    orient = orientation(orient,'L');
    HAL_Delay(500);
  }
}


void searchStates(){
      mazeStart();

      traverse(0,0,true,false,false);
      L= false; R= false; F= false;
      center();
      cellBrake();
      buzz();
      HAL_Delay(3000);
      traverse(15,0,false,false,false);
      cellBrake();
      buzz();
      traverse(0,0,false,false,false);
      cellBrake();
      fixOrientation();
      HAL_Delay(3000);

}




#endif /* INC_MAZESOLVE_H_ */
