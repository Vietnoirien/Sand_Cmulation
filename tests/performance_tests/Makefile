CXX = g++
# Add OpenMP flag to both compilation and linking
CXXFLAGS = -std=c++17 -Wall -Wextra -O3 -march=native -fopenmp \
           -I../../src/grid \
           -I../../src/particle \
           -I../../src/spatial \
           -I../../src/memory

LDFLAGS = -fopenmp

TARGET = performance_tests
SRCS = performance_tests.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(LDFLAGS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)
