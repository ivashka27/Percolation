TARGET = percolation_test

CXX = g++
CXXFLAGS = -Wall -std=c++11 -Iinclude

SRCS = main.cpp src/Percolation.cpp src/PercolationStats.cpp

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(shell find . -name "*.o")
	rm -f $(TARGET)