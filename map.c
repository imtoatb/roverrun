//
// Created by flasque on 19/10/2024.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "map.h"
#include "loc.h"
#include "queue.h"
#include "moves.h"

#define COST_WARNING_THRESHOLD 10000

/* prototypes of local functions */
/* local functions are used only in this file, as helper functions */

/**
 * @brief :  function to get the position of the base station
 * @param map : the map
 * @return : the position of the base station
 */
t_position getBaseStationPosition(t_map);

/**
 * @brief : function to calculate costs of the map  from the base station
 * @param map : the map
 * @return none
 */
void calculateCosts(t_map);

/**
 * @brief : function to remove 'false' crevasses costs from the costs array
 * @param map : the map
 * @return none
 */
void removeFalseCrevasses(t_map);

/* definition of local functions */

t_position getBaseStationPosition(t_map map)
{
    t_position pos;
    int i = 0;
    int found = 0;
    while (i < map.y_max && !found)
    {
        int j = 0;
        while (j < map.x_max && !found)
        {
            if (map.soils[i][j] == BASE_STATION)
            {
                pos.x = j;
                pos.y = i;
                found = 1;
            }
            j++;
        }
        i++;
    }
    // if the base station is not found, we exit the program
    if (!found)
    {
        fprintf(stderr, "Error: base station not found in the map\n");
        exit(1);
    }
    return pos;
}

void removeFalseCrevasses(t_map map)
{
    // step 1 : find the minimal cost > 10000 in the costs array where the soil is not a crevasse
    int over=0;
    int imin, jmin;
    while (!over)
    {
        int min_cost = COST_UNDEF;
        imin = map.y_max;
        jmin = map.x_max;
        for (int i=0; i<map.y_max; i++)
        {
            for (int j=0; j<map.x_max; j++)
            {
                if (map.soils[i][j] != CREVASSE && map.costs[i][j] > 10000 && map.costs[i][j] < min_cost)
                {
                    min_cost = map.costs[i][j];
                    imin = i;
                    jmin = j;
                }
            }
        }
        if (imin < map.y_max && jmin < map.x_max)
        {
            // step 2 : calculate the costs of the neighbours of the position
            t_position pos;
            pos.x = jmin;
            pos.y = imin;
            t_position lp, rp, up, dp;
            lp = LEFT(pos);
            rp = RIGHT(pos);
            up = UP(pos);
            dp = DOWN(pos);
            int min_neighbour = COST_UNDEF;
            if (isValidLocalisation(lp, map.x_max, map.y_max))
            {
                min_neighbour = (map.costs[lp.y][lp.x] < min_neighbour) ? map.costs[lp.y][lp.x] : min_neighbour;
            }
            if (isValidLocalisation(rp, map.x_max, map.y_max))
            {
                min_neighbour = (map.costs[rp.y][rp.x] < min_neighbour) ? map.costs[rp.y][rp.x] : min_neighbour;
            }
            if (isValidLocalisation(up, map.x_max, map.y_max))
            {
                min_neighbour = (map.costs[up.y][up.x] < min_neighbour) ? map.costs[up.y][up.x] : min_neighbour;
            }
            if (isValidLocalisation(dp, map.x_max, map.y_max))
            {
                min_neighbour = (map.costs[dp.y][dp.x] < min_neighbour) ? map.costs[dp.y][dp.x] : min_neighbour;
            }
            int self_cost = _soil_cost[map.soils[imin][jmin]];
            map.costs[imin][jmin] = (min_neighbour + self_cost < map.costs[imin][jmin]) ? min_neighbour + self_cost : map.costs[imin][jmin];
        }
        else
        {
            over = 1;
        }
    }
}

