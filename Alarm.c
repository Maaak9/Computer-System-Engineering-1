#include "Alarm.h"
#include "Main.h"
void setUpperLimit();
void setLowerLimit();

void setAlarm(){
  int back = 0;
  setAlarmScreen();
  while(back != 1){
    key = readKeyValue();
    checkEverySec();
    checkWhatMode();
    switch(key){
    case 1 :
      setUpperLimit();
      break;
    case 2 :
      setLowerLimit();
      break;
    case 9 :
      back = 1;
      MenuID = 0;
      break;
    }
  }
  setupMenu();
}

void setUpperLimit(){
  
  int count = 0;
  int first;
  int second;
  //Delay(10000000);
  while(count != 2){
    key = readKeyValue();
    checkEverySec();
    checkWhatMode();
    if(key != -1){
      count++;
      if(count == 1) {
        first = key;
        setAdressPointer(30, 3);
        printNumber(first);
      }
      if(count == 2) {
        second = key;
        setAdressPointer(31, 3);
        printNumber(second); 
      }
      Delay(8000000);
    }
  }
  setAdressPointer(32, 3);
  Write_Data_2_Display(0x23); // sätter en C
  Write_Command_2_Display(0xC0);
  upperAlarm = (first*10)+second; 
}

void setLowerLimit(){
  int count = 0;
  int first;
  int second;
  Delay(10000000);
  while(count != 2){
    key = readKeyValue();
    if(key != -1){
      checkEverySec();
      checkWhatMode();
      count++;
      if(count == 1) {
        first = key;
        setAdressPointer(30, 4);
        printNumber(first);
      }
      if(count == 2) {
        second = key;
        setAdressPointer(31, 4);
        printNumber(second);
      }
      Delay(8000000);
    }
  }
  setAdressPointer(32, 4);
  Write_Data_2_Display(0x23); // sätter en C
  Write_Command_2_Display(0xC0);
  lowerAlarm = ((first*10)+second); 
}

int Roundfloat(float myfloat)
{
  return (int)(myfloat + 0.5);
}

void checkAlarm(){
  static int checker = 0;
  float temp222 = TrueTemp;
  int temprature = Roundfloat(temp222);
  if(temprature > upperAlarm | temprature < lowerAlarm & checker == 0){
    setAdressPointer(13, 15);
    char alarm[10] = {'t','e','m','p',' ','a','l','a','r','m'};
    printText(alarm, 10);
    checker = 1;
  }
  if(temprature < upperAlarm & temprature > lowerAlarm & checker == 1){
    setAdressPointer(13, 15);
    for(int i=0; i<11; i++){
      Write_Data_2_Display(0x00);
      Write_Command_2_Display(0xC0);
    }
    checker = 0;
  }
}


void checkFastMode(){
  if(fastMode == 1000){
    setAdressPointer(32, 0);
    char *fast = "fastmode";
    printTextV2(fast, 8);
  }
  if(fastMode != 1000){
    setAdressPointer(32, 0);
    for(int i=0; i<9; i++){
      Write_Data_2_Display(0x00);
      Write_Command_2_Display(0xC0);
    }
  } 
  
}

