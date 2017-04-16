#include "at91sam3x8.h"
#include "Main.h"
#include "Dlinkedlist.h"
#include "Snake.h"

int snakeSpeed = 0;

void snakeGame(){
  int start = 0;
  Reset_Screen();
  removeAll(listan);
  AddLast(18,8, listan);
  AddLast(19,8, listan);
  setUpGameScreen();
  while(start != 1){
    key = readKeyValue();
    if(key == 6){start = 1;}
  }
  setAdressPointer(18,8);
  Write_Data_2_Display(0x0A); 
  Write_Command_2_Display(0xC0);
  Write_Data_2_Display(0x0A); 
  Write_Command_2_Display(0xC0);
  startGame();
}

void startGame(){
  int back = 0;
  int xPos = 19; // start pos
  int yPos = 8;
  int rembKey = 0;
  int lastRoundKey = 6;
  int moved = 0;
  while(back != -1){
    
    key = readKeyValue();
    if(key == 2 | key == 4 | key == 5 | key == 6){rembKey = key;}
    if(count1sec>300){
      switch(rembKey){  // direction 
        case 2 :
          if(lastRoundKey != 5){
            yPos = yPos-1;
            back = checkColide(xPos, yPos);
            AddLast(xPos, yPos , listan);
            printSnake(listan);
            lastRoundKey = 2;
            moved = 1;
          }
          else{
            rembKey = 5;
            moved = 0;
          }
        break;
        case 4 :
          if(lastRoundKey != 6){
            xPos = xPos -1;
            back = checkColide(xPos, yPos);
            AddLast(xPos, yPos , listan);
            printSnake(listan);
            lastRoundKey = 4;
            moved = 1;
          }
          else{
            rembKey = 6;
            moved = 0;
          }
        break;
        case 5 :
          if(lastRoundKey != 2){
            yPos = yPos+1;
            back = checkColide(xPos, yPos);
            AddLast(xPos, yPos , listan);
            printSnake(listan);
            lastRoundKey = 5;
            moved = 1;
          }
          else{
            rembKey = 2;
            moved = 0;
          }
        break;
        case 6 : 
          if(lastRoundKey != 4){
            xPos = xPos+1;
            back = checkColide(xPos, yPos);
            AddLast(xPos, yPos , listan);
            printSnake(listan);
            lastRoundKey = 6;
            moved = 1;
            }
            else{
              rembKey = 4;
              moved = 0;
            }
        break;
      }
      if(moved == 1){
        count1sec = 0;
      }
    }
  }
  RestartOrBack();
}

void RestartOrBack(){
 int back = 0;
 while(back == 0){
  key = readKeyValue();
  if(key>0 & key<3){
    back = key;
  }
 }
 if(back == 1){
 }
 if(back == 2){
   snakeGame();
 }
}

int checkColide(int checkX, int checkY){ // alive 1 / dead -1
  for(int i=0; i<40; i++){
    if(checkX == i & checkY == 0){
      return -1;
    }
    if(checkX == i & checkY == 15){
      return -1;
    }
  }
  for(int i=0; i<17; i++){
    if(checkX == 0 & checkY == i){
      return -1;
    }
    if(checkX == 39 & checkY == i){
      return -1;
    }
  }
  return 1;
}

void setUpGameScreen(){
  setAdressPointer(0, 0);
  for(int i=0; i<40; i++){
    Write_Data_2_Display(0x03);
    Write_Command_2_Display(0xC0);
  }
  for(int i=0; i<16; i++){
    setAdressPointer(0, i);
    Write_Data_2_Display(0x03); 
    Write_Command_2_Display(0xC0);
    setAdressPointer(39, i);
    Write_Data_2_Display(0x03); 
    Write_Command_2_Display(0xC0);
  }
  setAdressPointer(0, 15);
  for(int i=0; i<40; i++){
    Write_Data_2_Display(0x03);
    Write_Command_2_Display(0xC0);
  }
  
}

void printSnake(linkedlist *lista){
  int x = lista->head->next->xCord;
  int y = lista->head->next->yCord;
  setAdressPointer(x, y);
  Write_Data_2_Display(0x00); 
  Write_Command_2_Display(0xC0);
  removeFirst(listan);
  x = lista->tail->xCord;
  y = lista->tail->yCord;
  setAdressPointer(x, y);
  Write_Data_2_Display(0x0A); 
  Write_Command_2_Display(0xC0);
}

