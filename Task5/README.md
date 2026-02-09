# Backpack with Bitcoin transactions

## Algorithm & Complexity

### Implementation Logic
* **Density-based Selection:** Transactions are sorted by their "fee density" ($fee / size$). This ensures that transactions providing the highest reward per byte of block space are prioritized.
* **Size Constraint:** The algorithm iterates through the sorted list and adds transactions to the block until the next transaction would exceed the **1,000,000 bytes** (1 MB) limit.
* **Optimization:** By using `std::vector::reserve` and passing objects by reference, the program minimizes memory allocations and unnecessary data copying.

### Complexity Analysis
* **Time Complexity:** $O(N \log N)$
    * The bottleneck is the sorting process ($O(N \log N)$).
    * The linear pass to construct the block takes $O(N)$.
* **Space Complexity:** $O(N)$
    * Memory is used to store the array of `Transaction` structures. 

*Where `N` is the number of transactions in the input file.*


## How to Build and Run

Ensure you have a C++ compiler and CMake installed.

1.  **Prepare the Data:**
    Place your `transactions.csv` file in the root directory of the project.

2. **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task5
    ```
    
3. **Build using CMake:**
    ```bash
    mkdir build
    cd build
    cmake ..
    cmake --build .
    ```

4. **Run the application:**
    *On Linux/macOS:*
    ```bash
    ./Task5
    ```
    *On Windows :*
    ```bash
    .\Debug\Task5.exe
    ```

