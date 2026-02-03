# Website Analytics

## Algorithm & Complexity

The application calculates user retention and discovery metrics using efficient Hash Map (Dictionary) and Hash Set structures. It performs a single pass over the first dataset to build an index and a streaming pass over the second dataset to filter results.

-   **Data Indexing (Day 1):** `O(N)`
    -   The script reads the first file line by line.
    -   It builds a Hash Map (`dict`) where keys are `user_id` and values are Hash Sets of `product_id`.
    -   Insertion into a Hash Map/Set takes `O(1)` on average.
-   **Filtering (Day 2):** `O(M)`
    -   The script reads the second file line by line without loading it entirely into memory (streaming).
    -    Checks if the user exists in the Day 1 index (`O(1)`).
    -   Checks if the product is *not* in the user's Day 1 history (`O(1)`).
-   **Space Complexity:** `O(N)`
    -   Only data from the first day is stored in RAM. The second file is processed sequentially, requiring minimal memory overhead.

*Where `N` is the number of records in the first file and `M` is the number of records in the second file.*

The application accepts input from the console. You must provide paths to two CSV files as arguments.

**Input CSV Structure:**
`user_id,product_id,timestamp`


## How to Build and Run

Ensure you have **Python 3.6+** installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git 
    cd Test_Tasks/Task3
    ```

2. **Run the application:**
    You need to provide two CSV files as arguments.
    
    *On Linux/macOS:*
    ```bash
    python3 main.py day1.csv day2.csv
    ```
    *On Windows:*
    ```bash
    python main.py day1.csv day2.csv
    ```