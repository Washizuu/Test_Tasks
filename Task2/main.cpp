#include <algorithm>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <queue>

struct Point {
    int x;
    int y;
};

enum CellType {ENTRANCE =0,
    EXIT = 1,
    ROAD = 2,
    WALL = 3,
    TRAP = 4,
    TREASURE = 5
};

class Maze {
private:
    int rows;
    int columns;
    std::mt19937 rng;
    std::vector<std::vector<int>> grid;

public:
    Maze(int r, int c) : rows(r), columns(c) {
        if (rows % 2 == 0) rows++;
        if (columns % 2 == 0) columns++;

        grid.resize(rows, std::vector<int>(columns, WALL));

        rng.seed(std::time(0));
    }

    void GenerateMaze(int cx, int cy) {
        grid[cy][cx] = ROAD;

        int directions[4][2] = {{0, -2}, {0, 2}, {-2, 0}, {2, 0}};

        std::shuffle(std::begin(directions), std::end(directions), rng);

        for (auto& dir : directions) {
            int dy = dir[0];
            int dx = dir[1];

            int new_x = cx + dx;
            int new_y = cy + dy;

            if (new_y > 0 && new_y < rows - 1 && new_x > 0 && new_x < columns - 1 && grid[new_y][new_x] == WALL) {
                grid[cy + dy / 2][cx + dx / 2] = ROAD;
                GenerateMaze(new_x, new_y);
            }
        }
    }

    void PlaceObjects() {
        grid[1][1] = ENTRANCE;

        grid[rows - 2][columns - 2] = EXIT;

        std::vector<Point> freeRoads;

        for (int y = 1; y < rows - 1; y++) {
            for (int x = 1; x < columns - 1; x++) {
                if (grid[y][x] == ROAD) {
                    freeRoads.push_back({x, y});
                }
            }
        }

        std::shuffle(freeRoads.begin(), freeRoads.end(), rng);

        if (!freeRoads.empty()) {
            Point p = freeRoads.back();
            grid[p.y][p.x] = TREASURE;
            freeRoads.pop_back();
        }

        std::uniform_int_distribution<int> dist(0, 5);
        int trapCount = dist(rng);

        for (int i = 0; i < trapCount; i++) {
            if (freeRoads.empty()) break;

            Point p = freeRoads.back();
            grid[p.y][p.x] = TRAP;
            freeRoads.pop_back();
        }
    }

    void Build() {
        bool success = false;
        while (!success) {
            for(auto& row : grid) {
                std::fill(row.begin(), row.end(), WALL);
            }

            GenerateMaze(1, 1);

            PlaceObjects();

            if (IsSolvable()) {
                success = true;
            }
        }
    }

    void Print() {
        for (const auto& row : grid) {
            for (int cell : row) {
                char c = '#';
                if (cell == ROAD) c = ' ';
                else if (cell == ENTRANCE) c = 'S';
                else if (cell == EXIT) c = 'E';
                else if (cell == TRAP) c = '0';
                else if (cell == TREASURE) c = 'T';
                std::cout << c << " ";
            }
            std::cout << "\n";
        }
    }

    bool IsSolvable() {
        struct State {
            int x, y, damage;
        };

        std::queue<State> states;
        states.push({1,1,0});

        std::vector<std::vector<bool>> visited(rows, std::vector<bool>(columns, false));
        visited[1][1] = true;

        int dx[] = {0, 0, -1, 1};
        int dy[] = {-1, 1, 0, 0};

        while (!states.empty()) {
            State current = states.front();
            states.pop();

            if (grid[current.y][current.x] == EXIT) {
                return true;
            }

            for (int i = 0; i < 4; i++) {
                int new_x = current.x + dx[i];
                int new_y = current.y + dy[i];

                if (new_x >= 0 && new_x < columns && new_y >= 0 && new_y < rows &&
                    grid[new_y][new_x] != WALL && !visited[new_y][new_x]) {
                    int receivedDamage = current.damage + (grid[new_y][new_x] == TRAP ? 1 : 0);
                    if (receivedDamage < 3) {
                        visited[new_y][new_x] = true;
                        states.push({new_x, new_y, receivedDamage});
                    }
                }
            }
        }
        return false;
    }


};

int main() {
    std::cout << "Enter the number of rows and columns of Maze: ";
    int rows, columns;
    std::cin >> rows >> columns;
    Maze maze(rows, columns);
    maze.Build();
    maze.Print();
    return 0;

}