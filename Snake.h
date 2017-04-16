#ifndef SNAKE_H
#define SNAKE_H

extern int key;
extern int count1sec;
extern struct linkedlist *listan;


void snakeGame();
void setUpGameScreen();
void printSnake(linkedlist *lista);
void startGame();
int checkColide(int checkX, int checkY);
void RestartOrBack();

#endif