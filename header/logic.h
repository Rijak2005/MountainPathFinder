#pragma once

typedef struct {
  int x;
  int y;
} Point;

typedef struct {
  int* elevations;
  int pathLength, totalEffort, uphill, downhill;
} Stats;

Point* getPathFromStartPoint(Point start);
Stats calculateUserPathStats(Point* path, int pathLength, int effort);

extern Point* bestPath;
extern int bestPathEffort;
extern Stats greedyUserPathStats;