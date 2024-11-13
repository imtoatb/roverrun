#include "main.c"
#include "test.h"
#include "loc.c"
#include "loc.h"


#define x_max 7
#define y_max 6


void initMap(char map[x_max][y_max]){

    t_map map = createMapFromFile("..\\maps\\example1.map");
}

void updateMap(char map[x_max][y_max], t_position robot_pos){

    if (isValidLocalisation(robot_pos, x_max, y_max)){
        map[robot_pos.y][robot_pos.x] = 'R';  // 'R' représente la position du robot

    }
}