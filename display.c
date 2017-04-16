#include "at91sam3x8.h"
#include "Display.h"
#include "Main.h"
#define WR (0x4000) // pin 14
#define RD (0x8000) // pin 15
#define CE (0x10000) // pin 16
#define CD (0x20000) // pin 17
#define PC2_PC9 (0x3FC) // pin 2-9
#define OE (0x1000) // pin 12
#define DIR (0x2000) // pin 13




unsigned char Read_Status_Display(void){
  unsigned char Temp;
  *AT91C_PIOC_ODR = PC2_PC9; //make databus as input
  *AT91C_PIOC_SODR = DIR; //Set dir as input (74chip, 1 = input)
  *AT91C_PIOC_CODR = OE; //Clear/enable output (74chip 0 = enable)
  *AT91C_PIOC_SODR = CD; //Set C/D
  *AT91C_PIOC_CODR = CE; //Clear chip select display
  *AT91C_PIOC_CODR = RD; //Clear read display
  Delay(40);//Make a Delay
  Temp = *AT91C_PIOC_PDSR; //Read data bus and save it in temp
  *AT91C_PIOC_SODR = CE; //Set chip select display
  *AT91C_PIOC_SODR = RD; //Set read display
  *AT91C_PIOC_SODR = OE; //Disable output (74chip)
  *AT91C_PIOC_SODR = DIR; //Set dir as output (74chip)
  return Temp;
}

void Write_Command_2_Display(unsigned char Command){
  unsigned char command_data = Command;
  unsigned char Check = 0x00;
  unsigned char temp = 0;
  while(Check != 0xC){ // 0000 1100 |11|00
    temp = Read_Status_Display(); // väntar på klartecken från display
    Check = (temp & 0xC);
  }
  // fixa Wait until Read_Status_Display returns an OK 
  *AT91C_PIOC_CODR = PC2_PC9; //Clear databus
  *AT91C_PIOC_SODR = (command_data<<2); //Set Command to databus (10000010)
  *AT91C_PIOC_CODR = DIR; //Set dir as output (74chip)
  *AT91C_PIOC_CODR = OE; //Enable output (74chip)
  *AT91C_PIOC_OER = PC2_PC9; //Set databus as output
  *AT91C_PIOC_SODR = CD; //Set C/D signal High (1 = Command)
  *AT91C_PIOC_CODR = CE; //Clear chip select display
  *AT91C_PIOC_CODR = WR; // Clear write display
  Delay(20);//Make a Delay
  *AT91C_PIOC_SODR = CE; //Set chip enable display
  *AT91C_PIOC_SODR = WR; //Set write display
  *AT91C_PIOC_SODR = OE; //Disable output (74chip)
  *AT91C_PIOC_ODR = PC2_PC9; //Make databus as input 
  
} 

void Write_Data_2_Display(unsigned char Data){
 unsigned char cmd_data = Data; //input som 0x14, 0x15 osv
 unsigned char check_data = 0x00; // just to initialize
  
  while(check_data != 0xC) { // Så länge check inte är 1100
    check_data=Read_Status_Display();
    check_data=(check_data&0xC);
  }
  //Wait until Read_Status_Display returns an OK
  *AT91C_PIOC_CODR = PC2_PC9; //Clear databus
  *AT91C_PIOC_SODR = (cmd_data<<2); //Set write d//Set Data to databus
  *AT91C_PIOC_CODR = DIR; //Set dir as output (74chip)
  *AT91C_PIOC_CODR = OE; //Enable output (74chip)
  *AT91C_PIOC_OER = PC2_PC9; //Set databus as output
  *AT91C_PIOC_CODR = CD; //Clear C/D signal High (0 = Data)
  *AT91C_PIOC_CODR = CE; //Clear chip select display
  *AT91C_PIOC_CODR = WR; //Clear write display
  Delay(20);//Make a Delay
  *AT91C_PIOC_SODR = CE; //Set chip enable display
  *AT91C_PIOC_SODR = WR; //Set write display
  *AT91C_PIOC_SODR = OE; //Disable output (74chip)
  *AT91C_PIOC_ODR = PC2_PC9; //Make databus as input 
}


