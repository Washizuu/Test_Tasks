# Maze Generator

## Algorithm & Complexity

The generator uses a 2D matrix (`std::vector<std::vector<int>>`) to represent the grid and uses **Recursive Backtracker** for structure generation and **Breadth-First Search (BFS)** for validation.

-   **Maze Generation:** `O(N * M)`
  -   The Recursive Backtracker (DFS) visits every cell exactly once to create paths.
  -   Stack operations for backtracking take constant time `O(1)`.
-   **Object Placement:** `O(N * M)`
  -   Collecting available road cells requires a single pass through the matrix.
  -   `std::shuffle` ensures random distribution of traps and treasures in linear time relative to the number of road cells.
-   **Validation (BFS):** `O(V + E)` -> `O(N * M)`
  -   **Graph Traversal:** The algorithm treats cells as vertices (`V`) and adjacent paths as edges (`E`).
  -   **Worst Case:** In the worst-case scenario, the algorithm traverses all reachable nodes to verify that a path exists where the player takes `< 3` damage from traps.

*Where `N` is the number of rows and `M` is the number of columns.*

## Usage Example

The application accepts input from the console in the following format:
`[Rows] [Columns]`
## Legend

The console output uses specific characters to represent different cell types:

* `S` — **Entrance**: The starting point of the maze (always top-left).
* `E` — **Exit**: The destination point (always bottom-right).
* `#` — **Wall**: Impassable obstacles defining the maze structure.
* ` ` — **Road**: Walkable path (represented by a space).
* `0` — **Trap**: Stepping here deals **1 damage**. The path is guaranteed to have fewer than 3 traps.
* `T` — **Treasure**: A bonus item placed randomly within the maze.

**Example Session:**

```text
Enter the number of rows and columns of Maze: 15 21

# # # # # # # # # # # # # # # # # # # # #
# S #       #                   #       #
#   # T # # #   # # # # #   #   #   #   #
#   #               #       #   #   #   #
#   #   # # # # # # #   # # #   #   #   #
#   #   #               #   #   #   #   #
#   # # #   # # # # # # #   #   #   #   #
#           #       #       # 0 #   #   #
# # # # # # #   # # #   #   #   #   # # #
#           #           #   #   # 0     #
#   # # #   #   # # # # #   #   # # #   #
#       #   #           #   #       #   #
# # #   #   # # # # #   #   # # #   #   #
#       #               #             E #
# # # # # # # # # # # # # # # # # # # # #
```

## How to Build and Run

Ensure you have a C++ compiler and CMake installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task2
    ```

2.  **Build using CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

3.  **Run the application:**
    *On Linux/macOS:*
    ```bash
    ./Task2
    ```
    *On Windows:*
    ```bash
    Debug\Task2.exe
    ```
