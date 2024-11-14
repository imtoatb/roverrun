/**
#ifndef TEST_H
#define TEST_H

#include "loc.h"
#include "test.c"

#define x_max 6
#define y_max 6

void initMap(char map[x_max][y_max]);

void updateMap(char map[x_max][y_max], t_position robot_pos);


void printMap(char map[x_max][y_max]);

#endif
*/