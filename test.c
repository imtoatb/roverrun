#include <stdio.h>
#include <assert.h>
#include "test.h"
#include <stdlib.h>
#include "main.c"
#include "map.h"

char* testmouvement() {


    return "mouvement";
}

int random(int min, int max) {
    return min + rand() % (max - min);
}