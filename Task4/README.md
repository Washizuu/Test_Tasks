# Hitler Crawler

## Features & Implementation

* **Algorithm (BFS):** The program uses a **Breadth-First Search** to guarantee the shortest possible path between the starting article and the target.
* **Concurrency (Multithreading):** Program utilizes `ThreadPoolExecutor` for scanning multiple Wikipedia pages simultaneously.
* **Data Persistence (JSON Logging):** A local `wiki_cache.json` file acts as a persistent cache. If a page has been scanned before, the script retrieves links from the local disk instead of making a new HTTP request.
* **Thread Safety:** Implements `threading.Lock` to ensure that the shared Cache Manager and "Visited" sets are updated correctly without data corruption during parallel execution.
## How to Build and Run

Ensure you have **Python 3.8+** installed.

This project requires `requests` for networking and `BeautifulSoup4` for HTML parsing.
```bash
pip install requests beautifulsoup4
```
1.  **Clone the repository:**
    ```bash
    git clone https://github.com/Washizuu/Test_Tasks.git 
    cd Test_Tasks/Task4
    ```

2. **Run the application:**
    
    *On Linux/macOS:*
    ```bash
    python3 main.py 
    ```
    *On Windows:*
    ```bash
    python main.py 
    ```