CXX = c++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic

all: percolation_tests

percolation_tests: percolation.cpp percolation_stats.cpp test.cpp percolation.h percolation_stats.h
	$(CXX) $(CXXFLAGS) percolation.cpp percolation_stats.cpp test.cpp -o percolation_tests

test: percolation_tests
	./percolation_tests

clean:
	rm -f percolation_tests
