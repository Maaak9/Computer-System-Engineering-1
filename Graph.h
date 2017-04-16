#ifndef GRAPH_H
#define GRAPH_H

extern float averages[7];
extern float maxs[7];
extern float mins[7];
extern int key;

void printGraph(float array[7]);
void printTempLine(int x, int y);
void clearGraphicScreen();
void setGraphicAPointer(int x, int y);
int Round(float myfloat);
int celciusToCord(int temp);
void printLine(int x0,int y0,int x1,int y1);
void setGraphScreen();
void setGraphScreenMenu();


#endif