#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX 50

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point point;
    int f, g, h;
    Point parent; // To track the path
} Node;

typedef struct {
    Node nodes[MAX * MAX];
    int size;
} PriorityQueue;

int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// Utility functions
bool isValid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

bool isObstacle(int maze[MAX][MAX], int x, int y) {
    return (maze[x][y] == -1);
}

int heuristic(Point a, Point b) {
    return abs(a.x - b.x) + abs(a.y - b.y); // Manhattan distance
}

void initPriorityQueue(PriorityQueue *pq) {
    pq->size = 0;
}

bool isEmpty(PriorityQueue *pq) {
    return pq->size == 0;
}

void insert(PriorityQueue *pq, Node node) {
    pq->nodes[pq->size++] = node;
}

Node removeMin(PriorityQueue *pq) {
    int minIndex = 0;
    for (int i = 1; i < pq->size; i++) {
        if (pq->nodes[i].f < pq->nodes[minIndex].f) {
            minIndex = i;
        }
    }
    Node minNode = pq->nodes[minIndex];
    pq->nodes[minIndex] = pq->nodes[--pq->size];
    return minNode;
}

void printMaze(int maze[MAX][MAX], int rows, int cols, Point current) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == current.x && j == current.y) {
                printf("B "); // Current position
            } else if (maze[i][j] == -1) {
                printf("X "); // Obstacle
            } else if (maze[i][j] == 2) {
                printf("A "); // Path for Best First Search
            } else if (maze[i][j] == 3) {
                printf("P "); // Path for A* Search
            } else {
                printf("O "); // Empty space
            }
        }
        printf("\n");
    }
    printf("\n");
}

void printPath(Node path[MAX][MAX], Point start, Point goal) {
    Point p = goal;
    int cost = 0;

    printf("Path: ");
    while (!(p.x == start.x && p.y == start.y)) {
        printf("(%d, %d) <- ", p.x, p.y);
        p = path[p.x][p.y].parent;
        cost++;
    }
    printf("(%d, %d)\n", start.x, start.y);
    printf("Total steps: %d\n", cost);
}

void printOpenList(PriorityQueue pq) {
    printf("Open Nodes List:\n");
    for (int i = 0; i < pq.size; i++) {
        printf("(%d, %d) f: %d\n", pq.nodes[i].point.x, pq.nodes[i].point.y, pq.nodes[i].f);
    }
}

void printClosedList(bool visited[MAX][MAX], int rows, int cols) {
    printf("Closed Nodes List:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (visited[i][j]) {
                printf("(%d, %d) ", i, j);
            }
        }
    }
    printf("\n");
}

// Function to print the maze with the final path marked
void printFinalMazeWithPath(int maze[MAX][MAX], Node path[MAX][MAX], Point start, Point goal, int rows, int cols, int mode) {
    Point p = goal;
    int pathSymbol = (mode == 1) ? 2 : 3; // 2 for Best First Search, 3 for A*

    while (!(p.x == start.x && p.y == start.y)) {
        maze[p.x][p.y] = pathSymbol; // Marking the path with A or P
        p = path[p.x][p.y].parent;
    }
    maze[start.x][start.y] = pathSymbol; // Mark the start point as part of the path

    printf("Final Maze with Path:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == -1) {
                printf("X "); // Obstacle
            } else if (maze[i][j] == 2) {
                printf("A "); // Path for Best First Search
            } else if (maze[i][j] == 3) {
                printf("P "); // Path for A* Search
            } else {
                printf("O "); // Empty space
            }
        }
        printf("\n");
    }
}

