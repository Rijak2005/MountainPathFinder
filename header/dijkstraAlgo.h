#pragma once
#include "logic.h"
#include "cmdline.h"
#include "graphics.h"
typedef struct {
    int *x;
    int *y;
    int length;
} Path;
Path dijkstra(Point interactiveStartPoint);
extern int total_up;
extern int total_down;
Stats getStatsFromPath(Path path);
int inBounds(int x, int y);