#include <cassert>
#include "Grid.hpp"
#include "SpatialHash.hpp"
#include "QuerySystem.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

void printTestResult(const std::string& testName, bool success) {
    std::cout << std::setw(30) << std::left << testName 
              << " : " << (success ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m") << "\n";
}

bool testSpatialHashInsertion() {
    std::cout << "\nRunning Spatial Hash Insertion Tests...\n";
    bool success = true;
    
    SpatialHash hash;
    Grid grid(100, 100);
    
    // Calculate correct cell coordinates
    uint32_t cell_x = 50;
    uint32_t cell_y = 50;
    ParticleRef particle(&grid, cell_x, cell_y);
    
    std::cout << "- Testing single particle insertion\n";
    hash.insert(particle, cell_x, cell_y);
    
    // Query using the exact cell coordinates
    auto results = hash.query(cell_x, cell_y);
    
    if (results.size() == 1 && 
        results[0].getX() == cell_x && 
        results[0].getY() == cell_y) {
        std::cout << "  √ Particle inserted correctly\n";
    } else {
        std::cout << "  × Particle insertion failed\n";
        success = false;
    }
    
    printTestResult("Spatial Hash Insertion", success);
    return success;
}
bool testSpatialHashRemoval() {
    std::cout << "\nRunning Spatial Hash Removal Tests...\n";
    bool success = true;
    
    SpatialHash hash;
    Grid grid(100, 100);
    ParticleRef particle(&grid, 50, 50);
    
    hash.insert(particle, 50, 50);
    std::cout << "- Testing particle removal\n";
    hash.remove(particle, 50, 50);
    
    auto results = hash.query(50/SpatialHash::CELL_SIZE, 50/SpatialHash::CELL_SIZE);
    if (results.empty()) {
        std::cout << "  √ Particle removed correctly\n";
    } else {
        std::cout << "  × Particle removal failed\n";
        success = false;
    }
    
    printTestResult("Spatial Hash Removal", success);
    return success;
}

bool testSpatialHashQuery() {
    std::cout << "\nRunning Spatial Query Tests...\n";
    bool success = true;
    
    SpatialHash hash;
    Grid grid(100, 100);
    
    // Debug coordinates and hash values
    uint32_t cell_x = 50;
    uint32_t cell_y = 50;
    uint64_t cell_hash = hash.hashPos(cell_x, cell_y);
    
    std::cout << "Debug Info:\n";
    std::cout << "Cell coordinates: (" << cell_x << "," << cell_y << ")\n";
    std::cout << "Cell hash: " << cell_hash << "\n";
    
    // Insert particles with position tracking
    for(int i = 0; i < 5; i++) {
        uint32_t pos_x = 50 + i;
        uint32_t pos_y = 50;
        ParticleRef particle(&grid, pos_x, pos_y);
        
        uint64_t particle_hash = hash.hashPos(pos_x, pos_y);
        std::cout << "Inserting particle " << i << " at (" << pos_x << "," << pos_y 
                 << ") hash: " << particle_hash << "\n";
        
        hash.insert(particle, pos_x, pos_y);
    }
    
    std::cout << "- Testing cell query\n";
    auto results = hash.query(cell_x, cell_y);
    
    std::cout << "Query results size: " << results.size() << "\n";
    
    if (results.size() == 5) {
        std::cout << "  √ Cell query returned correct count\n";
        // Verify particle positions
        for(const auto& p : results) {
            std::cout << "Found particle at (" << p.getX() << "," << p.getY() << ")\n";
        }
    } else {
        std::cout << "  × Cell query count incorrect\n";
        std::cout << "    Expected: 5, Got: " << results.size() << "\n";
        success = false;
    }
    
    printTestResult("Spatial Hash Query", success);
    return success;
}

bool testSpatialHashCollisions() {
    std::cout << "\nRunning Hash Collision Tests...\n";
    bool success = true;
    
    SpatialHash hash;
    Grid grid(100, 100);
    
    // Insert particles in different cells that might hash to same bucket
    ParticleRef p1(&grid, 0, 0);
    ParticleRef p2(&grid, 64, 64); // Different cell but potential hash collision
    
    uint64_t hash1 = hash.hashPos(0, 0);
    uint64_t hash2 = hash.hashPos(64, 64);
    
    std::cout << "Hash values:\n";
    std::cout << "Particle 1 (0,0): " << hash1 << "\n";
    std::cout << "Particle 2 (64,64): " << hash2 << "\n";
    
    hash.insert(p1, 0, 0);
    hash.insert(p2, 64, 64);
    
    std::cout << "- Testing collision handling\n";
    auto results1 = hash.query(0, 0);
    auto results2 = hash.query(64, 64);
    
    std::cout << "Query results:\n";
    std::cout << "Cell (0,0): " << results1.size() << " particles\n";
    std::cout << "Cell (64,64): " << results2.size() << " particles\n";
    
    if (results1.size() == 1 && results2.size() == 1) {
        std::cout << "  √ Hash collisions handled correctly\n";
    } else {
        std::cout << "  × Hash collision handling failed\n";
        success = false;
    }
    
    printTestResult("Hash Collision Handling", success);
    return success;
}

int main() {
    std::cout << "\n=== Starting Spatial Hash Tests ===\n";
    
    std::vector<std::pair<std::string, bool>> results = {
        {"Spatial Hash Insertion", testSpatialHashInsertion()},
        {"Spatial Hash Removal", testSpatialHashRemoval()},
        {"Spatial Query", testSpatialHashQuery()},
        {"Hash Collision Handling", testSpatialHashCollisions()}
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
