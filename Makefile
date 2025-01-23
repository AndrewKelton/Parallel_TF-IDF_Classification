# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20 -pthread -Iinclude

# Define the executable name
TARGET = tfidf

# Define the source files
SRCS = src/main.cpp src/count_vectorization.cpp src/document.cpp src/preprocess.cpp

# Define the object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

# Compile each object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(TARGET) $(OBJS)