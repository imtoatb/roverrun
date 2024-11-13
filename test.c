#include "main.c"
#include "test.h"
#include "loc.c"
#include "loc.h"


#define x_max 7
#define y_max 6


//To visualize the robot
char robot(t_position loc, int x_max, int y_max) {
    while(isValidLocalisation(loc, x_max, y_max) == 0) {
        printf("The robot is outside the map. OVER");
    }
    return "good";
}

void updateMap(char map[x_max][y_max], t_position robot_pos) {
    // Vérifier que la position du robot est valide
    if (isValidLocalisation(robot_pos, x_max, y_max)) {
        map[robot_pos.y][robot_pos.x] = 'R';  // 'R' représente la position du robot
    }
}