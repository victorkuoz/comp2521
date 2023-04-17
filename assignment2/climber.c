// Implementation of boulder climbing algorithms

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "climber.h"
#include "Wall.h"

typedef struct rock Rock;

struct path findShortestPath(Wall w, int reach, Colour colour) {
    // TODO - Task 1
    struct path p = {0, NULL};

    int height = WallHeight(w);
    int width = WallWidth(w);

    Rock *rocks = calloc(height * width, sizeof(Rock));
    int numOfRocks = WallGetAllRocks(w, rocks);

    bool **explored = calloc(height, sizeof(bool*));
    for (int r = 0; r < height; r++)
        explored[r] = calloc(width, sizeof(bool));

    Rock ***prevRock = calloc(height, sizeof(Rock**));
    for (int r = 0; r < height; r++)
        prevRock[r] = calloc(width, sizeof(Rock*));

    int bfsSize = 0;
    Rock **bfs = calloc(height * width, sizeof(Rock*));
    for (int i = 0; i < numOfRocks; i++) {
        if (rocks[i].row <= reach && rocks[i].colour == colour) {
            bfs[bfsSize++] = &rocks[i];
            explored[rocks[i].row][rocks[i].col] = true;
        }
    }

    int bfsIdx = 0, squ = reach * reach;
    while (bfsIdx < bfsSize) {
        Rock *cur = bfs[bfsIdx++];

        if (height <= cur->row + reach) {
            Rock **revPath = calloc(height * width, sizeof(Rock));
            while (cur) {
                revPath[p.numRocks++] = cur;
                cur = prevRock[cur->row][cur->col];
            }

            p.rocks = calloc(p.numRocks, sizeof(Rock));
            for (int i = 0, j = p.numRocks - 1; i < p.numRocks; i++, j--)
                p.rocks[i] = *revPath[j];
            free(revPath);
            break;
        }

        for (int i = 0; i < numOfRocks; i++) {
            int r = rocks[i].row, c = rocks[i].col;
            if (rocks[i].colour != colour || explored[r][c])
                continue;

            int dr = abs(r - cur->row), dc = abs(c - cur->col);

            if (squ < dr*dr + dc*dc)
                continue;

            bfs[bfsSize++] = &rocks[i];
            explored[r][c] = true;
            prevRock[r][c] = cur;
        }
    }

    free(rocks);

    for (int r = 0; r < height; r++)
        free(explored[r]);
    free(explored);

    for (int r = 0; r < height; r++)
        free(prevRock[r]);
    free(prevRock);

    free(bfs);

    return p;
}

typedef struct {
    int energy;
    Rock *rock;
} EnergyNode;

struct path findMinEnergyPath(Wall w, int reach, int energyCosts[NUM_COLOURS]) {
    // TODO - Task 2
    struct path p = {0, NULL};

    int height = WallHeight(w);
    int width = WallWidth(w);

    Rock *rocks = calloc(height * width, sizeof(Rock));
    int numOfRocks = WallGetAllRocks(w, rocks);

    bool **explored = calloc(height, sizeof(bool*));
    for (int r = 0; r < height; r++)
        explored[r] = calloc(width, sizeof(bool));

    Rock ***prevRock = calloc(height, sizeof(Rock**));
    for (int r = 0; r < height; r++)
        prevRock[r] = calloc(width, sizeof(Rock*));

    int pqInsertIdx = 1;
    EnergyNode *que = calloc(height * width, sizeof(EnergyNode));
    EnergyNode *pq = calloc(height * width, sizeof(EnergyNode));

    for (int i = 0; i < numOfRocks; i++) {
        if (rocks[i].row <= reach) {
            int curIdx = pqInsertIdx++;
            que[curIdx] = (EnergyNode) {.energy = energyCosts[rocks[i].colour], &rocks[i]};
            pq[curIdx] = (EnergyNode) {.energy = energyCosts[rocks[i].colour], &rocks[i]};

            while (1 < curIdx) {
                if (pq[curIdx >> 1].energy <= pq[curIdx].energy)
                    break;

                EnergyNode node = pq[curIdx >> 1];
                pq[curIdx >> 1] = pq[curIdx];
                pq[curIdx] = node;
                curIdx >>= 1;
            }
        }
    }

    // for (int i = 1; i < pqInsertIdx; i++) {
    //     printf("%d %d %d %d\n", que[i].energy, que[i].rock->row, que[i].rock->col, que[i].rock->colour);
    // }

    // printf("\n");

    // for (int i = 1; i < pqInsertIdx; i++) {
    //     printf("%d %d %d %d\n", pq[i].energy, pq[i].rock->row, pq[i].rock->col, pq[i].rock->colour);
    // }

    // printf("\n");

    while (1 < pqInsertIdx) {
        int curIdx = 1;
        EnergyNode top = pq[curIdx];
        pq[curIdx] = pq[--pqInsertIdx];

        while (true) {
            int leftIdx = curIdx << 1, rightIdx = leftIdx + 1;
            if (leftIdx >= pqInsertIdx)
                break;

            if (rightIdx >= pqInsertIdx) {
                if (pq[curIdx].energy > pq[leftIdx].energy) {
                    EnergyNode node = pq[curIdx];
                    pq[curIdx] = pq[leftIdx];
                    pq[leftIdx] = node;
                }
                break;
            }

            if (pq[leftIdx].energy < pq[curIdx].energy && pq[leftIdx].energy <= pq[rightIdx].energy) {
                EnergyNode node = pq[curIdx];
                pq[curIdx] = pq[leftIdx];
                pq[leftIdx] = node;
                curIdx = leftIdx;
            } else if (pq[rightIdx].energy < pq[curIdx].energy && pq[rightIdx].energy <= pq[leftIdx].energy) {
                EnergyNode node = pq[curIdx];
                pq[curIdx] = pq[rightIdx];
                pq[rightIdx] = node;
                curIdx = rightIdx;
            } else {
                break;
            }
        }

        // printf("%d %d %d %d\n", top.energy, top.rock->row, top.rock->col, top.rock->colour);

        // todo
    }

    free(rocks);

    for (int r = 0; r < height; r++)
        free(explored[r]);
    free(explored);

    for (int r = 0; r < height; r++)
        free(prevRock[r]);
    free(prevRock);

    free(pq);
    free(que);

    return p;
}

struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    // TODO - Task 3
    struct path p = {0, NULL};
    return p;
}

// task1 
// multiple start point (exploired) vs single start point (no explore)

// task2
// task1 vs task2 (no while pop) vs minimum spanning (while pop)

// task2
// pq (implement by min-heap)