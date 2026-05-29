CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = percolation
SOURCES = main.cpp percolation.cpp percolation_stats.cpp
all:
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)
run: all
	./$(TARGET)
clean:
	rm -f $(TARGET)