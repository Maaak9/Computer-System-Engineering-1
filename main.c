#include "system_sam3x.h"
#include "at91sam3x8.h"
#include "core_cm3.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Main.h"
#include "Checkers.h"
#include "Display.h"
#include "Keypad.h"
#include <math.h>
#include "Temp.h"
#include "Servo.h"
#include "Alarm.h"
#include "DlinkedList.h"
#include "Graph.h"
#include "Snake.h"
#include "Logging.h"


#define AT91C_PIOD_AIMER (AT91_CAST(AT91_REG *) 0x400E14B0) // (PIOD) Additional Interrupt Modes Enable Register
#define AT91C_PIOD_DIFSR (AT91_CAST(AT91_REG *) 0x400E1484) //  Debouncing Input Filtering Select Register
#define AT91C_PIOD_SCDR (AT91_CAST(AT91_REG *) 0x400E148C) // PIO Slow Clock Divider Debouncing Register
#define AT91C_PIOD_FELLSR (AT91_CAST(AT91_REG *)0x400E14D0) // Falling Edge/Low Level Select Register
#define AT91C_PIOD_LSR (AT91_CAST(AT91_REG *)0x400E14C4) // Level Select Register
#define leftshift14 (1<<14)
#define leftshift2 (1<<2)
#define leftshift4 (1<<4)

double lightValueRight;
double lightValueLeft;
int upperAlarm = 999;
int lowerAlarm = -999;
int secORmin = 0; //count1sec
int count1sec = 0;
int fastMode = 60000;
int checkKey = 0;
float Temp_Timer = 0; 
float TrueTemp = 0;
int MenuID = 0;
int key = 0;
int servoPos = 3937;
float logging[10080];
float averages[7];
float maxs[7];
float mins[7];

linkedlist *listan;
//float *farray;

void SysTick_Handler(void);

void Delay(int Value);
void setUpperLimit();
void setLowerLimit();


void Delay(int Value){
  int i;
  for(i=0;i<Value;i++){
    asm("nop");
  }
}

 void SysTick_Handler(void){
  count1sec++;
  checkKey++;
  secORmin++;
 }


void TC0_Handler(){
  Temp_Timer =  *AT91C_TC0_RB - *AT91C_TC0_RA;
  *AT91C_TC0_IDR = 0x40; // interupt disable register
}


void main(void){
  SystemInit(); // Disables the Watchdog and setup the MCK
  *AT91C_PMC_PCER = 0x00002000; // start clock PIOC
  Init_Display();
  Reset_Screen();
  initKeypad();
  initTemp();
  Temp_Mesurement();
  Delay(840000);
  Temp_Mesurement();
  //Master_UI_Ten_out_of_Ten();
  initPWM();
  initLightSensor();
  setupMenu();
  

  listan = (linkedlist *) malloc(sizeof(linkedlist));
  linkedlistInit(listan);

  
  SysTick_Config(84000);
  setupMenu();
  while(50){
    key = readKeyValue();
    checkEverySec();
    checkWhatMode();  
    switch(key){
      case 1 : 
        if(fastMode == 60000){
          fastMode = 1000; // 1sec
          secORmin = 0;
          speed = -1;
        }
        else{
          fastMode = 60000; // 1min
          secORmin = 0;
          speed = -1;
        }
        break;
      case 2 : 
        lightMenu();
        break;
      case 3 :
        setAlarm();
        break;
      case 4 :
        loggingMenu();
        break;
      case 5 : 
        GraphMenu();
        break;
      case 6 :
        snakeGame();
        break;
    }
  }
}