void Init_Display(void){
  *AT91C_PIOC_PER = 0xFF3FC; // pio enable register för pin PC2-PC9 OCH PC12-PC19
  *AT91C_PIOC_OER = 0xFF000;
  *AT91C_PIOD_PER = 0x1; // pio enable register för pin PD0
  *AT91C_PIOD_OER = 0x1; // output enable register
  *AT91C_PIOD_CODR = 0x1; // resetar skärmen
  Delay(40);
  *AT91C_PIOD_SODR = 0x1; // Set Output Data Register // Sätter 1 så inte resetar
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x40);//Set text home address
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x40);
  Write_Command_2_Display(0x42); //Set graphic home address
  Write_Data_2_Display(0x28); // 1e
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x41); // Set text area
  Write_Data_2_Display(0x28);  //1e
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x43); // Set graphic area
  Write_Command_2_Display(0x80); // text mode
  Write_Command_2_Display(0x94); // Text on graphic off
} 

void setAdressPointer(int x, int y){
  int cord = 0;
  cord = x + (40*y);
  int data2 = cord/255;
  int data1 = cord%255;
  if(data2>0){data1=data1-1;}
  if(data2>1){data1=data1-1;}
  Write_Data_2_Display(data1);
  Write_Data_2_Display(data2);
  Write_Command_2_Display(0x24);//Set text home address
}

void PrintTemp2Screen(){
  //--------------Transform to value--------
  int tens = 0;  //27.15
  tens = TrueTemp/10;
  int ones = (int) TrueTemp%10;
  float temp = TrueTemp;
  temp = temp-(10*tens+ones);
  temp = temp*100;
  int firstdec = 0;
  firstdec = temp/10; 
  int lastdec = 0;
  lastdec = (int)temp%10;
  // -----------done---------------------
  //------------print to display----------
  setAdressPointer(5, 0);
  printNumber(tens);
  printNumber(ones);
  printDot();
  printNumber(firstdec);
  printNumber(lastdec);
  char *c = "c";
  printTextV2(c, 1);
}

void Reset_Screen(){
  Write_Data_2_Display(0x00);
  Write_Data_2_Display(0x00);
  Write_Command_2_Display(0x24);//Set text home address
  
   for(int i=0; i<800; i++){
    Write_Data_2_Display(0x00);
    Write_Command_2_Display(0xC0);
  }
}

void printText(char str[], int size){
  char temp = 0;
  for(int i=0; i<size; i++){
    temp = str[i]-64;
    if(temp == 0xE0){temp = 0;}
    Write_Data_2_Display(temp);
    Write_Command_2_Display(0xC0);
  }
}


void printNumber(int x){
    Write_Data_2_Display(x+16);
    Write_Command_2_Display(0xC0);
}

void setupMenu(void){
  Reset_Screen();
  
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(15, 0);
  char mainMenu[9] = {'m','a','i','n',' ','m','e','n','u'};
  printText(mainMenu, 9);
  
  setAdressPointer(3, 3);
  Write_Data_2_Display(0x11); // sätter en 1a
  Write_Command_2_Display(0xC0);
  printDot();
  char measure[11] = {' ',' ','f','a','s','t',' ', 'm','o','d','e'};
  printText(measure, 11);
  
  setAdressPointer(3, 5);
  Write_Data_2_Display(0x12); // sätter en 2a
  Write_Command_2_Display(0xC0);
  printDot();
  char light[12] = {' ',' ','l','i','g','h','t' };
  printText(light, 7);
  
  setAdressPointer(3, 7);
  Write_Data_2_Display(0x13); // sätter en 3a
  Write_Command_2_Display(0xC0);
  printDot();
  char alarm[11] = {' ',' ','s','e','t',' ','a','l','a','r','m'};
  printText(alarm, 11);
  
  setAdressPointer(3, 9);
  Write_Data_2_Display(0x14); // sätter en 4a
  Write_Command_2_Display(0xC0);
  printDot();
  char logging[9] = {' ',' ','l','o','g','g','i','n','g'};
  printText(logging, 9);
  
  setAdressPointer(3, 11);
  Write_Data_2_Display(0x15); // sätter en 4a
  Write_Command_2_Display(0xC0);
  printDot();
  char *graph = "  graph";
  printTextV2(graph, 7);
}

