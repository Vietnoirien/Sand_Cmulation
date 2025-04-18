#include "SandSimulation.hpp"
#include <SDL2/SDL.h>
#include <cmath>

void SandSimulation::handleEvents() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        // Handle key presses
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_1:
                    setParticleType(ParticleType::SAND);
                    break;
                case SDLK_2:
                    setParticleType(ParticleType::WATER);
                    break;
                case SDLK_3:
                    setParticleType(ParticleType::STONE);
                    break;
                case SDLK_4:
                    setParticleType(ParticleType::WOOD);
                    break;
                case SDLK_PLUS:
                case SDLK_EQUALS:
                    setBrushSize(brushSize + 1);
                    break;
                case SDLK_MINUS:
                    setBrushSize(std::max(1, brushSize - 1));
                    break;
                case SDLK_c:
                    connector->clear();
                    break;
            }
        }
        
        // Handle mouse input for drawing particles
        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEMOTION) {
            if (event.motion.state & SDL_BUTTON_LMASK) {
                int mouseX = event.motion.x / cellSize;
                int mouseY = event.motion.y / cellSize;
                
                addParticlesInRadius(mouseX, mouseY, brushSize);
            }
        }
    }
}

void SandSimulation::update() {
    // Apply physics and update the grid
    // For now, just a simple gravity effect for sand particles
    
    uint32_t width = connector->getWidth();
    uint32_t height = connector->getHeight();
    
    // Process from bottom to top for better gravity simulation
    for (int y = height - 2; y >= 0; y--) {
        for (uint32_t x = 0; x < width; x++) {
            Particle& p = connector->getParticle(x, y);
            
            if (p.isEmpty()) continue;
            
            // Simple physics based on particle type
            switch (p.type) {
                case ParticleType::SAND:
                    // Try to move down
                    if (y < height - 1 && connector->isEmpty(x, y + 1)) {
                        connector->moveParticle(x, y, x, y + 1);
                    }
                    // Try to move diagonally down
                    else if (y < height - 1) {
                        bool movedLeft = false;
                        bool movedRight = false;
                        
                        // Try left diagonal
                        if (x > 0 && connector->isEmpty(x - 1, y + 1)) {
                            connector->moveParticle(x, y, x - 1, y + 1);
                            movedLeft = true;
                        }
                        
                        // Try right diagonal if didn't move left
                        if (!movedLeft && x < width - 1 && connector->isEmpty(x + 1, y + 1)) {
                            connector->moveParticle(x, y, x + 1, y + 1);
                            movedRight = true;
                        }
                    }
                    break;
                    
                case ParticleType::WATER:
                    // Water has more horizontal spread
                    // Try to move down
                    if (y < height - 1 && connector->isEmpty(x, y + 1)) {
                        connector->moveParticle(x, y, x, y + 1);
                    }
                    // Try to move diagonally down
                    else if (y < height - 1) {
                        bool moved = false;
                        
                        // Try left diagonal
                        if (x > 0 && connector->isEmpty(x - 1, y + 1)) {
                            connector->moveParticle(x, y, x - 1, y + 1);
                            moved = true;
                        }
                        
                        // Try right diagonal
                        if (!moved && x < width - 1 && connector->isEmpty(x + 1, y + 1)) {
                            connector->moveParticle(x, y, x + 1, y + 1);
                            moved = true;
                        }
                        
                        // Try horizontal movement if didn't move down
                        if (!moved) {
                            // Randomly choose direction
                            bool goLeft = (rand() % 2 == 0);
                            
                            if (goLeft && x > 0 && connector->isEmpty(x - 1, y)) {
                                connector->moveParticle(x, y, x - 1, y);
                            } else if (!goLeft && x < width - 1 && connector->isEmpty(x + 1, y)) {
                                connector->moveParticle(x, y, x + 1, y);
                            }
                        }
                    }
                    break;
                    
                // Stone and wood don't move
                case ParticleType::STONE:
                case ParticleType::WOOD:
                default:
                    break;
            }
        }
    }
    
    // Update the connector to sync grid and spatial hash
    connector->update();
}

void SandSimulation::render() {
    visualizer->render();
}

void SandSimulation::addParticlesInRadius(int centerX, int centerY, int radius) {
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            // Calculate distance from center
            float distance = std::sqrt(dx*dx + dy*dy);
            
            // Only add particles within the radius
            if (distance <= radius) {
                int x = centerX + dx;
                int y = centerY + dy;
                
                // Check if position is valid
                if (connector->isValidPosition(x, y) && connector->isEmpty(x, y)) {
                    // Create particle based on current type
                    Particle p(currentParticleType);
                    
                    // Set mass based on type
                    switch (currentParticleType) {
                        case ParticleType::SAND:
                            p.mass = 100;
                            break;
                        case ParticleType::WATER:
                            p.mass = 50;
                            break;
                        case ParticleType::STONE:
                            p.mass = 200;
                            break;
                        case ParticleType::WOOD:
                            p.mass = 150;
                            break;
                        default:
                            p.mass = 1;
                            break;
                    }
                    
                    connector->addParticle(x, y, p);
                }
            }
        }
    }
}