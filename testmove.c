#include <stdio.h>
#include "moves.h"
#include "map.h"
#include "loc.h"

// Stage 1 -To test the different movements

void test_movement(t_map map) {

    t_localisation robot_loc = loc_init(map.x_max / 2, map.y_max / 2, NORTH);       // Robot in the center facing NORTH
    printf("Initial Position: (%d, %d), Orientation: %d\n", robot_loc.pos.x, robot_loc.pos.y, robot_loc.ori);

    // Test
    t_move moves[] = {F_10, T_RIGHT, F_20, T_LEFT, F_10, B_10, U_TURN, F_30};
    int num_moves = sizeof(moves) / sizeof(moves[0]);


    for (int i = 0; i < num_moves; i++) {
        t_move current_move = moves[i];
        printf("\nExecuting move: %s\n", getMoveAsString(current_move));

        updateLocalisation(&robot_loc, current_move);       //Update


        if (isValidLocalisation(robot_loc.pos, map.x_max, map.y_max)) { //Check if in the map
            printf("New Position: (%d, %d), Orientation: %d\n",
                   robot_loc.pos.x, robot_loc.pos.y, robot_loc.ori);
        } else {
            printf("Invalid position: (%d, %d). Out of bounds. Reverting to last valid position.\n",
                   robot_loc.pos.x, robot_loc.pos.y);
            // Move the robot back to a valid position (for testing purposes, resetting to center)
            robot_loc = loc_init(map.x_max / 2, map.y_max / 2, robot_loc.ori);
        }
    }
}

int main() {
    t_map map = createMapFromFile("..\\maps\\example1.map");
    printf("Map loaded with dimensions %d x %d\n", map.y_max, map.x_max);

    test_movement(map);

    return 0;
}