void setAlarmScreen(void){
  Reset_Screen();
  
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(15, 0);
  char temp5[10] = {'a','l','a','r','m',' ','m','e','n','u'};
  printText(temp5, 10);
  
  setAdressPointer(3, 3);
  Write_Data_2_Display(0x11); // sätter en 1a
  Write_Command_2_Display(0xC0);
  printDot();
  char temp4[17] = {' ',' ','s','e','t',' ',' u', 'p','p','e','r',' ','l','i','m','i','t'};
  printText(temp4, 17);
  
  setAdressPointer(3, 5);
  Write_Data_2_Display(0x12); // sätter en 2a
  Write_Command_2_Display(0xC0);
  printDot();
  char lowerlimit[17] = {' ',' ','s','e','t',' ',' l', 'o','w','e','r',' ','l','i','m','i','t'};
  printText(lowerlimit, 17);
  
  setAdressPointer(3, 7);
  Write_Data_2_Display(0x19); // sätter en 9a
  Write_Command_2_Display(0xC0);
  printDot();
  char back[6] = {' ',' ','b','a','c','k'};
  printText(back, 6);
}

void loggingScreen(){
  Reset_Screen();
  
  setAdressPointer(0, 0);
  char temprature[4] = {'t','e','m','p'};
  printText(temprature, 4);
  
  setAdressPointer(15, 0);
  char loggingmenu[12] = {'l','o','g','g','i','n','g',' ','m','e','n','u'};
  printText(loggingmenu, 12);
  
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
  printNumber(4);
  printDot();
  char *test = "  test";
  printTextV2(test, 6);
  
  setAdressPointer(3, 11);
  Write_Data_2_Display(0x15); // 5a
  Write_Command_2_Display(0xC0);
  printDot();
  //char *speed = "  set loggging speed " ; 
  char *perSek = "  times per min ";
  printTextV2(perSek, 16);
  printNumber(1);
  Write_Data_2_Display(0x0D); 
  Write_Command_2_Display(0xC0);
  printNumber(9);
  
  setAdressPointer(3, 13);
  Write_Data_2_Display(0x19); // sätter en 9a
  Write_Command_2_Display(0xC0);
  printDot();
  char back[6] = {' ',' ','b','a','c','k'};
  printText(back, 6);
  
  setAdressPointer(20, 3);
  char *mon = "mon";
  printTextV2(mon, 3);
  setAdressPointer(20, 4);
  char *tue = "tue";
  printTextV2(tue, 3);
  setAdressPointer(20, 5);
  char *wed = "wed";
  printTextV2(wed, 3);
  setAdressPointer(20, 6);
  char *thu = "thu";
  printTextV2(thu, 3);
  setAdressPointer(20, 7);
  char *fri = "fri";
  printTextV2(fri, 3);
  setAdressPointer(20, 8);
  char *sat = "sat";
  printTextV2(sat, 3);
  setAdressPointer(20, 9);
  char *sun = "sun";
  printTextV2(sun, 3);
}

void printDot(){
  Write_Data_2_Display(0x0E);
  Write_Command_2_Display(0xC0);
}

void printTextV2(char *temp, int size){
  char ch = 0;
  for(int i=0; i<size; i++){
    ch = temp[i]-64;
    if(ch == 0xE0){ch = 0;}
    Write_Data_2_Display(ch);
    Write_Command_2_Display(0xC0);
  }
}

void printFloatWith2Dec(volatile double a){
  double tempo = a;
  int tens = (int)tempo/10;
  int ones = (int)tempo%10;
  tempo = tempo -(tens*10);
  tempo = tempo - ones;
  tempo = tempo*100;
  int first = (int)tempo/10;
  int second =(int)tempo%10;
  printNumber(tens);
  printNumber(ones);
  printDot();
  printNumber(first);
  printNumber(second);
  char *c = "c";
  printTextV2(c, 1);
}