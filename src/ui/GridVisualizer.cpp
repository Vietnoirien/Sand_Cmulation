#include "GridVisualizer.hpp"

void GridVisualizer::render() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Render each cell in the grid
    grid.forEachCell([&](uint32_t x, uint32_t y, const Particle& p) {
        if (!p.isEmpty()) {
            SDL_Rect rect = {
                static_cast<int>(x * cellSize),
                static_cast<int>(y * cellSize),
                cellSize,
                cellSize
            };
            
            // Set color based on particle type
            switch (p.type) {
                case ParticleType::SAND:
                    SDL_SetRenderDrawColor(renderer, 240, 210, 140, 255); // Sandy color
                    break;
                case ParticleType::WATER:
                    SDL_SetRenderDrawColor(renderer, 64, 164, 223, 255); // Blue
                    break;
                case ParticleType::STONE:
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Gray
                    break;
                case ParticleType::WOOD:
                    SDL_SetRenderDrawColor(renderer, 139, 69, 19, 255); // Brown
                    break;
                default:
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White
                    break;
            }
            
            SDL_RenderFillRect(renderer, &rect);
        }
    });
    
    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

void GridVisualizer::handleEvents() {
    // This is handled in the main application class
}

void GridVisualizer::run() {
    running = true;
    
    while (running) {
        handleEvents();
        render();
    }
}