#ifndef LIGHT_H
#define LIGHT_H

extern double lightValueLeft;
extern double lightValueRight;
extern int servoPos;
extern int count1sec;
extern int key;

void initLightSensor(void);
void measureLight(void);
void findLight(void);
void lightMenu();
void lightScreen();
void calculateSun();
void printSunDegre(float x);

#endif