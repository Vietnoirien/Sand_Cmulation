CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../../src/grid -I../../src/particle

TARGET = interaction_tests
SRCS = interaction_tests.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