void calculateCosts(t_map map)
{
    t_position baseStation = getBaseStationPosition(map);
    //create a queue to store the positions to visit
    t_queue queue = createQueue(map.x_max * map.y_max);
    //enqueue the base station
    enqueue(&queue, baseStation);
    // while the queue is not empty
    while (queue.first != queue.last)
    {
        // dequeue the position
        t_position pos = dequeue(&queue);
        // get its self cost
        int self_cost = _soil_cost[map.soils[pos.y][pos.x]];
        // get ts neighbours
        t_position lp, rp, up, dp;
        lp = LEFT(pos);
        rp = RIGHT(pos);
        up = UP(pos);
        dp = DOWN(pos);
        // get the mimimum cost of the neighbours
        int min_cost = COST_UNDEF;
        if (isValidLocalisation(lp, map.x_max, map.y_max))
        {
            min_cost = (map.costs[lp.y][lp.x] < min_cost) ? map.costs[lp.y][lp.x] : min_cost;
        }
        if (isValidLocalisation(rp, map.x_max, map.y_max))
        {
            min_cost = (map.costs[rp.y][rp.x] < min_cost) ? map.costs[rp.y][rp.x] : min_cost;
        }
        if (isValidLocalisation(up, map.x_max, map.y_max))
        {
            min_cost = (map.costs[up.y][up.x] < min_cost) ? map.costs[up.y][up.x] : min_cost;
        }
        if (isValidLocalisation(dp, map.x_max, map.y_max))
        {
            min_cost = (map.costs[dp.y][dp.x] < min_cost) ? map.costs[dp.y][dp.x] : min_cost;
        }
        // the cost of the current position is the minimum cost of the neighbours + 1 or 0 if the soil is a base station
        map.costs[pos.y][pos.x] = (map.soils[pos.y][pos.x] == BASE_STATION) ? 0 : min_cost + self_cost;
        // enqueue the neighbours if they are not visited yet
        if (isValidLocalisation(lp, map.x_max, map.y_max) && map.costs[lp.y][lp.x] == COST_UNDEF)
        {
            // mark as visited - change the cost to 65534
            map.costs[lp.y][lp.x] = COST_UNDEF-1;
            enqueue(&queue, lp);
        }
        if (isValidLocalisation(rp, map.x_max, map.y_max) && map.costs[rp.y][rp.x] == COST_UNDEF)
        {
            map.costs[rp.y][rp.x] = COST_UNDEF-1;
            enqueue(&queue, rp);
        }
        if (isValidLocalisation(up, map.x_max, map.y_max) && map.costs[up.y][up.x] == COST_UNDEF)
        {
            map.costs[up.y][up.x] = COST_UNDEF-1;
            enqueue(&queue, up);
        }
        if (isValidLocalisation(dp, map.x_max, map.y_max) && map.costs[dp.y][dp.x] == COST_UNDEF)
        {
            map.costs[dp.y][dp.x] = COST_UNDEF-1;
            enqueue(&queue, dp);
        }
    }


    return;
}
/* definition of exported functions */

t_map createMapFromFile(char *filename)
{
    /* rules for the file :
     * - the first line contains the number of lines : y dimension (int)
     * - the second line contains the number of columns : x dimension (int)
     * - the next lines contain the map values (int) separated by spaces : one line = one row
     * - the values are the following : 0 = BASE_STATION, 1 = PLAIN, 2 = ERG, 3 = REG, 4 = CREVASSE
     */

    t_map map;
    int xdim, ydim;     // dimensions of the map
    char buffer[100];   // buffer for reading the file line by line

    FILE *file = fopen(filename,"rt");
    if (file == NULL)
    {
        fprintf(stderr, "Error: cannot open file %s\n", filename);
        exit(1);
    }
    fscanf(file, "%d", &ydim);
    fscanf(file, "%d", &xdim);
    map.x_max = xdim;
    map.y_max = ydim;
    map.soils = (t_soil **)malloc(ydim * sizeof(t_soil *));
    for (int i = 0; i < ydim; i++)
    {
        map.soils[i] = (t_soil *)malloc(xdim * sizeof(t_soil));
    }
    map.costs = (int **)malloc(ydim * sizeof(int *));
    for (int i = 0; i < ydim; i++)
    {
        map.costs[i] = (int *)malloc(xdim * sizeof(int));
    }
    for (int i = 0; i < ydim; i++)
    {

        // parse the line to get the values : 0 = BASE_STATION, 1 = PLAIN, 2 = ERG, 3 = REG, 4 = CREVASSE
        // values are separated by spaces, so we use sscanf with %d to get the values
        for (int j = 0; j < xdim; j++)
        {
            int value;
            fscanf(file, "%d", &value);
            map.soils[i][j] = value;
            // cost is 0 for BASE_STATION, 65535 for other soils
            map.costs[i][j] = (value == BASE_STATION) ? 0 : COST_UNDEF;
        }

    }
    fclose(file);
    calculateCosts(map);
    removeFalseCrevasses(map);
    return map;
}

