#include "main.c"
#include "loc.c"
#include "loc.h"


#define x_max 6
#define y_max 6


void initMap(char map[x_max][y_max]){
    for (int y = 0; y < x_max; y++) {
        for (int x = 0; x < y_max; x++) {
            map[y][x] = '.';
        }
    }
}

void updateMap(char map[x_max][y_max], t_position robot_pos) {
    if (isValidLocalisation(robot_pos, x_max, y_max)){
        map[robot_pos.y][robot_pos.x] = 'R';

    }
}

void printMap(char map[x_max][y_max]){
    for (int y = 0; y < x_max; y++) {
        for (int x = 0; x < y_max; x++) {
            printf("%c ", map[y][x]);
        }
        printf("\n");
    }
}

    
int main2(){
    char map[x_max][y_max];
    t_localisation robot = loc_init(3, 3, 0);

    initMap(map);
    updateMap(map, robot.pos);
    printMap(map);

    return 0;
}