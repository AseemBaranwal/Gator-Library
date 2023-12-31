# Makefile for the project

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Executable name
EXECUTABLE = main

# Source files
SRCS = Book.cpp \
       MinHeap.cpp \
       Node.cpp \
       RedBlackTree.cpp \
       Reservation.cpp \
       main.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Target for the main executable
$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o gatorLibrary

# Target for each object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to clean up generated files
clean:
	rm -f *.o *_output_file.txt gatorLibrary

# Target to run the executable
run: $(EXECUTABLE)
	./$(EXECUTABLE)
	$(MAKE) clean