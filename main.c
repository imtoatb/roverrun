#include <stdio.h>
#include "map.h"
#include "test.c"

int main() {
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map created with dimensions %d x %d\n", map.y_max, map.x_max);
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%d ", map.soils[i][j]);
        }
        printf("\n");
    }
    // printf the costs, aligned left 5 digits
    for (int i = 0; i < map.y_max; i++)
    {
        for (int j = 0; j < map.x_max; j++)
        {
            printf("%-5d ", map.costs[i][j]);
        }
        printf("\n");
    }
    displayMap(map);
    return 0;
}



int robot_x = 1;
int robot_y = 1;

// Print the map with the robot's position marked as 'R'
for(int i = 0;i<map.y_max;i++){
        for (int j = 0; j < map.x_max; j++)
        {
            if (i == robot_y && j == robot_x)
            {
                printf("R ");
            }
            else
            {
                printf("%d ", map.soils[i][j]);
            }
        }
        printf("\n");
    }

