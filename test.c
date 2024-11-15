
#include "main.c"
#include "loc.c"
#include "loc.h"
#include "test.h"
#include <stdlib.h>

robot* allocateRobot() {
    robot* new_robot = (robot*) calloc(1, sizeof(robot));
    return new_robot;
}

robot* createrobot(int x, int y){
    robot* new_robot = allocateRobot();
    new_robot->localisation = loc_init(x, y, 0);
    new_robot->form = 'R';
    return new_robot;
}

void displayrobot(robot robot) {
    printf("%c", robot.form);
}

