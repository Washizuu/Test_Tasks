#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <iomanip>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#else
    #include <sys/resource.h>
#endif

struct Transaction {
    std::string id;
    int size;
    long long fee;
    double density;
};

struct BlockResult {
    std::vector<std::string> tx_ids;
    int total_size;
    long long total_fee;
    double duration_ms;
    size_t peak_memory;
};

size_t getPeakMemory() {
#ifdef _WIN32
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo(GetCurrentProcess(), &info, sizeof(info));
    return (size_t)info.PeakWorkingSetSize;
#else
    struct rusage r_usage;
    getrusage(RUSAGE_SELF, &r_usage);
    return (size_t)(r_usage.ru_maxrss * 1024);
#endif
}

std::vector<Transaction> loadTransactions(const std::string& filename) {
    std::vector<Transaction> txs;
    std::ifstream file(filename);
    if (!file.is_open()) return txs;

    std::string line, header;
    std::getline(file, header);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string id, s_size, s_fee;

        if (std::getline(ss, id, ',') && std::getline(ss, s_size, ',') && std::getline(ss, s_fee, ',')) {
            int size = std::stoi(s_size);
            long long fee = std::stoll(s_fee);
            txs.push_back({id, size, fee, (double)fee / size});
        }
    }
    return txs;
}

BlockResult constructBlock(std::vector<Transaction>& txs, int max_capacity) {
    auto start = std::chrono::high_resolution_clock::now();

    std::sort(txs.begin(), txs.end(), [](const Transaction& a, const Transaction& b) {
        return a.density > b.density;
    });

    std::vector<std::string> selected_ids;
    int current_size = 0;
    long long current_fee = 0;

    for (const auto& tx : txs) {
        if (current_size + tx.size <= max_capacity) {
            selected_ids.push_back(tx.id);
            current_size += tx.size;
            current_fee += tx.fee;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    return {
        selected_ids,
        current_size,
        current_fee,
        std::chrono::duration<double, std::milli>(end - start).count(),
        getPeakMemory()
    };
}

void printReport(const BlockResult& res) {
    std::cout << "BITCOIN BLOCK CONSTRUCTION REPORT" << std::endl;
    std::cout << "Transactions in block: " << res.tx_ids.size() << std::endl;
    std::cout << "Total Block Size: " << res.total_size << " / 1000000 bytes" << std::endl;
    std::cout << "Total Fee Extracted: " << res.total_fee << " satoshis" << std::endl;
    std::cout << "Construction  Time: " << std::fixed << std::setprecision(4) << res.duration_ms << " ms" << std::endl;
    std::cout << "Peak Memory Usage: " << res.peak_memory / 1024 / 1024 << " MB" << std::endl;
}

int main() {
    const int MB = 1000000;

    std::vector<Transaction> transactions = loadTransactions("transactions.csv");
    if (transactions.empty()) {
        std::cerr << "No data found or file error." << std::endl;
        return 1;
    }

    BlockResult result = constructBlock(transactions, MB);

    printReport(result);
    return 0;
}