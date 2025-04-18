#include "app/SandSimulation.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    try {
        // Create a simulation with 800x600 window and 5px cell size
        SandSimulation simulation(800, 600, 5);
        
        // Run the simulation
        simulation.run();
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}