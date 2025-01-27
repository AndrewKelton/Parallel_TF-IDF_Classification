# Define the compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -Wunused-private-field -std=c++17 -pthread -L/opt/homebrew/lib -larmadillo -I/opt/homebrew/include -I/opt/homebrew/include/ -Iinclude -Iinclude/OleanderStemmingLibrary/src -Wdeprecated-declarations

# Define the executable name
TARGET = tfidf

# Define the source files
SRCS = src/main_2.cpp src/count_vectorization.cpp src/document.cpp src/preprocess.cpp src/file_operations.cpp src/tfidf.cpp

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