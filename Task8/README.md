# Student's Capital 

## Algorithm & Complexity

The application maximizes the investment capital using an efficient Greedy approach combined with a Max-Heap (Priority Queue) and Sorting. It performs a single sort of the inventory and uses a heap to constantly retrieve the most profitable affordable project.

-   **Data Preparation (Sorting):** `O(K log K)`
    -   The script combines prices and profits into pairs.
    -   It sorts all `K` available laptops by price in ascending order.
    -   Sorting allows us to efficiently identify affordable laptops without re-scanning the entire list.
-   **Greedy Selection (Processing):** `O(N log K)`
    -   The script iterates `N` times (maximum number of transactions).
    -   In each iteration, it pushes all affordable laptops into a Max-Heap (`O(log K)` per insertion).
    -   It extracts the laptop with the maximum profit from the Heap (`O(log K)`).
-   **Space Complexity:** `O(K)`
    -   We store the list of projects and the heap structure in memory.

*Where `N` is the maximum number of transactions and `K` is the total number of available laptops.*

The application accepts input from the console interactively.

**Input Format:**
1.  **N** (Integer): Maximum number of laptops to buy.
2.  **C** (Integer): Initial Capital.
3.  **Prices** (String): Space-separated list of integers (e.g., `0 1 2`).
4.  **Gains** (String): Space-separated list of integers (e.g., `1 2 3`).

## How to Build and Run

Ensure you have **Python 3.6+** installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task8
    ```

2. **Run the application:**
    Run the script and follow the interactive prompts.
    
    *On Linux/macOS:*
    ```bash
    python3 main.py
    ```
    *On Windows:*
    ```bash
    python main.py
    ```

