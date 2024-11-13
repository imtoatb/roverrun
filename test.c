#include "main.c"
#include "test.h"
#include "loc.c"
#include "loc.h"

//To visualize the robot
char robot(t_position loc, int x_max, int y_max) {
    while(isValidLocalisation(loc, x_max, y_max) == 0) {
        printf("The robot is outside the map. OVER");
    }
    return "good";
}

void updateMap(char map[x_max][y_max],t_position robot_pos) {

}