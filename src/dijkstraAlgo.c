#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include "../header/data-read.h"
#include "../header/cmdline.h"
#include "../header/graphics.h"
#include "../header/logic.h"
#include "../header/dijkstraAlgo.h"
#define INF INT_MAX
#define HEAPSIZE 405120

// max pathlength: a lot because of interactive mode with blocked paths
#define MAX_PATH_LENGTH ((GRIDROWS + GRIDCOLS)*100) //> 130.000


typedef struct {
    int X,Y,Cost;
} Node;

Node heap[HEAPSIZE];
int heapSize = 0;

void swap(Node *a, Node *b){
    Node temp = *a;
    *a = *b;
    *b = temp;
}
//adds new node and makes sure that parent-nodes are less costly than child-nodes
void push(Node n){
    if(heapSize >= HEAPSIZE){
        printf("Heap Overflow\n");
        exit(1);
    }
    int i = heapSize++;
    heap[i] = n;
    while(i > 0 && heap[i].Cost < heap[(i-1)/2].Cost){
        swap(&heap[i], &heap[(i-1)/2]);
        i = (i - 1) / 2;
    }
}
//gives back node with smallest cost and removes it
Node pop() {
    Node top = heap[0];
    heap[0] = heap[--heapSize];
    int i = 0;
    while (1) {
        int left = i * 2 + 1, right = i * 2 + 2, smallest = i;
        if (left < heapSize && heap[left].Cost < heap[smallest].Cost) smallest = left;
        if (right < heapSize && heap[right].Cost < heap[smallest].Cost) smallest = right;
        if (smallest == i) break;
        swap(&heap[i], &heap[smallest]);
        i = smallest;
    }
    return top;
}

int absdiff(int a, int b) {
    return a > b ? a - b : b - a;
}
int inBounds(int x, int y) {
    return x >= 0 && x < GRIDCOLS && y >= 0 && y < GRIDROWS;
}

