CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fopenmp
INCLUDES = -Isrc/grid -Isrc/particle -Isrc/spatial -Isrc/app -Isrc/ui
LIBS = -lSDL2 -fopenmp

TARGET = sand_simulation
SRCS = src/main.cpp
# Additional source files as needed
SRCS += src/app/SandSimulation.cpp
SRCS += src/ui/GridVisualizer.cpp

OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LIBS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean