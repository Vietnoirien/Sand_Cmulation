#include <cassert>
#include "Grid.hpp"
#include "GridOperations.hpp"
#include "Particle.hpp"
#include <iostream>
#include <iomanip>
#include <vector>

void printTestResult(const std::string& testName, bool success) {
    std::cout << std::setw(30) << std::left << testName 
              << " : " << (success ? "\033[32mPASSED\033[0m" : "\033[31mFAILED\033[0m") << "\n";
}

bool testParticleCreation() {
    std::cout << "\nRunning Particle Creation Tests...\n";
    bool success = true;
    
    std::cout << "- Testing empty particle properties\n";
    Particle empty;
    if (empty.isEmpty() && empty.type == ParticleType::EMPTY && empty.mass == 0) {
        std::cout << "  √ Empty particle correctly initialized\n";
    } else {
        std::cout << "  × Empty particle initialization failed\n";
        success = false;
    }
    
    std::cout << "- Testing sand particle properties\n";
    Particle sand(ParticleType::SAND, 100);
    if (!sand.isEmpty() && sand.type == ParticleType::SAND && 
        sand.mass == 100 && sand.velocity_x == 0 && sand.velocity_y == 0) {
        std::cout << "  √ Sand particle correctly initialized\n";
    } else {
        std::cout << "  × Sand particle initialization failed\n";
        success = false;
    }
    
    printTestResult("Particle Creation", success);
    return success;
}

bool testBasicParticleMovement() {
    std::cout << "\nRunning Basic Movement Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing particle placement\n";
    grid.update(5, 5, Particle(ParticleType::SAND));
    if (grid.at(5, 5).type == ParticleType::SAND) {
        std::cout << "  √ Particle placed correctly\n";
    } else {
        std::cout << "  × Particle placement failed\n";
        success = false;
    }
    
    std::cout << "- Testing particle movement\n";
    bool moved = ops.moveParticle(5, 5, 5, 6);
    if (moved && grid.at(5, 6).type == ParticleType::SAND && grid.at(5, 5).isEmpty()) {
        std::cout << "  √ Particle moved correctly\n";
    } else {
        std::cout << "  × Particle movement failed\n";
        success = false;
    }
    
    printTestResult("Basic Movement", success);
    return success;
}

bool testParticleCollision() {
    std::cout << "\nRunning Collision Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Setting up collision scenario\n";
    grid.update(5, 5, Particle(ParticleType::SAND));
    grid.update(5, 6, Particle(ParticleType::STONE));
    
    std::cout << "- Testing collision prevention\n";
    bool moved = ops.moveParticle(5, 5, 5, 6);
    if (!moved && grid.at(5, 5).type == ParticleType::SAND && 
        grid.at(5, 6).type == ParticleType::STONE) {
        std::cout << "  √ Collision prevented correctly\n";
    } else {
        std::cout << "  × Collision prevention failed\n";
        success = false;
    }
    
    printTestResult("Collision Detection", success);
    return success;
}

bool testGridBoundaries() {
    std::cout << "\nRunning Boundary Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing grid boundaries\n";
    grid.update(0, 0, Particle(ParticleType::SAND));
    
    bool moved_left = ops.moveParticle(0, 0, -1, 0);
    bool moved_up = ops.moveParticle(0, 0, 0, -1);
    
    if (!moved_left && !moved_up && grid.at(0, 0).type == ParticleType::SAND) {
        std::cout << "  √ Boundary constraints working\n";
    } else {
        std::cout << "  × Boundary constraints failed\n";
        success = false;
    }
    
    printTestResult("Grid Boundaries", success);
    return success;
}

bool testNeighborAccess() {
    std::cout << "\nRunning Neighbor Access Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing center position neighbors\n";
    auto center_neighbors = ops.getNeighbors(5, 5);
    if (center_neighbors.size() == 8) {
        std::cout << "  √ Center position has correct neighbor count\n";
    } else {
        std::cout << "  × Center position neighbor count incorrect\n";
        success = false;
    }
    
    std::cout << "- Testing corner position neighbors\n";
    auto corner_neighbors = ops.getNeighbors(0, 0);
    if (corner_neighbors.size() == 3) {
        std::cout << "  √ Corner position has correct neighbor count\n";
    } else {
        std::cout << "  × Corner position neighbor count incorrect\n";
        success = false;
    }
    
    printTestResult("Neighbor Access", success);
    return success;
}

bool testDirtyStateTracking() {
    std::cout << "\nRunning Dirty State Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    
    std::cout << "- Testing initial clean state\n";
    if (grid.getDirtyIndices().empty()) {
        std::cout << "  √ Grid starts with clean state\n";
    } else {
        std::cout << "  × Initial clean state failed\n";
        success = false;
    }
    
    std::cout << "- Testing dirty state after update\n";
    grid.update(5, 5, Particle(ParticleType::SAND));
    if (grid.getDirtyIndices().size() == 1) {
        std::cout << "  √ Update correctly marks cell as dirty\n";
    } else {
        std::cout << "  × Dirty state tracking failed\n";
        success = false;
    }
    
    std::cout << "- Testing dirty state clearing\n";
    grid.clearDirtyStates();
    if (grid.getDirtyIndices().empty()) {
        std::cout << "  √ Dirty states cleared correctly\n";
    } else {
        std::cout << "  × Dirty state clearing failed\n";
        success = false;
    }
    
    printTestResult("Dirty State Tracking", success);
    return success;
}

int main() {
    std::cout << "\n=== Starting Particle System Tests ===\n";
    
    std::vector<std::pair<std::string, bool>> results = {
        {"Particle Creation", testParticleCreation()},
        {"Basic Movement", testBasicParticleMovement()},
        {"Collision Detection", testParticleCollision()},
        {"Grid Boundaries", testGridBoundaries()},
        {"Neighbor Access", testNeighborAccess()},
        {"Dirty State Tracking", testDirtyStateTracking()}
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
