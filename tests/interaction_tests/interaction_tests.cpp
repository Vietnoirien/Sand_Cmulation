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

bool testSandPileFormation() {
    std::cout << "\nRunning Sand Pile Formation Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing vertical stacking\n";
    // Place sand particles vertically
    grid.update(5, 5, Particle(ParticleType::SAND));
    grid.update(5, 4, Particle(ParticleType::SAND));
    
    // Try moving bottom particle
    bool moved = ops.moveParticle(5, 5, 5, 6);
    if (!moved) {
        std::cout << "  √ Sand particles stack correctly\n";
    } else {
        std::cout << "  × Sand stacking failed\n";
        success = false;
    }
    
    printTestResult("Sand Pile Formation", success);
    return success;
}

bool testParticlePriority() {
    std::cout << "\nRunning Particle Priority Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing sand vs stone interaction\n";
    grid.update(5, 5, Particle(ParticleType::SAND));
    grid.update(5, 6, Particle(ParticleType::STONE));
    
    bool moved = ops.moveParticle(5, 5, 5, 6);
    if (!moved) {
        std::cout << "  √ Stone blocks sand movement\n";
    } else {
        std::cout << "  × Particle priority failed\n";
        success = false;
    }
    
    printTestResult("Particle Priority", success);
    return success;
}

bool testDiagonalFlow() {
    std::cout << "\nRunning Diagonal Flow Tests...\n";
    bool success = true;
    
    Grid grid(10, 10);
    GridOperations ops(grid);
    
    std::cout << "- Testing diagonal movement\n";
    grid.update(5, 5, Particle(ParticleType::SAND));
    grid.update(5, 6, Particle(ParticleType::STONE));
    
    // Sand should flow diagonally when blocked
    bool moved = ops.moveParticle(5, 5, 4, 6);
    if (moved && grid.at(4, 6).type == ParticleType::SAND) {
        std::cout << "  √ Diagonal flow works correctly\n";
    } else {
        std::cout << "  × Diagonal flow failed\n";
        success = false;
    }
    
    printTestResult("Diagonal Flow", success);
    return success;
}

int main() {
    std::cout << "\n=== Starting Particle Interaction Tests ===\n";
    
    std::vector<std::pair<std::string, bool>> results = {
        {"Sand Pile Formation", testSandPileFormation()},
        {"Particle Priority", testParticlePriority()},
        {"Diagonal Flow", testDiagonalFlow()}
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
