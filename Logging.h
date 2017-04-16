#ifndef LOGGING_H
#define LOGGING_H

extern int key;
extern float TrueTemp;
extern float logging[10080];
extern float averages[];
extern float maxs[];
extern float mins[];
extern int count1sec;
extern int MenuID;
extern int fastMode;

void loggingMenu();
float average(float dVec[], int start, int end);
void logg2Array(float temp);
float minima(float dVec[], int start, int end);
float maxima(float dVec[], int start, int end);
void printLogg(float arr[]);
void testFullArray();
//float average1(linkedlist *lista, int nVals);


#endif