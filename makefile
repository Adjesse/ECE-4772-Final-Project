# Makefile for a simple C++ program

CC = g++   # Compiler
CFLAGS = -std=c++11 -Wall   # Compiler flags

all: Final_seq Final_tbb   # The default target is to build both programs

Final_seq: Final_seq.cpp   # Target for building program1 from source1.cpp
	$(CC) $(CFLAGS) Final_seq.cpp -o Final_seq

Final_tbb: Final_tbb.cpp   # Target for building program2 from source2.cpp
	$(CC) $(CFLAGS) Final_tbb.cpp -ltbb12 -o Final_tbb

clean:   # Target for cleaning up generated files
	rm -f Final_seq Final_tbb
