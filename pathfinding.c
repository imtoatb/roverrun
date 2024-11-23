#include "pathfinding.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global variables
int visited[7][6] = {0};  // Using static size for simplicity
Path allPaths[3];         // Config size initialized in `main`
int pathIndex = 0;

// Function implementations
int comparePaths(const void *a, const void *b) {
    return ((Path*)a)->cost - ((Path*)b)->cost;
}

int isJumpValid(int x1, int y1, int x2, int y2, int jumpLength, int map[7][6]) {
    // Check if the skipped tiles have cost <= config.jump_cost_threshold
    if (x1 == x2) {  // Horizontal jump
        int minY = y1 < y2 ? y1 : y2;
        int maxY = y1 > y2 ? y1 : y2;
        for (int y = minY + 1; y < maxY; y++) {
            if (map[x1][y] > config.jump_cost_threshold) {
                return 0; // Invalid jump due to high terrain cost
            }
        }
    } else if (y1 == y2) {  // Vertical jump
        int minX = x1 < x2 ? x1 : x2;
        int maxX = x1 > x2 ? x1 : x2;
        for (int x = minX + 1; x < maxX; x++) {
            if (map[x][y1] > config.jump_cost_threshold) {
                return 0; // Invalid jump due to high terrain cost
            }
        }
    }
    return 1; // Jump is valid
}

void dfs(int x, int y, int currentCost, int map[7][6], int targetTile, Path currentPath, int isStart) {
    // Calculate the maximum allowed path length
    int maxMovements = (int)sqrt(config.rows * config.cols);

    // Skip out-of-bounds, visited tiles, or tiles with cost greater than config.max_cost
    if (x < 0 || x >= config.rows || y < 0 || y >= config.cols || visited[x][y] || map[x][y] > config.max_cost) return;

    // Check if the current path length exceeds the maximum allowed movements
    if (currentPath.length >= maxMovements) {
        return;  // Stop if the path exceeds the length limit
    }

    // If the target tile is found, store the path
    if (map[x][y] == targetTile) {
        currentPath.path[currentPath.length++] = (Node){" ", map[x][y], currentCost + map[x][y]};
        currentPath.cost = currentCost + map[x][y];

        // Store the path in the allPaths array if it's one of the top shortest paths
        if (currentPath.length > 0 && currentPath.cost < allPaths[config.max_paths - 1].cost) {
            allPaths[config.max_paths - 1] = currentPath;
            qsort(allPaths, config.max_paths, sizeof(Path), comparePaths);
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
    if (x + 2 < config.rows && isJumpValid(x, y, x + 2, y, 2, map)) {
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
    if (y + 2 < config.cols && isJumpValid(x, y, x, y + 2, 2, map)) {
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
    if (x + 3 < config.rows && isJumpValid(x, y, x + 3, y, 3, map)) {
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
    if (y + 3 < config.cols && isJumpValid(x, y, x, y + 3, 3, map)) {
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