t_map createTrainingMap()
{
    return createMapFromFile("..\\maps\\training.map");
}

void displayMap(t_map map)
{
    /** the rules for display are :
     * display all soils with 3x3 characters
     * characters are : B for base station, '-' for plain, '~' for erg, '^' for reg, ' ' for crevasse
     */
    for (int i = 0; i < map.y_max; i++)
    {
        for (int rep = 0; rep < 3; rep++)
        {
            for (int j = 0; j < map.x_max; j++)
            {
                char c[4];
                switch (map.soils[i][j])
                {
                    case BASE_STATION:
                        if (rep==1)
                        {
                            strcpy(c, " B ");
                        }
                        else
                        {
                            strcpy(c, "   ");
                        }
                        break;
                    case PLAIN:
                        strcpy(c, "---");
                        break;
                    case ERG:
                        strcpy(c, "~~~");
                        break;
                    case REG:
                        strcpy(c, "^^^");
                        break;
                    case CREVASSE:
                        sprintf(c, "%c%c%c",219,219,219);
                        break;
                    default:
                        strcpy(c, "???");
                        break;
                }
                printf("%s", c);
            }
            printf("\n");
        }

    }
    return;
}


int getCost(t_map map, t_position pos)
{
    if (!isValidLocalisation(pos, map.x_max, map.y_max))
    {
        fprintf(stderr, "Error: Position (%d, %d) is out of bounds\n", pos.x, pos.y);
        return COST_UNDEF;
    }
    return map.costs[pos.y][pos.x];
}

/*
void displayMoveCosts(t_map map, t_localisation robot)
{
    t_position positions[12];
    int costs[12];
    const char *move_names[12] = {
        "NORTH", "SOUTH", "WEST", "EAST",
        "F_20_NORTH", "F_20_SOUTH", "F_20_WEST", "F_20_EAST",
        "F_30_NORTH", "F_30_SOUTH", "F_30_WEST", "F_30_EAST"
    };
    int is_valid_move[12];
    positions[0] = UP(robot.pos);
    positions[1] = DOWN(robot.pos);
    positions[2] = LEFT(robot.pos);
    positions[3] = RIGHT(robot.pos);
    positions[4] = UP(UP(robot.pos));
    positions[5] = DOWN(DOWN(robot.pos));
    positions[6] = LEFT(LEFT(robot.pos));
    positions[7] = RIGHT(RIGHT(robot.pos));
    positions[8] = UP(UP(UP(robot.pos)));
    positions[9] = DOWN(DOWN(DOWN(robot.pos)));
    positions[10] = LEFT(LEFT(LEFT(robot.pos)));
    positions[11] = RIGHT(RIGHT(RIGHT(robot.pos)));
    for (int i = 0; i < 12; i++) {
        is_valid_move[i] = isValidLocalisation(positions[i], map.x_max, map.y_max);
        if (is_valid_move[i]) {
            costs[i] = getCost(map, positions[i]);
        } else {
            costs[i] = COST_UNDEF;
        }
    }
    printf("Move Costs for Robot at (%d, %d) facing %s:\n", robot.pos.x, robot.pos.y, orientationToString(robot.ori));
    for (int i = 0; i < 12; i++) {
        if (is_valid_move[i]) {
            printf("%-10s: Valid, Cost: %-5d\n", move_names[i], costs[i]);
        } else {
            printf("%-10s: Invalid Move\n", move_names[i]);
        }
    }
}
*/