// Best First Search
bool bestFirstSearch(int maze[MAX][MAX], int rows, int cols, Point start, Point goal) {
    PriorityQueue pq;
    initPriorityQueue(&pq);

    Node startNode = {start, heuristic(start, goal), 0, heuristic(start, goal), start};
    insert(&pq, startNode);

    bool visited[MAX][MAX] = {false};
    visited[start.x][start.y] = true;

    Node path[MAX][MAX];

    while (!isEmpty(&pq)) {
        Node current = removeMin(&pq);

        printMaze(maze, rows, cols, current.point);
        printOpenList(pq);
        printClosedList(visited, rows, cols);

        if (current.point.x == goal.x && current.point.y == goal.y) {
            printf("Path found using Best First Search.\n");
            printPath(path, start, goal);
            printFinalMazeWithPath(maze, path, start, goal, rows, cols, 1); // 1 for Best First Search
            return true;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.point.x + dx[i];
            int ny = current.point.y + dy[i];

            if (isValid(nx, ny, rows, cols) && !visited[nx][ny] && !isObstacle(maze, nx, ny)) {
                visited[nx][ny] = true;
                Node neighbor = {{nx, ny}, heuristic((Point){nx, ny}, goal), 0, heuristic((Point){nx, ny}, goal), current.point};
                path[nx][ny] = neighbor;
                insert(&pq, neighbor);
            }
        }
    }

    printf("No path found using Best First Search.\n");
    return false;
}

// A* Search
bool aStarSearch(int maze[MAX][MAX], int rows, int cols, Point start, Point goal) {
    PriorityQueue pq;
    initPriorityQueue(&pq);

    Node startNode = {start, heuristic(start, goal), 0, heuristic(start, goal), start};
    insert(&pq, startNode);

    bool visited[MAX][MAX] = {false};
    visited[start.x][start.y] = true;

    Node path[MAX][MAX];

    while (!isEmpty(&pq)) {
        Node current = removeMin(&pq);

        printMaze(maze, rows, cols, current.point);
        printOpenList(pq);
        printClosedList(visited, rows, cols);

        if (current.point.x == goal.x && current.point.y == goal.y) {
            printf("Path found using A* Search.\n");
            printPath(path, start, goal);
            printFinalMazeWithPath(maze, path, start, goal, rows, cols, 2); // 2 for A* Search
            return true;
        }

        for (int i = 0; i < 4; i++) {
            int nx = current.point.x + dx[i];
            int ny = current.point.y + dy[i];

            if (isValid(nx, ny, rows, cols) && !visited[nx][ny] && !isObstacle(maze, nx, ny)) {
                visited[nx][ny] = true;
                int g = current.g + 1;
                int h = heuristic((Point){nx, ny}, goal);
                Node neighbor = {{nx, ny}, g + h, g, h, current.point};
                path[nx][ny] = neighbor;
                insert(&pq, neighbor);
            }
        }
    }

    printf("No path found using A* Search.\n");
    return false;
}

// Main function
int main() {
    int rows, cols;
    int maze[MAX][MAX];
    Point start, goal;
    int numObstacles;

    printf("Enter the number of rows in the maze: ");
    scanf("%d", &rows);

    printf("Enter the number of columns in the maze: ");
    scanf("%d", &cols);

    // Initialize the maze with 0s
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = 0;
        }
    }

    // Input obstacles
    printf("Enter the number of obstacles: ");
    scanf("%d", &numObstacles);

    for (int i = 0; i < numObstacles; i++) {
        int x, y;
        printf("Enter obstacle coordinates (row and column): ");
        scanf("%d %d", &x, &y);
        maze[x][y] = -1; // Mark obstacle positions
    }

    // Input start and goal positions
    printf("Enter start coordinates (row and column): ");
    scanf("%d %d", &start.x, &start.y);

    printf("Enter goal coordinates (row and column): ");
    scanf("%d %d", &goal.x, &goal.y);

    int choice;
    printf("Choose search method:\n1. Best First Search\n2. A* Search\n");
    scanf("%d", &choice);

    if (choice == 1) {
        bestFirstSearch(maze, rows, cols, start, goal);
    } else if (choice == 2) {
        aStarSearch(maze, rows, cols, start, goal);
    } else {
        printf("Invalid choice.\n");
    }

    return 0;
}