#include <stdio.h>
#include "at91sam3x8.h"
//#include "Dlinkedlist.h"
#include "Logging.h"
#include "Main.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int speed = -1;

void loggingMenu(){
  loggingScreen();
  int back = 0;
  while(back != 1){
    checkEverySec();
    checkWhatMode(); 
    key = readKeyValue();
    switch(key){
    case 1 :
      printLogg(averages);
      break;
    case 2: 
      printLogg(maxs);
      break;
    case 3 :
      printLogg(mins);
      break;
    case 4 : 
      testFullArray();
      break;
    case 5 :
      while(speed == -1){
      checkEverySec();
      checkWhatMode();
      key = readKeyValue();
      if(key>0 && key<10){
        setAdressPointer(26, 11);
        Write_Data_2_Display(0x1D); // sätter en =
        Write_Command_2_Display(0xC0);
        setAdressPointer(28, 11);
        printNumber(key);
        fastMode = 60000/key;
        speed = key;
        }
      }
      break;
    case 9 :
      back = 1;
      MenuID = 0;
      break;
    }
  }
  setupMenu();
}

void printLogg(float arr[]){
  int row = 3;
  int i=0;
  for(i=0; i<7; i++){
    setAdressPointer(26, row);
    row++;
    volatile double value = (double)arr[i];
    printFloatWith2Dec(value);
  }
}


void logg2Array(float temp){
  static int day = 0;
  static int start = 0;
  static int arrayNr= 0;
  int length = 0;
  logging[arrayNr] = temp;
  arrayNr++;
  int stop = start + 1440;
  
  if(arrayNr == stop){
    averages[day] = average(logging, start, stop);
    mins[day] = minima(logging, start, stop);
    maxs[day] = maxima(logging, start, stop);  
    day++;
    start = 1440*day;
  }
  
  if(day == 7){
    day = 0;
    start = 0;
    arrayNr = 0;
  }
  
}


void logg2linkedlist(){
  /*
  static int day = 0;
  static int min = 0;
  while(AddLast(TrueTemp, listan) == -1){
    removeFirst(listan);
  }
  min++;
  if(min == 10){
    averages[day] = average1(listan, min);
    day++;
    min = 0;
  }
  if(day == 7){day = 0;}
  */
}

/*  Funkade inte bra
float average1(linkedlist *lista, int nVals){
  float sum = 0;
  int i;
  float answer;
  node * nodetemp = (node *)malloc(sizeof(node));
  int start = (lista->size)-nVals;
  nodetemp = lista->head->next;
  if(start !=0){
    for(int k = 0; k<start; k++){
      nodetemp = nodetemp->next;
    }
  }
  for(i=0; i<nVals; i++){
    nodetemp = nodetemp->next;
    sum += nodetemp->value;
  }
  free(nodetemp);
  answer = (sum/nVals);
  return answer;
}
*/

float average(float dVec[], int start, int end){
  float sum = 0;
  int i;
  float answer;
  for(i=start; i<=end; i++){
    sum += dVec[i];
  }
  answer = (float)(sum/(end-start));
  return answer;
}

float minima(float dVec[], int start, int end){
  float minsta = 99;
  for(int i=start; i<end; i++){
    if(dVec[i]<minsta){
      minsta = dVec[i];
    }
  }
  return minsta;
} 

float maxima(float dVec[], int start, int end){
  float storsta = 0;
  for(int i=start; i< end; i++){
    if(dVec[i]>storsta){
      storsta = dVec[i];
    }
  }
  return storsta; 
}

void testFullArray(){
  for(int i = 0; i<10080; i++){
    float temp = (float)((rand()/(float)(RAND_MAX)) * 32);
    logg2Array(temp);
  }
}