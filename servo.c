#include "at91sam3x8.h"
#include "Servo.h"
// Sida 48 id36 PWM
// Servo sida 987 PWM
void initPWM(void){
  *AT91C_PMC_PCER1 = 0x10;
  *AT91C_PIOB_PDR = 1<<17;
  *AT91C_PIOB_ABMR = *AT91C_PIOB_ABMR | 0x20000;
  *AT91C_PWMC_ENA = 0x2; //enable pwm channel chid1
  *AT91C_PWMC_CH1_CMR = 0x5;
  // MCK/32 ---- Period left aligned,  starts at low level, 
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 2625;
}

void servoLeft(void){
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 6200;
}

void servoRight(void){
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 1800;
}

void servoForward(void){
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 3937;
}

void servoTest(void){
  *AT91C_PWMC_CH1_CPRDR = 52500;
  *AT91C_PWMC_CH1_CDTYR = 1650;
}