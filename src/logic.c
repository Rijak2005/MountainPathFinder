// calculation of the best path with the greedy alogrithm
#include "../header/data-read.h"
#include "../header/cmdline.h"
#include "../header/graphics.h"
#include "../header/logic.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Point *bestPath = NULL; // Pointer to store the best path found
int bestPathEffort = __INT_MAX__; // Initialize bestPathEffort to a very high value

Stats greedyUserPathStats = {NULL, 0, 0, 0, 0};

void stepTo(Point *currentPath, int index, int x, int y, int xChange, int yChange, int effort, int *currentEffort) {
  int newX = x + xChange;
  int newY = y + yChange;

  if (newY < 0 || newY >= GRIDROWS || newX >= GRIDCOLS) {
    printf("Sorry out of Bounds!");
    return;
  }

  *currentEffort += effort;

  currentPath[index + 1].x = newX;
  currentPath[index + 1].y = newY; 
}

Stats calculateUserPathStats(Point* path, int pathLength, int effort){
  Stats result;
  result.elevations = malloc(sizeof(int) * pathLength);
  if(!result.elevations) {
    fprintf(stderr, "Memory allocation for elevations failed.\n");
    result.pathLength = 0;
    return result;
  }
  result.pathLength = pathLength;
  result.totalEffort = effort;
  result.uphill = 0;
  result.downhill = 0;
  // calculate the elevations array and the uphill and downhill values
  for (int i = 0; i < pathLength; i++) {
    result.elevations[i] = bergdaten[path[i].y][path[i].x];
    if (i > 0) {
      int elevationChange = result.elevations[i] - result.elevations[i - 1];
      if (elevationChange > 0) {
        result.uphill += elevationChange;
      } else if (elevationChange < 0) {
        result.downhill += -1*elevationChange;
      }
    }
  }
  return result;
}

Point* getPathFromStartPoint(Point start){
  // A path is an Array of points.
  Point *currentPath = malloc(sizeof(Point)*(GRIDCOLS-start.x));
  currentPath[0] = start;
  int currentEffort = 0;

  if (!currentPath) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }
  
  for (int i = 0;  i < (GRIDCOLS-start.x-1); i++){
    int x = currentPath[i].x;
    int y = currentPath[i].y;

    int effortStraight = abs(bergdaten[y][x] - bergdaten[y][x+1]);
    int effortDiagDown = (y < GRIDROWS - 1) ? abs(bergdaten[y][x] - bergdaten[y + 1][x + 1]) : __INT_MAX__;
    int effortDiagUp = (y > 0) ? abs(bergdaten[y][x] - bergdaten[y - 1][x + 1]) : __INT_MAX__;

    if (effortStraight <= effortDiagDown && effortStraight <= effortDiagUp) {
      // go straight ahead
      stepTo(currentPath, i, x, y, 1, 0, effortStraight, &currentEffort);
      continue;
    }
    if (effortDiagDown < effortStraight && effortDiagDown < effortDiagUp) { 
      // go diagonally down
      stepTo(currentPath, i, x, y, 1, 1, effortDiagDown, &currentEffort);
      continue;
    }
    if (effortDiagUp < effortDiagDown && effortDiagUp < effortStraight) {
      // go diagonally up 
      stepTo(currentPath, i, x, y, 1, -1, effortDiagUp, &currentEffort);
      continue;
    }
    if (effortDiagDown == effortDiagUp){
      // tie now toss a coin
      int randomNumber = rand() % 2;
      if (randomNumber) stepTo(currentPath, i, x, y, 1, 1, effortDiagDown, &currentEffort);
      else stepTo(currentPath, i, x, y, 1, -1, effortDiagUp, &currentEffort);
      continue;
    }
  }

  if (currentEffort < bestPathEffort && !interactiveMode) {
    bestPathEffort = currentEffort;
    free(bestPath); // Free the previous best path
    bestPath = currentPath; // Directly assign the current path to bestPath
  } else if (interactiveMode) {
    // In interactive mode, we display the stats
    greedyUserPathStats = calculateUserPathStats(currentPath, GRIDCOLS - start.x, currentEffort);
  }
  
  return currentPath;
}

