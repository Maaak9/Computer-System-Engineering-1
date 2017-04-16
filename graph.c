#include "at91sam3x8.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "Graph.h"

void GraphMenu(){
  int back = 0;
  setGraphScreenMenu();
  while(back != 1){
    key = readKeyValue();
    checkEverySec();
    checkWhatMode();
    switch(key){
    case 1 :
      setGraphScreen();
      printGraph(averages);
      break;
    case 2 :
      setGraphScreen();
      printGraph(maxs);
      break;
    case 3 :
      setGraphScreen();
      printGraph(mins);
      break;
    case 4 :
      for(int i=0; i<7; i++){
        float x = (float)rand()/(float)(RAND_MAX/10)+20;
        averages[i] = x;
      } 
      for(int i=0; i<7; i++){
        float x = (float)rand()/(float)(RAND_MAX/25)+10;
        maxs[i] = x;
      }
      for(int i=0; i<7; i++){
        float x = (float)rand()/(float)(RAND_MAX/15);
        mins[i] = x;
      }
      break;
      
    case 5 :
      clearGraphicScreen();
      setGraphScreenMenu();
      break;
    case 9 :
      clearGraphicScreen();
      back = 1;
      break;
    }
  }
  setupMenu();
}


void printGraph(float array[7]){
  int yCord1;
  int yCord2;
  for(int i = 0; i<6; i++){
    yCord1 = celciusToCord(Round(array[i]));
    yCord2 = celciusToCord(Round(array[i+1]));
    printLine(8+(i*5), yCord1, 14+(i*5), yCord2);
  }
}

void printTempLine(int x, int y){
  setGraphicAPointer(x, y);
  Write_Command_2_Display(0xF8 + 0);
  Write_Command_2_Display(0xF8 + 1);
  setGraphicAPointer(x+1, y);
  Write_Command_2_Display(0xF8 + 3);
  Write_Command_2_Display(0xF8 + 4);
  Write_Command_2_Display(0xF8 + 5);
  Write_Command_2_Display(0xF8 + 6);
}

void clearGraphicScreen(){
  int pixel, x, y;
  for(int row = 0; row<128; row++){
    for(int column = 0; column<=40; column++){
      int data1 = ((column+(row*40))%256);
      int data2 = (((column+(row*40))/256)+0x40);
      if(data2 > 0x40){
        //data1=data1-1;
        int k = 5;
      }
      Write_Data_2_Display(data1);
      Write_Data_2_Display(data2);
      Write_Command_2_Display(0x24);
      for(int bit = 0; bit < 6; bit++){
        Write_Command_2_Display(0xF0 + bit);
      }
    }
  }
}

void setGraphicAPointer(int x, int y){
  int data1 = ((x+(y*40))%256);
  int data2 = (((x+(y*40))/256)+0x40);
  Write_Data_2_Display(data1);
  Write_Data_2_Display(data2);
  Write_Command_2_Display(0x24);
}

int Round(float myfloat)
{
  return (int)(myfloat + 0.5);
}

int celciusToCord(int temp){
  int yCord;  
  if(temp > 0){
      yCord = 70 - temp;  // 0grader motsvarar Y Cordinat 70
    }
    else{
      yCord = 70 + temp;
    }
  return yCord;
}

void printLine(int x0,int y0,int x1,int y1){
  int sizeX = x1-x0;
  int sizeY = y1-y0;
  if(sizeY < 0){
    sizeY = y0-y1;
  }
  int i = 0;
  if(y0 == y1){
    for(i = 0; i<sizeX; i++){
      setGraphicAPointer(x0+i, y0);
      for(int bit = 0; bit < 6; bit++){
        Write_Command_2_Display(0xF8 + bit);
      }
    }
  }
  if(x0 == x1){
    for(i = 0; i<sizeY; i++){
      setGraphicAPointer(x0, y0+i);
        Write_Command_2_Display(0xF8 + 5);
      }
    }
  if(x0 != x1 & y0 != y1){
    int direction = 0;
    if(y1-y0 < 0){
      sizeY = y0-y1;
      direction = 1;
    }
    float incline = (float)(sizeX*6)/sizeY;
    float pixel = 0;
    int xCord = x0;
    int yCord = y0;
    int bit = 6;
    for(i = 0; i<sizeX*6; i++){
      if(pixel > incline){ // if(((i%incline)==0)& i != 0)
        pixel = pixel - incline;
        if(direction == 1){
          yCord--;
        }
        else{
          yCord++;
        }
      }
      if(bit < 0){
        bit = 6;
        xCord++;
      }
      setGraphicAPointer(xCord, yCord);
      Write_Command_2_Display(0xF8 + bit);
      bit--;
      pixel++;
    }
  }
}

void setGraphScreen(){
  Reset_Screen();
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(20, 0);
  Write_Data_2_Display(0x15); // sätter en 1a
  Write_Command_2_Display(0xC0);
  printDot();
  char *back = "back";
  printText(back, 4);
  
  setAdressPointer(1, 1); 
  printNumber(6);
  printNumber(0);
  setAdressPointer(1, 3);
  printNumber(4);
  printNumber(0);
  setAdressPointer(1, 6);
  printNumber(2);
  printNumber(0);
  setAdressPointer(1, 8);
  printNumber(0);
  setAdressPointer(0, 11);
  Write_Data_2_Display(0x0D);
  Write_Command_2_Display(0xC0);
  printNumber(2);
  printNumber(0);
  setAdressPointer(6, 14); 
  char *days = "mo   tu   we   th   fr   sa   su";
  printTextV2(days, 32);
  Write_Command_2_Display(0x9C);
  clearGraphicScreen();
  printLine(4, 110, 38, 110);
  printLine(4, 10, 4, 110);
  for(int i = 0; i<7; i++){
    printLine(8+(i*5), 108, 8+(i*5), 112);
  }
  for(int i = 0; i<=5; i++){
    //printStraightLine(2, 10+(i*10), 3, 10+(i*10));
    printTempLine(3, 10+(i*20));
  }
}

void setGraphScreenMenu(){
  Reset_Screen();
  
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(15, 0);
  char *menu = "graph menu";
  printText(menu, 10);
  
  setAdressPointer(3, 3);
  Write_Data_2_Display(0x11); // sätter en 1a
  Write_Command_2_Display(0xC0);
  printDot();
  char avg[9] = {' ',' ','a','v','e','r','a', 'g','e'};
  printText(avg, 9);
  
  setAdressPointer(3, 5);
  Write_Data_2_Display(0x12); // sätter en 2a
  Write_Command_2_Display(0xC0);
  printDot();
  char max[9] = {' ',' ','m','a','x','i','m','u','m'};
  printText(max, 9);
  
  setAdressPointer(3, 7);
  Write_Data_2_Display(0x13); // sätter en 3a
  Write_Command_2_Display(0xC0);
  printDot();
  char min[9] = {' ',' ','m','i','n','i','m','u','m'};
  printText(min, 9);
  
  setAdressPointer(3, 9);
  Write_Data_2_Display(0x14); // sätter en 9a
  Write_Command_2_Display(0xC0);
  printDot();
  char *test = "  test values";
  printText(test, 13);
  
  setAdressPointer(3, 11);
  Write_Data_2_Display(0x19); // sätter en 9a
  Write_Command_2_Display(0xC0);
  printDot();
  char back[6] = {' ',' ','b','a','c','k'};
  printText(back, 6);
}