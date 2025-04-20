# Sand_Cmulation

A high-performance particle simulation system with advanced spatial partitioning in C++.

## Overview

Sand_Cmulation is a CS50X final project that implements a particle-based sand simulation with efficient spatial queries and grid operations. The system uses a combination of grid-based storage and spatial hashing to achieve high-performance particle operations.

## Features

- High-performance spatial partitioning with O(1) lookups
- Thread-safe operations with OpenMP parallelization
- Advanced spatial queries (radius, box, k-nearest neighbors)
- Memory-efficient particle storage
- Real-time memory usage monitoring
- Optimized batch operations

## Current Status

The project is currently in development. The grid system is visually functional, demonstrating the basic particle simulation capabilities.

## Grid System

The core of the simulation is the `GridSpatialConnector` which provides a unified interface between the grid storage and spatial hash systems:

### Key Components

- **Grid**: Stores particles in a 2D array
- **SpatialHash**: Provides O(1) spatial lookups
- **QuerySystem**: Handles advanced spatial queries
- **GridOperations**: Manages grid-level operations

### Performance Metrics

The following metrics were measured on an i9 9900KF processor with DDR4 @ 3200MHz:

- Insertion: ~613k particles/second
- Queries: ~49k queries/second
- Batch sync: ~2.4M updates/second
- Movement: ~79k moves/second

## Usage Examples

```cpp
// Initialize systems
Grid grid(1000, 1000);
SpatialHash spatialHash;
GridSpatialConnector connector(grid, spatialHash);

// Basic particle operations
Particle sand;
connector.addParticle(x, y, sand);
connector.moveParticle(oldX, oldY, newX, newY);

// Get neighboring cells
auto neighbors = connector.getNeighbors(x, y);

// Advanced spatial queries
Vector2D pos(x, y);
float radius = 5.0f;
auto nearbyParticles = connector.queryRadius(pos, radius);

// Box query
Vector2D min(x1, y1), max(x2, y2);
auto particlesInBox = connector.queryBox(min, max);

// K-nearest neighbors
auto nearestParticles = connector.queryKNearest(pos, 10);
```

## API Categories

### Particle Operations
- `addParticle()`: Add new particle
- `removeParticle()`: Remove particle
- `moveParticle()`: Move particle with boundary checking
- `getNeighbors()`: Get valid neighboring cells

### Basic Spatial Queries
- `queryArea()`: Simple area query
- `isValidPosition()`: Position validation
- `isEmpty()`: Cell emptiness check

### Advanced Spatial Queries
- `queryRadius()`: Radius-based search
- `queryRadiusFiltered()`: Filtered radius search
- `queryBox()`: Box-bounded search
- `queryKNearest()`: K-nearest neighbors
- `queryDenseRegions()`: Density-based search

### Grid Properties
- `getWidth()`: Grid width
- `getHeight()`: Grid height
- `getParticle()`: Particle access

### System Operations
- `update()`: System sync
- `clear()`: System reset
- `batchSyncDirtyStates()`: Force sync

### Performance Monitoring
- `getMetrics()`: Performance metrics
- `resetMetrics()`: Reset counters

### Memory Management
- `getCurrentMemoryUsage()`: Get current memory usage
- `getPeakMemoryUsage()`: Get peak memory usage
- `getMemoryAllocationMap()`: Get detailed memory allocation map

## Current Interface

The current user interface is minimal and designed primarily to visually verify that the grid system is working correctly. As this is a CS50X final project, the focus has been on implementing the core simulation functionality rather than a polished UI.

## Future Improvements

- Implement a more user-friendly interface
- Add more particle types and interactions
- Optimize memory usage for larger simulations
- Add persistence for saving/loading simulation states

## Building the Project

```bash
make
```

## Running the Simulation

```bash
./sand_simulation
```

## Requirements

- C++17 compatible compiler
- OpenMP for parallel processing
- SDL2 library
