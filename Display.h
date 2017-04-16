#ifndef DISPLAY_H
#define DISPLAY_H

void Delay(int Value);

extern float TrueTemp;

unsigned char Read_Status_Display(void); 
void Write_Data_2_Display(unsigned char Data);
void Init_Display(void);
void Reset_Screen();
void Master_UI_Ten_out_of_Ten();
void PrintTemp2Screen();
void printText(char str[], int size);
void setupMenu(void);
void setAdressPointer(int x, int y);
void setAlarmScreen(void);
void printNumber(int x);
void printDot();
void loggingScreen();
void printTextV2(char *temp, int size);
void printFloatWith2Dec(volatile double a);

#endif