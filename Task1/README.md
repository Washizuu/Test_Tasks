# Orderbook

## Algorithm & Complexity 

The order book uses `std::map` to store price levels and `std::list` for the queue of orders at each price level.

-   **Add Order:** `O(log N)`
    -   Finding the correct price level in `std::map` takes logarithmic time.
    -   Appending to the `std::list` takes $O(1)$.
- **Match Order:** `O(1)` per trade | `O(k)` total.
    - **Per Trade:** Accessing the top of the asks/bids and removing an order takes constant time `O(1)` due to `std::list` efficiency.
    - **Total Execution:** The complexity is `O(k)`, where `k` is the number of executed trades. This depends only on the trade volume, not on the total number of orders in the book (`N`).

## Usage Example

The application accepts input from the console in the following format:
`[User ID] [Amount] [Price] [Side: 1=Buy, 0=Sell]`

**Example Session:**

```text
>1 100 40 0

Order placed in ASKS (rest: 100)
>2 50 39 0

Order placed in ASKS (rest: 50)
>3 60 41 1

--- MATCH EXECUTED (50 UAH @ 39 USD) ---
User 3: +50 UAH
User 3: -1950 USD
User 2: -50 UAH
User 2: +1950 USD
------------------------------------------
--- MATCH EXECUTED (10 UAH @ 40 USD) ---
User 3: +10 UAH
User 3: -400 USD
User 1: -10 UAH
User 1: +400 USD
------------------------------------------
``` 

## How to Build and Run

Ensure you have a C++ compiler and CMake installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task1
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
    ./Task1
    ```
    *On Windows:*
    ```bash
    Debug\Task1.exe
    ```
