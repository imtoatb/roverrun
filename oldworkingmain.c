#include <stdio.h>
#include <stdlib.h>
#include <math.h>  // For sqrt

#define ROWS 7
#define COLS 6
#define MAX_COST 12
#define MAX_PATHS 3
#define JUMP_COST_THRESHOLD 20        // Max cost of tiles that can be jumped over

typedef struct {
    char move[4];                     // Array for move description (e.g., "2JD" for 2 tiles Jump Down)
    int terrainCost;                  // Terrain cost at this step
    int totalCost;                    // Cumulative total cost at this step
} Node;

typedef struct {
    Node path[ROWS * COLS];
    int cost;
    int length;
} Path;

int visited[ROWS][COLS] = {0};
Path allPaths[MAX_PATHS];              // Store the top 3 paths
int pathIndex = 0;

int comparePaths(const void *a, const void *b) {
    return ((Path*)a)->cost - ((Path*)b)->cost;
}

int isJumpValid(int x1, int y1, int x2, int y2, int jumpLength, int map[ROWS][COLS]) {
    // Check if the skipped tiles have cost <= JUMP_COST_THRESHOLD
    if (x1 == x2) {                                                                      // Horizontal jump
        int minY = y1 < y2 ? y1 : y2;
        int maxY = y1 > y2 ? y1 : y2;
        for (int y = minY + 1; y < maxY; y++) {
            if (map[x1][y] > JUMP_COST_THRESHOLD) {
                return 0;                                                                 // Invalid jump due to high terrain cost
            }
        }
    } else if (y1 == y2) {  // Vertical jump
        int minX = x1 < x2 ? x1 : x2;
        int maxX = x1 > x2 ? x1 : x2;
        for (int x = minX + 1; x < maxX; x++) {
            if (map[x][y1] > JUMP_COST_THRESHOLD) {
                return 0;                                                                 // Invalid jump due to high terrain cost
            }
        }
    }
    return 1;                                                                             // Jump is valid
}