void checkCurrentCost(t_map map, t_localisation robot) {
    int current_cost = getCost(map, robot.pos);
    if (current_cost > COST_WARNING_THRESHOLD) {
        printf("Game Over: The robot is on a cell with a cost of %d, which exceeds the limit of %d.\n", current_cost, COST_WARNING_THRESHOLD);
        exit(1);
    }
}

void alertMoveCost(int move_cost, const char* move_name) {
    if (move_cost > COST_WARNING_THRESHOLD) {
        printf("Warning: Moving %s will cost %d, which could be dangerous for the robot.\n", move_name, move_cost);
    }
}

void checkValidMove(t_map map, t_localisation robot) {
    t_position positions[12];
    int costs[12];
    const char *move_names[12] = {
        "NORTH", "SOUTH", "WEST", "EAST",
        "F_20_NORTH", "F_20_SOUTH", "F_20_WEST", "F_20_EAST",
        "F_30_NORTH", "F_30_SOUTH", "F_30_WEST", "F_30_EAST"
    };

    positions[0] = UP(robot.pos);
    positions[1] = DOWN(robot.pos);
    positions[2] = LEFT(robot.pos);
    positions[3] = RIGHT(robot.pos);
    positions[4] = UP(UP(robot.pos));
    positions[5] = DOWN(DOWN(robot.pos));
    positions[6] = LEFT(LEFT(robot.pos));
    positions[7] = RIGHT(RIGHT(robot.pos));
    positions[8] = UP(UP(UP(robot.pos)));
    positions[9] = DOWN(DOWN(DOWN(robot.pos)));
    positions[10] = LEFT(LEFT(LEFT(robot.pos)));
    positions[11] = RIGHT(RIGHT(RIGHT(robot.pos)));


for (int i = 0; i < 12; i++) {
        if (isValidLocalisation(positions[i], map.x_max, map.y_max)) {
            int cost = getCost(map, positions[i]);

            // Si le coût est trop élevé (crevasse) ou la position contient une crevasse
            if (cost > 9999 || map.soils[positions[i].y][positions[i].x] == CREVASSE) {
                printf("%-10s: Invalid, You'll fall into a crevasse if you do this move\n", move_names[i]);
            } else if (i >= 4 && i < 8) {  // F_20 moves (positions 4-7)
                t_position prev_position = (i == 4) ? UP(robot.pos) :
                                           (i == 5) ? DOWN(robot.pos) :
                                           (i == 6) ? LEFT(robot.pos) : RIGHT(robot.pos);

                if (getCost(map, prev_position) > 9999 || map.soils[prev_position.y][prev_position.x] == CREVASSE) {
                    printf("%-10s: Invalid, You'll fall into a crevasse if you do this move\n", move_names[i]);
                } else {
                    printf("%-10s: Valid, Cost: %-5d\n", move_names[i], cost);
                }
            } else if (i >= 8 && i < 12) { // F_30 moves (positions 8-11)
                t_position prev_position1, prev_position2;
                if (i == 8) {
                    prev_position1 = UP(UP(robot.pos));
                    prev_position2 = UP(robot.pos);
                } else if (i == 9) {
                    prev_position1 = DOWN(DOWN(robot.pos));
                    prev_position2 = DOWN(robot.pos);
                } else if (i == 10) {
                    prev_position1 = LEFT(LEFT(robot.pos));
                    prev_position2 = LEFT(robot.pos);
                } else {
                    prev_position1 = RIGHT(RIGHT(robot.pos));
                    prev_position2 = RIGHT(robot.pos);
                }

                if (getCost(map, prev_position1) > 9999 || map.soils[prev_position1.y][prev_position1.x] == CREVASSE ||
                    getCost(map, prev_position2) > 9999 || map.soils[prev_position2.y][prev_position2.x] == CREVASSE) {
                    printf("%-10s: Invalid, You'll fall into a crevasse if you do this move\n", move_names[i]);
                } else {
                    printf("%-10s: Valid, Cost: %-5d\n", move_names[i], cost);
                }
            } else {
                printf("%-10s: Valid, Cost: %-5d\n", move_names[i], cost);
            }
        } else {
            printf("%-10s: Invalid Move, out of bound\n", move_names[i]);
        }
    }
}