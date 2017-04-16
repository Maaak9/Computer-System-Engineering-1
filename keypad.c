#include "at91sam3x8.h"
#include "Keypad.h"
#include "Main.h"
#define PC2_PC5 (0x0000003C)
#define PC7_PC9 (0x00000380)

void initKeypad(){
  *AT91C_PIOC_PER = 0x000003BC; // pio enable register för pin PC2-PC5 OCH PC7-PC9
  *AT91C_PIOD_PER = 0x00000004; // pio enable register för pin PD2
  *AT91C_PIOD_OER = 0x00000004; // output enable register, skickar ut en nolla för att starta 74hc245
  *AT91C_PIOD_CODR = 0x4; // sätter 0 på OE
  *AT91C_PIOC_OER = PC7_PC9; // output enable register
  *AT91C_PIOC_SODR = PC7_PC9; // Set Output Data Register
  *AT91C_PIOC_PPUDR = 0x00000380;
}

int readKeyValue(){
  if(checkKey > 100){
  *AT91C_PIOD_CODR = 0x4;
  *AT91C_PIOC_ODR = PC2_PC5;
  *AT91C_PIOC_OER = PC7_PC9;
  *AT91C_PIOC_SODR = PC7_PC9;
  static int checker = 0;
  int column[3] = {256, 512, 128}; // pin 8, pin 9, pin 7
  int row[4] = {32, 4, 8, 16}; // pin6,  pin 2, pin 3, pin 4
  int temp = 0;
  int value=-1;
  Delay(40);
  *AT91C_PIOC_SODR = PC7_PC9;
  for(int i=0; i<3; i++){
    *AT91C_PIOC_CODR = column[i];
    for(int k=0; k<4; k++){
        temp = *AT91C_PIOC_PDSR & row[k]; // MASKA UT PIN PC2-PC5
        if(temp == 0){
          value = ((k*3)+(i+1));
        }
      }
    *AT91C_PIOC_SODR = PC7_PC9;
    }
    if(value == 11){value = 0;}
/*    if(value != -1){
      printf("\n%d \n" , value);
      Delay(50);
    }*/
  *AT91C_PIOC_ODR = PC7_PC9;
  *AT91C_PIOC_ODR = PC2_PC5;
  *AT91C_PIOD_SODR = 0x4;
  if(checker != value){ 
    checker = value;
    return value;
  }
  checkKey=0;
  }
  return -1;
}