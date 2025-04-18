#include <chrono>
#include <iostream>
#include <iomanip>
#include "Grid.hpp"
#include "SpatialHash.hpp"
#include "MemoryMonitor.hpp"
#include "MemoryPool.hpp"
#include <omp.h>


class PerformanceMetrics {
private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = Clock::time_point;
    
    std::string testName;
    size_t operationCount;
    TimePoint startTime;

public:
    PerformanceMetrics(const std::string& name) 
        : testName(name)
        , operationCount(0)
        , startTime(Clock::now())
    {}
    
    void recordOperation() {
        operationCount++;
    }
    
    void printResults() {
        auto endTime = Clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
            endTime - startTime
        );
        
        double opsPerSecond = operationCount / (duration.count() / 1000000.0);
        
        std::cout << "\n=== " << testName << " Performance Results ===\n";
        std::cout << "Total Operations: " << operationCount << "\n";
        std::cout << "Duration (μs): " << duration.count() << "\n";
        std::cout << "Operations/second: " << std::fixed << std::setprecision(2) 
                 << opsPerSecond << "\n";
    }
};
void testGridPerformance() {
    PerformanceMetrics metrics("Grid Operations");
    Grid grid(1000, 1000);
    
    // Test particle updates
    for(uint32_t i = 0; i < 1000; i++) {
        for(uint32_t j = 0; j < 1000; j++) {
            grid.update(i, j, Particle(ParticleType::SAND));
            metrics.recordOperation();
        }
    }
    
    metrics.printResults();
}

void testSpatialHashPerformance() {
    PerformanceMetrics metrics("Spatial Hash Operations");
    SpatialHash hash;
    Grid grid(1000, 1000);
    
    // Test insertions and queries
    for(uint32_t i = 0; i < 1000; i++) {
        ParticleRef particle(&grid, i, i);
        hash.insert(particle, i, i);
        hash.query(i, i);
        metrics.recordOperation();
    }
    
    metrics.printResults();
}

void testMemoryAllocationPerformance() {
    PerformanceMetrics metrics("Memory Operations");
    MemoryPool<Particle> pool;
    std::vector<Particle*> allocations;
    allocations.reserve(10000); // Pre-reserve to avoid reallocation
    
    // First phase: Allocations
    for(int i = 0; i < 10000; i++) {
        Particle* ptr = pool.allocate();
        if(ptr) {
            *ptr = Particle(ParticleType::SAND); // Initialize particle
            allocations.push_back(ptr);
            metrics.recordOperation();
        }
    }
    
    // Second phase: Deallocations
    // Only deallocate pointers we own
    while(!allocations.empty()) {
        Particle* ptr = allocations.back();
        allocations.pop_back();
        pool.deallocate(ptr);
        metrics.recordOperation();
    }
    
    metrics.printResults();
}

int main() {
    std::cout << "=== Starting Performance Benchmarks ===\n";
    
    testGridPerformance();
    testSpatialHashPerformance();
    testMemoryAllocationPerformance();
    
    auto& monitor = MemoryMonitor::getInstance();
    std::cout << "\n=== Memory Usage Statistics ===\n";
    std::cout << "Peak Memory Usage: " << monitor.getPeakUsage() << " bytes\n";
    std::cout << "Current Memory Usage: " << monitor.getCurrentUsage() << " bytes\n";
    
    return 0;
}
