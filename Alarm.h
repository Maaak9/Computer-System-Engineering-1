#ifndef ALARM_H
#define ALARM_H

extern int upperAlarm;
extern int lowerAlarm;
extern int key;
extern int MenuID;
extern float TrueTemp;
extern int count1sec;
extern int fastMode;

void checkAlarm();
void setAlarm();
void setUpperLimit();
void setLowerLimit();
//void checkAlarm();
int Roundfloat(float myfloat);
void checkFastMode();

#endif