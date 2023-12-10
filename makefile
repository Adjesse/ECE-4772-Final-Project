# Makefile for a simple C++ program

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source file
SOURCE = Final_seq.cpp

# Executable name
EXECUTABLE = Final_seq

# Rule to build the executable
$(EXECUTABLE): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(SOURCE) -o $(EXECUTABLE)

# Clean rule to remove the executable
clean:
	rm -f $(EXECUTABLE)
