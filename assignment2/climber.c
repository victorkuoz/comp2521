// Implementation of boulder climbing algorithms

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "climber.h"
#include "Wall.h"

struct path findShortestPath(Wall w, int reach, Colour colour) {
    // TODO - Task 1
    struct path p = {0, NULL};
    return p;
}

struct path findMinEnergyPath(Wall w, int reach, int energyCosts[NUM_COLOURS]) {
    // TODO - Task 2
    struct path p = {0, NULL};
    return p;
}

struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    // TODO - Task 3
    struct path p = {0, NULL};
    return p;
}