void dfs(int x, int y, int currentCost, int map[ROWS][COLS], int targetTile, Path currentPath, int isStart) {
    // Calculate the maximum allowed path length
    int maxMovements = (int)sqrt(ROWS * COLS);

    // Skip out-of-bounds, visited tiles, or tiles with cost greater than MAX_COST
    if (x < 0 || x >= ROWS || y < 0 || y >= COLS || visited[x][y] || map[x][y] > MAX_COST) return;

    // Check if the current path length exceeds the maximum allowed movements
    if (currentPath.length >= maxMovements) {
        return;                                                                          // Stop if the path exceeds the length limit
    }

    // If the target tile is found, store the path
    if (map[x][y] == targetTile) {
        currentPath.path[currentPath.length++] = (Node){" ", map[x][y], currentCost + map[x][y]}; // Just for tracking
        currentPath.cost = currentCost + map[x][y];

        // Store the path in the allPaths array if it's one of the top 3 shortest
        if (currentPath.length > 0 && currentPath.cost < allPaths[MAX_PATHS - 1].cost) {
            allPaths[MAX_PATHS - 1] = currentPath; // Replace the longest path
            qsort(allPaths, MAX_PATHS, sizeof(Path), comparePaths); // Sort paths by cost
        }
        return;
    }

    // Mark the current tile as visited
    visited[x][y] = 1;

    // Start moving to neighboring tiles, don't add cost for the initial tile
    if (!isStart) {
        currentCost += map[x][y];
    }

    // Move Down
    currentPath.path[currentPath.length++] = (Node){"D", map[x][y], currentCost};
    dfs(x + 1, y, currentCost, map, targetTile, currentPath, 0);
    currentPath.length--;

    // Move Up
    currentPath.path[currentPath.length++] = (Node){"U", map[x][y], currentCost};
    dfs(x - 1, y, currentCost, map, targetTile, currentPath, 0);
    currentPath.length--;

    // Move Right
    currentPath.path[currentPath.length++] = (Node){"R", map[x][y], currentCost};
    dfs(x, y + 1, currentCost, map, targetTile, currentPath, 0);
    currentPath.length--;

    // Move Left
    currentPath.path[currentPath.length++] = (Node){"L", map[x][y], currentCost};
    dfs(x, y - 1, currentCost, map, targetTile, currentPath, 0);
    currentPath.length--;

    // Jump Down (2 tiles down)
    if (x + 2 < ROWS && isJumpValid(x, y, x + 2, y, 2, map)) {
        currentPath.path[currentPath.length++] = (Node){"2JD", map[x][y], currentCost};
        dfs(x + 2, y, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Up (2 tiles up)
    if (x - 2 >= 0 && isJumpValid(x, y, x - 2, y, 2, map)) {
        currentPath.path[currentPath.length++] = (Node){"2JU", map[x][y], currentCost};
        dfs(x - 2, y, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Right (2 tiles right)
    if (y + 2 < COLS && isJumpValid(x, y, x, y + 2, 2, map)) {
        currentPath.path[currentPath.length++] = (Node){"2JR", map[x][y], currentCost};
        dfs(x, y + 2, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Left (2 tiles left)
    if (y - 2 >= 0 && isJumpValid(x, y, x, y - 2, 2, map)) {
        currentPath.path[currentPath.length++] = (Node){"2JL", map[x][y], currentCost};
        dfs(x, y - 2, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Down (3 tiles down)
    if (x + 3 < ROWS && isJumpValid(x, y, x + 3, y, 3, map)) {
        currentPath.path[currentPath.length++] = (Node){"3JD", map[x][y], currentCost};
        dfs(x + 3, y, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Up (3 tiles up)
    if (x - 3 >= 0 && isJumpValid(x, y, x - 3, y, 3, map)) {
        currentPath.path[currentPath.length++] = (Node){"3JU", map[x][y], currentCost};
        dfs(x - 3, y, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Right (3 tiles right)
    if (y + 3 < COLS && isJumpValid(x, y, x, y + 3, 3, map)) {
        currentPath.path[currentPath.length++] = (Node){"3JR", map[x][y], currentCost};
        dfs(x, y + 3, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Jump Left (3 tiles left)
    if (y - 3 >= 0 && isJumpValid(x, y, x, y - 3, 3, map)) {
        currentPath.path[currentPath.length++] = (Node){"3JL", map[x][y], currentCost};
        dfs(x, y - 3, currentCost, map, targetTile, currentPath, 0);
        currentPath.length--;
    }

    // Unmark the current tile as visited before backtracking
    visited[x][y] = 0;
}

void printPath(Path path) {
    for (int i = 0; i < path.length; i++) {
        printf("%s (Tile: %d, Tot: %d) -> ", path.path[i].move, path.path[i].terrainCost, path.path[i].totalCost);
    }
    printf("TARGET (Total Cost: %d)\n", path.cost);
}

int main() {
    int map[ROWS][COLS] = {
        {7, 3, 2, 2, 10002, 13},
        {3, 1, 0, 1, 5, 9},
        {3, 2, 1, 5, 6, 7},
        {5, 3, 10001, 9, 7, 8},
        {7, 5, 9, 13, 8, 9},
        {8, 6, 7, 14, 9, 10},
        {10, 8, 9, 10014, 10, 11}
    };


    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("%d ", map[i][j] > 10000 ? -1 : map[i][j]);
        }
        printf("\n");
    }

    int startx = 5;
    int starty = 5;
    int targettile = 0;

    // Initialize allPaths with high cost
    for (int i = 0; i < MAX_PATHS; i++) {
        allPaths[i].cost = 1000000;                                 // An arbitrary large number to ensure proper sorting
        allPaths[i].length = 0;
    }

    Path currentPath;
    currentPath.length = 0;
    dfs(startx, starty, 0, map, targettile, currentPath, 1);

    // Print the 3 shortest paths
    for (int i = 0; i < MAX_PATHS && allPaths[i].length > 0; i++) {
        printPath(allPaths[i]);
    }

    return 0;
}
