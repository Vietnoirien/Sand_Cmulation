CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -fopenmp -I../../src/grid -I../../src/particle -I../../src/spatial

TARGET = spatial_tests
SRCS = spatial_tests.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
