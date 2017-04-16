#include <stdio.h>
#include "at91sam3x8.h"
#include "Checkers.h"
#include "Logging.h"
#include "Main.h"    

void checkEverySec(){
    if(count1sec > 1000){
      Temp_Mesurement();
      count1sec = count1sec-1000;
      PrintTemp2Screen();
      checkAlarm();
      checkFastMode();
    }
}

void checkWhatMode(){
  static int counter = 0;
  static float avgTemp[9];
    if(secORmin > fastMode){
      Temp_Mesurement(); 
      if(speed != -1){
        avgTemp[counter] = TrueTemp;
        counter++;
        if(counter == speed){
          float avg = 0;
          for(int i = 0; i<counter; i++){
            avg = avg + avgTemp[i];
          }
          avg = avg / counter;
          logg2Array(avg);
        }
      }
      else{
        float temp = TrueTemp;
        logg2Array(temp);
      }
      secORmin = secORmin - fastMode;
    }
}