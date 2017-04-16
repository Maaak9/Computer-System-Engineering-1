#include "at91sam3x8.h"
#include "Light.h"
#include "Main.h"

void initLightSensor(void){
  *AT91C_PMC_PCER = 0x800; // start clock PIOA
  *AT91C_PMC_PCER1 = 0x20; // start clock ADC
  *AT91C_ADCC_MR = 0x200; // set prescale, speed 14Mhz
}

void measureLight(void){
  *AT91C_ADCC_CHER = 0x6; // Select channel in ADC_CHER   channel  2 och 3
  *AT91C_ADCC_CR = 0x2; //  Start analog to digital conversion
  Delay(20);
    // Sida 50 och 870
    lightValueRight = *AT91C_ADCC_CDR1 & 0xFFF;
    lightValueLeft = *AT91C_ADCC_CDR2 & 0xFFF;
  //} // convert to Voltage
  lightValueLeft = ((lightValueLeft)/(4095/3.3)); // 4095 max = FFF 
  lightValueRight = ((lightValueRight)/(4095/3.3));  // (det analog värdet)/(maxvalue 4095 / spänningen 3.3V)
}

void findLight(void){
  if(lightValueRight-lightValueLeft > 0.15 | lightValueLeft-lightValueRight > 0.15){
    if(lightValueRight > lightValueLeft){
        servoPos = servoPos - 200;
        if(servoPos < 1800) {servoPos = servoPos + 200;}
        if(servoPos > 1800 && servoPos < 6200){
          *AT91C_PWMC_CH1_CPRDR = 52500;
          *AT91C_PWMC_CH1_CDTYR = servoPos;
          }
        }
    if(lightValueLeft > lightValueRight){
        servoPos = servoPos + 200;
        if(servoPos > 6200) {servoPos = servoPos - 200; }
        if(servoPos > 1800 && servoPos < 6200){
          *AT91C_PWMC_CH1_CPRDR = 52500;
          *AT91C_PWMC_CH1_CDTYR = servoPos;
        }
    }
  }
}

void lightMenu(){
  int back = 0;
  int menuID = 0;
  lightScreen();
  servoForward();
  servoPos = 3937; // Forward
  while(back != 1){
    checkEverySec();
    checkWhatMode(); 
    key = readKeyValue();
    if(key > 0){menuID = key;}
    switch(menuID){
    case 1 :
      measureLight();
      Delay(400000);
      findLight();
      calculateSun();
      break;
    case 2 :
      break;
    case 9 :
      back = 1;
      break;
    }
  }
  setupMenu();
}

void lightScreen(){
  Reset_Screen();
  
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(15, 0);
  char *menu = "lightmenu";
  printTextV2(menu, 9);
  
  setAdressPointer(3, 3);
  printNumber(1);
  printDot();
  char *find = "  find light";
  printTextV2(find, 12);
  
  setAdressPointer(20, 3);
  char *degree = "degrees ";
  printTextV2(degree, 8);
  Write_Data_2_Display(0x1A);
  Write_Command_2_Display(0xC0);
  
  setAdressPointer(3, 5);
  printNumber(2);
  printDot();
  char *stop = "  stop";
  printTextV2(stop, 6);
  
  setAdressPointer(3, 7);
  printNumber(9);
  printDot();
  char *back = "  back";
  printTextV2(back, 6);
}

void calculateSun(){
  float degre;
  static float check = 0;
  if(servoPos<3937){
    degre = (servoPos-1800)/((3937-1800)/90)-90;
  }
  if(servoPos == 3937){
    degre = 0;
  }
  if(servoPos>3937){
    degre = (servoPos-3973)/((6200-3937)/90);
  }
  if(check != degre){
    printSunDegre(degre);
  }
  check = degre;
}


void printSunDegre(float x){
  setAdressPointer(31, 3);
  int number = (int)x;
  if(number<0){
    number = number * (-1);
    Write_Data_2_Display(0x0D);
    Write_Command_2_Display(0xC0);
  }
  else{
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
  }
  int first = number/10;
  int second = number%10;
  printNumber(first);
  printNumber(second);
}