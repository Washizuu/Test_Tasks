# Long Arithmetic 

## Algorithm & Complexity

Program implements a `BigInt` class to handle arbitrary-precision signed integers that exceed standard C++ type limits (`long long`). The implementation relies on a **High-Base Representation** ($10^9$) to optimize memory usage and performance.

### Internal Representation
* **Base $10^9$ Optimization:** instead of storing one decimal digit per array element (Base 10), the library stores **9 digits** in a single `int` element.
    * *Benefit:* Reduces memory consumption and the number of operations for arithmetic by a factor of ~9 compared to naive implementations.
* **Storage:** `std::vector<int>` is used for dynamic memory management.
* **Sign Handling:** A separate boolean flag (`is_negative`) manages the sign, while absolute values are handled by the helper class `BigNatural`.

### Complexity Analysis
* **Addition / Subtraction:** `O(N)`
    * Linear time complexity relative to the number of "digits" (blocks of 9 decimal digits).
* **Multiplication:** `O(N * M)`
    * Standard "schoolbook" multiplication algorithm.
* **Division / Modulo:** `O(N * M)`
    * Implements **Knuth’s Algorithm D** logic (using binary search for the quotient digit estimation) to efficiently handle division of large numbers.
* **Exponentiation (`pow`):** `O(M * N * log P)`
    * Uses **Binary Exponentiation** (Exponentiation by squaring) to compute powers in logarithmic time relative to the exponent.

*Where `N` and `M` are the lengths of the operands in base $10^9$.*

## Features

The library supports a wide range of operations covering standard integer behavior:

* **Arithmetic:** `+`, `-`, `*`, `/`, `%` (modulo).
* **Advanced Math:** `pow(base, exp)` — supports calculation of large powers (e.g., $2^{100}$).
* **Comparisons:** `<`, `<=`, `>`, `>=`, `==`, `!=`.
* **Conversions:**
    * `string` -> `BigInt`
    * `BigInt` -> `string`
    * `long long` -> `BigInt`
    * `BigInt` -> `long long` (with overflow protection).
* **Safety:** Exception handling for **Division by Zero** and overflow checks.

## Usage Example

```cpp
int main() {
    // Initialization from string
    BigInt a("12345678901234567890");
    BigInt b("100");
    
    // Arithmetic
    BigInt sum = a + b;
    BigInt product = a * b;
    BigInt div = a / b;
    BigInt mod = a % b;
    
    // Power
    BigInt power = BigInt::pow(a, b);
    
    // Output
    std::cout << "Sum: " << sum << std::endl;
    std::cout << "a^b: " << power << std::endl; // very long number

    return 0;
}
```
## How to Build and Run

Ensure you have a C++ compiler and CMake installed.

1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git
    cd Test_Tasks/Task9
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
    ./Task9
    ```
    *On Windows:*
    ```bash
    Debug\Task9.exe
    ```