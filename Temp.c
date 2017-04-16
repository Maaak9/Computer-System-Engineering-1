#include "at91sam3x8.h"
#include "system_sam3x.h"
#include "Temp.h"
#include "Main.h"

void initTemp(void){
  *AT91C_PMC_PCER = 1<<12; //pin 12 PIOB startar piob
  *AT91C_PMC_PCER = 1<<27; // pin 27 TC0
  
  //init
  //Don’t forget the PMC        -check
  *AT91C_TC0_CMR =  0xFFFFFFF8 &  *AT91C_TC0_CMR; //Select Timer_Clock1 as TCCLK  
   //counter is incremented on rising edge of the clock.
  *AT91C_TC0_CCR = 0x1; // enable counter
  *AT91C_TC0_CCR = 0x4; //Enable counter and make a sw_reset in TC0_CCR0
  //  a software trigger is performed: the counter is reset and the clock is started.
  *AT91C_TC0_CMR =  (*AT91C_TC0_CMR | 0x20000); //Load counter to A when TIOA falling in (TC0_CMR0)
  *AT91C_TC0_CMR = (*AT91C_TC0_CMR | 0x40000);//Load counter to B when TIOA rising in (TC0_CMR0)
  *AT91C_PIOB_SODR = 0x2000000;                  
  *AT91C_PIOB_OER = 0x2000000;
  *AT91C_PIOB_PER = 0x2000000;//Enable the pin
  //Enable the interrupt (NVIC) with the inline declared function
  NVIC_ClearPendingIRQ(TC0_IRQn);
  NVIC_SetPriority(TC0_IRQn, 0);
  NVIC_EnableIRQ(TC0_IRQn);
}

void Temp_Mesurement(void){ 
            //Enable interrupt TC_IER_LDRBS
  //• Start a mesurement
   *AT91C_TC0_IER = 0x40;
   *AT91C_PIOB_OER = 1<<25;
   *AT91C_PIOB_CODR = 1<<25;
   Delay(840000);
   *AT91C_PIOB_SODR = 1<<25;
   Delay(420000);
  // == Startpulse ==
  *AT91C_PIOB_CODR = 1<<25;     // Disable before startpulse
  Delay(210);  
  *AT91C_PIOB_SODR = 1<<25;
  *AT91C_PIOB_ODR = 1<<25;      // Disable output tempsensor
  Delay(25);
  *AT91C_TC0_CCR = 0x4;         // reset timer clock
  
  float test = (Temp_Timer/210) - 273.15;
  if(test<50.0 & test > 0.0){
  TrueTemp = (Temp_Timer/210) - 273.15;
  //printf("\n %f \n", TrueTemp);
  }
}