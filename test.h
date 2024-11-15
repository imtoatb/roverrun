#ifndef TEST_H
#define TEST_H
#include "loc.h"

typedef struct robot {
t_localisation localisation;
char form;
} robot;

robot* createRobot(int x, int y);

void displayRobot(robot robot);

#endif