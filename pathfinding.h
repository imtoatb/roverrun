#ifndef PATHFINDING_H
#define PATHFINDING_H

// Configuration struct
typedef struct {
    int rows;
    int cols;
    int max_cost;
    int max_paths;
    int jump_cost_threshold;
} Config;

// External configuration variable
extern Config config;

// Structs
typedef struct {
    char move[4];  // Array for move description (e.g., "2JD" for 2 tiles Jump Down)
    int terrainCost;  // Terrain cost at this step
    int totalCost;    // Cumulative total cost at this step
} Node;

typedef struct {
    Node path[49]; // Maximum path length: ROWS * COLS
    int cost;
    int length;
} Path;

// External variables
extern int visited[7][6];
extern Path allPaths[3];
extern int pathIndex;

// Function declarations
int comparePaths(const void *a, const void *b);
int isJumpValid(int x1, int y1, int x2, int y2, int jumpLength, int map[7][6]);
void dfs(int x, int y, int currentCost, int map[7][6], int targetTile, Path currentPath, int isStart);
void printPath(Path path);

#endif // PATHFINDING_H
