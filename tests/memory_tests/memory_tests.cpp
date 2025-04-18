#include <cassert>
#include <algorithm>
#include "MemoryPool.hpp"
#include "MemoryMonitor.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

void printTestResult(const std::string& testName, bool success) {
    std::cout << std::setw(30) << std::left << testName 
              << " : " << (success ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m") << "\n";
}

bool testMemoryPoolAllocation() {
    std::cout << "\nRunning Memory Pool Allocation Tests...\n";
    bool success = true;
    
    MemoryPool<int> pool;
    std::vector<int*> allocations;
    
    std::cout << "- Testing multiple allocations\n";
    for(int i = 0; i < 100; i++) {
        int* ptr = pool.allocate();
        *ptr = i;
        allocations.push_back(ptr);
    }
    
    std::cout << "Allocated count: " << pool.getAllocatedCount() << "\n";
    
    if(pool.getAllocatedCount() == 100) {
        std::cout << "  √ Allocation count correct\n";
    } else {
        std::cout << "  × Allocation count mismatch\n";
        success = false;
    }
    
    printTestResult("Memory Pool Allocation", success);
    return success;
}

bool testMemoryPoolDeallocation() {
    std::cout << "\nRunning Memory Pool Deallocation Tests...\n";
    bool success = true;
    
    MemoryPool<int> pool;
    std::vector<int*> allocations;
    
    // Allocate and then deallocate
    for(int i = 0; i < 50; i++) {
        allocations.push_back(pool.allocate());
    }
    
    std::cout << "- Testing deallocation\n";
    for(auto ptr : allocations) {
        pool.deallocate(ptr);
    }
    
    if(pool.getAllocatedCount() == 0) {
        std::cout << "  √ All memory properly deallocated\n";
    } else {
        std::cout << "  × Deallocation failed\n";
        success = false;
    }
    
    printTestResult("Memory Pool Deallocation", success);
    return success;
}

bool testMemoryMonitorTracking() {
    std::cout << "\nRunning Memory Monitor Tests...\n";
    bool success = true;
    
    auto& monitor = MemoryMonitor::getInstance();
    size_t initial_usage = monitor.getCurrentUsage();
    
    std::cout << "- Testing memory tracking\n";
    monitor.trackAllocation("TestComponent", 1024);
    
    size_t new_usage = monitor.getCurrentUsage();
    if(new_usage == initial_usage + 1024) {
        std::cout << "  √ Memory tracking accurate\n";
    } else {
        std::cout << "  × Memory tracking failed\n";
        success = false;
    }
    
    monitor.trackDeallocation("TestComponent", 1024);
    
    printTestResult("Memory Monitor Tracking", success);
    return success;
}

bool testMemoryPeakUsage() {
    std::cout << "\nRunning Peak Usage Tests...\n";
    bool success = true;
    
    auto& monitor = MemoryMonitor::getInstance();
    
    // Record initial state
    std::cout << "Initial peak: " << monitor.getPeakUsage() << "\n";
    
    // Test multiple allocations to verify peak tracking
    std::cout << "- Testing peak usage tracking\n";
    monitor.trackAllocation("TestComponent1", 1024);
    std::cout << "After first allocation: " << monitor.getPeakUsage() << "\n";
    
    monitor.trackAllocation("TestComponent2", 2048);
    std::cout << "After second allocation: " << monitor.getPeakUsage() << "\n";
    
    // Deallocate in reverse order
    monitor.trackDeallocation("TestComponent2", 2048);
    std::cout << "After first deallocation: " << monitor.getPeakUsage() << "\n";
    
    monitor.trackDeallocation("TestComponent1", 1024);
    std::cout << "Final peak: " << monitor.getPeakUsage() << "\n";
    
    // Peak should remain at highest point (3072) even after deallocations
    if(monitor.getPeakUsage() >= 3072) {
        std::cout << "  √ Peak usage correctly maintained\n";
    } else {
        std::cout << "  × Peak usage tracking failed\n";
        std::cout << "    Expected peak >= 3072, Got: " << monitor.getPeakUsage() << "\n";
        success = false;
    }
    
    printTestResult("Peak Usage Tracking", success);
    return success;
}

int main() {
    std::cout << "\n=== Starting Memory Management Tests ===\n";
    
    std::vector<std::pair<std::string, bool>> results = {
        {"Memory Pool Allocation", testMemoryPoolAllocation()},
        {"Memory Pool Deallocation", testMemoryPoolDeallocation()},
        {"Memory Monitor Tracking", testMemoryMonitorTracking()},
        {"Peak Usage Tracking", testMemoryPeakUsage()}
    };
    
    int totalTests = results.size();
    int passedTests = std::count_if(results.begin(), results.end(), 
                                  [](const auto& r) { return r.second; });
    
    std::cout << "\n=== Test Summary ===\n";
    std::cout << "Total Tests    : " << totalTests << "\n";
    std::cout << "Passed Tests   : " << passedTests << "\n";
    std::cout << "Failed Tests   : " << (totalTests - passedTests) << "\n";
    std::cout << "Success Rate   : " << std::fixed << std::setprecision(1) 
              << (passedTests * 100.0 / totalTests) << "%\n\n";
    
    return passedTests == totalTests ? 0 : 1;
}
