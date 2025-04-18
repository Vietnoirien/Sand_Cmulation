CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I../../src/memory

TARGET = memory_tests
SRCS = memory_tests.cpp
OBJS = $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: clean
