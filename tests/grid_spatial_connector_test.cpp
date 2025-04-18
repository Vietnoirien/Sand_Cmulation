#include "../src/spatial/grid_spatial_connector.hpp"
#include <iostream>
#include <chrono>
#include <random>

class GridSpatialConnectorTest {
private:
    Grid grid;
    SpatialHash spatialHash;
    GridSpatialConnector connector;
    
    static const uint32_t TEST_WIDTH = 1000;
    static const uint32_t TEST_HEIGHT = 1000;
    static const size_t NUM_PARTICLES = 100000;

public:
    GridSpatialConnectorTest() 
        : grid(TEST_WIDTH, TEST_HEIGHT)
        , spatialHash()
        , connector(grid, spatialHash) {}

    void runAllTests() {
        // First populate the grid with particles
        setupInitialParticles();

        // 1. Particle Operations
        testParticleOperations();
        
        // 2. Basic Spatial Queries
        testBasicSpatialQueries();
        
        // 3. Advanced Spatial Queries
        testAdvancedSpatialQueries();
        
        // 4. Grid Properties
        testGridProperties();
        
        // 5. System Operations
        testSystemOperations();
        
        // 6. Performance Monitoring
        testPerformanceMetrics();
        
        // 7. Memory Management
        testMemoryManagement();
    }

private:
    void setupInitialParticles() {
        std::mt19937 rng(42);
        std::uniform_int_distribution<uint32_t> dist(0, TEST_WIDTH/10);
        
        // Add cluster of particles for meaningful spatial queries
        for(size_t i = 0; i < 1000; i++) {
            uint32_t x = 45 + dist(rng);
            uint32_t y = 45 + dist(rng);
            Particle p;
            connector.addParticle(x, y, p);
        }
    }
    
    void testParticleOperations() {
        // Test particle addition
        Particle p;
        connector.addParticle(10, 10, p);
        
        // Test particle movement
        bool moved = connector.moveParticle(10, 10, 11, 11);
        
        // Test neighbor access
        auto neighbors = connector.getNeighbors(11, 11);
        
        // Test particle removal
        bool removed = connector.removeParticle(11, 11);
        
        printResults("Particle Operations Test", {
            {"Particle moved", moved},
            {"Neighbors found", neighbors.size()},
            {"Particle removed", removed}
        });
    }

    void testBasicSpatialQueries() {
        bool validPos = connector.isValidPosition(50, 50);
        bool isEmpty = connector.isEmpty(50, 50);
        auto areaResults = connector.queryArea(50, 50);
        
        printResults("Basic Spatial Queries Test", {
            {"Valid position check", validPos},
            {"Empty cell check", isEmpty},
            {"Area query results", areaResults.size()}
        });
    }

    void testAdvancedSpatialQueries() {
        Vector2D pos(50.0f, 50.0f);
        Vector2D min(45.0f, 45.0f);
        Vector2D max(55.0f, 55.0f);
        
        auto radiusResults = connector.queryRadius(pos, 5.0f);
        auto boxResults = connector.queryBox(min, max);
        auto kNearest = connector.queryKNearest(pos, 10);
        auto denseRegions = connector.queryDenseRegions(4.0f);
        
        printResults("Advanced Spatial Queries Test", {
            {"Radius query results", radiusResults.size()},
            {"Box query results", boxResults.size()},
            {"K-nearest results", kNearest.size()},
            {"Dense regions found", denseRegions.size()}
        });
    }

    void testGridProperties() {
        uint32_t width = connector.getWidth();
        uint32_t height = connector.getHeight();
        
        printResults("Grid Properties Test", {
            {"Width matches", width == TEST_WIDTH},
            {"Height matches", height == TEST_HEIGHT}
        });
    }

    void testSystemOperations() {
        auto start = std::chrono::high_resolution_clock::now();
        
        connector.update();
        connector.batchSyncDirtyStates();
        connector.clear();
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        printResults("System Operations Test", {
            {"Operation time (μs)", duration.count()}
        });
    }

    void testPerformanceMetrics() {
        auto metrics = connector.getMetrics();
        connector.resetMetrics();
        
        printResults("Performance Metrics Test", {
            {"Updates processed", metrics.updates_processed},
            {"Average sync time (μs)", metrics.avg_sync_time},
            {"Peak batch size", metrics.peak_batch_size}
        });
    }

    void testMemoryManagement() {
        size_t currentUsage = connector.getCurrentMemoryUsage();
        size_t peakUsage = connector.getPeakMemoryUsage();
        auto allocMap = connector.getMemoryAllocationMap();
        
        printResults("Memory Management Test", {
            {"Current memory usage", currentUsage},
            {"Peak memory usage", peakUsage},
            {"Number of allocations", allocMap.size()}
        });
    }

    void printResults(const std::string& testName, 
                     const std::vector<std::pair<std::string, size_t>>& results) {
        std::cout << "\n" << testName << ":\n";
        for (const auto& [metric, value] : results) {
            std::cout << "- " << metric << ": " << value << "\n";
        }
        std::cout << std::endl;
    }
};
int main() {
    GridSpatialConnectorTest test;
    test.runAllTests();
    return 0;
}
