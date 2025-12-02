// Implements optimal pathfinding using dynamic programming

#include "../header/data-read.h"
#include "../header/cmdline.h"
#include "../header/graphics.h"
#include "../header/logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Stats dpUserPathStats = {NULL, 0, 0, 0, 0};


// Function: getPathFromStartPoint
// Returns: Optimal path from start point to right edge
Point* getPathFromStartPointWithDP(Point start) {
    const int INF = __INT_MAX__;  // Represents "infinity" (unreachable)
    
    // Edge case: Start at last column
    if (start.x >= GRIDCOLS - 1) {
        Point* path = malloc(sizeof(Point));
        if (!path) return NULL;
        path[0] = start;
        if (interactiveMode) printf("\n\tEffort: 0\n");
        return path;
    }

    // Allocate DP tables
    int** dp = malloc(GRIDROWS * sizeof(int*));
    int** parent = malloc(GRIDROWS * sizeof(int*));
    if (!dp || !parent) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }

    // Initialize tables
    for (int y = 0; y < GRIDROWS; y++) {

        dp[y] = malloc(GRIDCOLS * sizeof(int));
        parent[y] = malloc(GRIDCOLS * sizeof(int));
        
        if (!dp[y] || !parent[y]) {
            // Cleanup partial allocations
            for (int i = 0; i < y; i++) {
              fprintf(stderr, "Memory allocation failed for row %d\n", i);
              free(dp[i]); 
              free(parent[i]);
            }
            free(dp);
            free(parent);
            return NULL;
        }

        // Set initial values
        for (int x = 0; x < GRIDCOLS; x++) {
          if (x == start.x && !interactiveMode) {dp[y][x] = 0;} // Starting column
          else{ dp[y][x] = INF;}      // Infinite effort (unreachable)
          // dp[y][x] = INF;
          parent[y][x] = -1;    // No parent
        }
    }

    // Base case: starting position
    dp[start.y][start.x] = 0;

    // Main DP processing (left to right)
    for (int x = start.x + 1; x < GRIDCOLS; x++) {
        for (int y = 0; y < GRIDROWS; y++) {
            int minEffort = INF;
            int bestPrevY = -1;

            // Check 3 possible previous positions:
            // 1. Left neighbor (same y)
            if (dp[y][x-1] != INF) {
                int effort = abs(bergdaten[y][x-1] - bergdaten[y][x]);
                int total = dp[y][x-1] + effort;
                if (total < minEffort) {
                    minEffort = total;
                    bestPrevY = y;
                }
            }

            // 2. Top-left neighbor (y-1)
            if (y > 0 && dp[y-1][x-1] != INF) {
                int effort = abs(bergdaten[y-1][x-1] - bergdaten[y][x]);
                int total = dp[y-1][x-1] + effort;
                if (total < minEffort) {
                    minEffort = total;
                    bestPrevY = y-1;
                }
            }

            // 3. Bottom-left neighbor (y+1)
            if (y < GRIDROWS - 1 && dp[y+1][x-1] != INF) {
                int effort = abs(bergdaten[y+1][x-1] - bergdaten[y][x]);
                int total = dp[y+1][x-1] + effort;
                if (total < minEffort) {
                    minEffort = total;
                    bestPrevY = y+1;
                }
            }

            // Update if valid path found
            if (bestPrevY != -1) {
                dp[y][x] = minEffort;
                parent[y][x] = bestPrevY;
            }
        }
    }

    // Find best endpoint in last column
    int min_effort = INF;
    int endY = -1;
    for (int y = 0; y < GRIDROWS; y++) {
        if (dp[y][GRIDCOLS-1] < min_effort) {
            min_effort = dp[y][GRIDCOLS-1];
            endY = y;
        }
    }

    // Handle no valid path
    if (endY == -1) {
        for (int y = 0; y < GRIDROWS; y++) {
            free(dp[y]); free(parent[y]);
        }
        free(dp); free(parent);
        return NULL;
    }

    // Backtrack to construct path
    int pathLength = GRIDCOLS - start.x;
    Point* path = malloc(pathLength * sizeof(Point));
    if (!path) {
        for (int y = 0; y < GRIDROWS; y++) {
            free(dp[y]); free(parent[y]);
        }
        free(dp); free(parent);
        return NULL;
    }

    int curX = GRIDCOLS - 1;
    int curY = endY;
    for (int idx = pathLength - 1; idx >= 0; idx--) {
        path[idx] = (Point){curX, curY};
        if (curX > start.x) {
            curY = parent[curY][curX];  // Move to parent
            curX--;  // Move left
        }
    }

    // Update global best path if needed
    if (!interactiveMode && min_effort < bestPathEffort) {
        bestPathEffort = min_effort;
    }

    // Output in interactive mode
    if (interactiveMode) {
      dpUserPathStats = calculateUserPathStats(path, pathLength, min_effort);
    }

    // Cleanup DP tables
    for (int y = 0; y < GRIDROWS; y++) {
        free(dp[y]);
        free(parent[y]);
    }
    free(dp);
    free(parent);

    return path;
}