Path dijkstra(Point start) {
    
    int dist[GRIDROWS][GRIDCOLS]; //array for shortest distance
    char visited[GRIDROWS][GRIDCOLS];  //array for already visited nodes
    int prev[GRIDROWS][GRIDCOLS][2];  //3d array for path reconstruktion , previous nodes
    heapSize = 0;

    Path path;
   
    path.x = malloc(MAX_PATH_LENGTH * sizeof(int));
    path.y = malloc(MAX_PATH_LENGTH * sizeof(int));

    if (!path.x || !path.y) {
        printf("bad memory reservation\n");
        exit(1);
    }
    path.length = 0;

    for (int y = 0; y < GRIDROWS; y++) {
        for (int x = 0; x < GRIDCOLS; x++) {
            dist[y][x] = INF;
            visited[y][x] = 0;
            prev[y][x][0] = -1; //saves the y-value of the previous node
            prev[y][x][1] = -1; //    "     x-value        "
        }
    }

    if(interactiveMode){
        dist[start.y][start.x] = 0;
        if (sperren[start.y][start.x]) {
        printf("Startpunkt ist gesperrt!\n");
        path.length = 0;
        return path;
        }
        push((Node){start.x,start.y,0});
    }else{
        for (int y = 0; y < GRIDROWS; y++) {
            dist[y][0] = 0;
            push((Node){0, y, 0});
        }
    }
    //defines the ways the algo can go: right, bottom right, top right, bottom, top, left, top left, bottom left
    int dx[] = {1, 1, 1, 0, 0, -1, -1, -1}; //example: you are at x,y => x+1,y  == taking one step to the right
    int dy[] = {0, -1, 1, -1, 1, 0, 1, -1};
    //main loop: get node with lowest cost form heap and mark as visited
    while (heapSize > 0) {
        Node current = pop();
        int x = current.X;
        int y = current.Y;

        if (current.Cost > dist[y][x]) continue;
        if(visited[y][x]) continue;
        visited[y][x] = 1;
        //goes in the 8 directions
        for (int d = 0; d < 8; d++) {
            int nx = x + dx[d];
            int ny = y + dy[d];

            if (!inBounds(nx, ny)) continue;
            if(sperren[ny][nx]) continue;
            

            // Block diagonals if adjacent orthogonals are blocked
            if (d == 1) {
                if (sperren[y-1][x] || sperren[y][x+1]) continue;  // N or E
            }
            else if (d == 2) {
                if (sperren[y+1][x] || sperren[y][x+1]) continue;  // S or E
            }
            else if (d == 6) {
                if (sperren[y+1][x] || sperren[y][x-1]) continue;  // S or W
            }
            else if (d == 7) {
                if (sperren[y-1][x] || sperren[y][x-1]) continue;  // N or W
            }

            //if shorter path got found, update new distanz and previous and add updated neighbour to heap
            int cost = absdiff(bergdaten[y][x], bergdaten[ny][nx]);
            if (dist[y][x] + cost < dist[ny][nx]) {
                dist[ny][nx] = dist[y][x] + cost;
                prev[ny][nx][0] = y;
                prev[ny][nx][1] = x;
                push((Node){nx, ny, dist[ny][nx]});
            }
        }
    }

    // searching for Endcolumn 
    int min_cost = INF;
    int end_y = -1;
    for (int y = 0; y < GRIDROWS; y++) {
        if (dist[y][GRIDCOLS - 1] < min_cost) {
            min_cost = dist[y][GRIDCOLS - 1];
            end_y = y;
        }
    }

    if (end_y == -1) {
        printf("bad endpoint\n");
        path.length = 0;
        return path;
    }

    int cx = GRIDCOLS - 1;
    int cy = end_y;

    int temp_x[MAX_PATH_LENGTH];
    int temp_y[MAX_PATH_LENGTH];
    int len = 0;
    //backtrack the path with prev array
    while (cx != -1 && cy != -1 && len < MAX_PATH_LENGTH) {
        temp_x[len] = cx;
        temp_y[len] = cy;
        len++;

        if (cx == start.x && cy == start.y) {
            break;
        }

        int py = prev[cy][cx][0];
        int px = prev[cy][cx][1];

        if (px == -1 || py == -1) {
            break;
        }

        cx = px;
        cy = py;
    }
 
    if (len >= MAX_PATH_LENGTH) {
        printf("path too long, discontinued at lenght: %d\n", len);
        path.length = 0;
        return path;
    }
    //reverse path, because it was backtracked
    for (int i = 0; i < len; i++) {
        path.x[i] = temp_x[len - 1 - i];
        path.y[i] = temp_y[len - 1 - i];
    }
    path.length = len;

        //uphill and downhill
    int total_up = 0, total_down = 0;
    

    for (int i = 1; i < path.length; i++) {
        int h1 = bergdaten[path.y[i - 1]][path.x[i - 1]];
        int h2 = bergdaten[path.y[i]][path.x[i]];
        int delta = h2 - h1;

        if (delta > 0) {total_up += delta; }
        else total_down -= delta; //delta is negativ . total_down - (-delta) = total_down
        
    }
    if(!interactiveMode){
    printf("\n- Best Path (Dijkstra, pink)\n\t- Effort: %d\n\t- Starting row: %d\n\t- Ending row: %d\n\t- Path length: %d steps\n", min_cost, path.y[0], path.y[path.length - 1], path.length);
    printf("\t- up: %d\n", total_up);
    printf("\t- down: %d\n", total_down);
    }
    return path;
}
Stats getStatsFromPath(Path path) {
    Stats stats = {0};
    if (path.length < 2) return stats;

    stats.pathLength = path.length;
    stats.elevations = malloc(path.length * sizeof(int));
    if (!stats.elevations) {
        printf("Memory allocation failed in getStatsFromPath\n");
        return stats;
    }

    stats.totalEffort = 0;
    stats.uphill = 0;
    stats.downhill = 0;

    for (int i = 0; i < path.length; i++) {
        stats.elevations[i] = bergdaten[path.y[i]][path.x[i]];
    }

    for (int i = 1; i < path.length; i++) {
        int delta = stats.elevations[i] - stats.elevations[i - 1];
        stats.totalEffort += abs(delta);
        if (delta > 0) stats.uphill += delta;
        else stats.downhill -= delta;  //delta is negativ . total_down - (-delta) = total_down
    }

    return stats;
}
