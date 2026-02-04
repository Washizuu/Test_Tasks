# Pinatas

## Algorithm & Complexity

The application calculates the maximum amount of candies obtainable by strategically smashing pinatas.

-   **Algorithm (Dynamic Programming):** `O(N³)`
    -   Instead of deciding which pinata to smash first, the script determines which pinata should be smashed **last** for every possible interval.
    -   It utilizes three nested loops: iterating over the interval length, the starting index, and the choice of the "last" pinata within that interval.
    -   The recurrence relation for an interval `(i, j)` with the last pinata `k` is:  
        `temp_results[i][j] = max(pinatas[i-1] * pinatas[k] * pinatas[j+1] + temp_results[i][k-1] + temp_results[k+1][j],temp_results[i][j])`.
-   **Space Complexity:** `O(N²)`
    -   Intermediate results are stored in a 2D matrix of size `(N+2) x (N+2)`.
    -   This prevents redundant calculations through memoization.

*Where `N` is the number of pinatas in the input array.*

The application accepts input from the console via command-line arguments.

## How to Build and Run

Ensure you have **Python 3.6+** installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task7
    ```

2.  **Run the application:**
    You need to provide pinata values as integer arguments separated by spaces.
    
    *On Linux/macOS:*
    ```bash
    python3 main.py <num1> <num2> ...
    ```
    *On Windows:*
    ```bash
    python main.py <num1> <num2> ...
    ```

