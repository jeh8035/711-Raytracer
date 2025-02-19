# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -g -std=c++20

# Target executable
TARGET = main

# For deleting the target
TARGET_DEL = main

# Source files
SRCS = main.cpp world.cpp Primitives/camera.cpp Primitives/light.cpp Primitives/material.cpp Primitives/object.cpp Primitives/primitives.cpp Libraries/tinyga/tinyga.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default rule to build and run the executable
all: $(TARGET) run

# Rule to link object files into the target executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the executable
run: $(TARGET)
	./$(TARGET)

# Clean rule to remove generated files
clean:
	rm $(TARGET_DEL) $(OBJS)