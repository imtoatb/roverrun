#include "main.c"
#include "test.h"
#include "loc.c"
#include "loc.h"


char robot(t_position loc, int x_max, int y_max) {
    while(isValidLocalisation(loc, x_max, y_max) == 0) {
        printf("The robot is outside the map. OVER");
    }
    return "good";
}