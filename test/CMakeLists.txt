
# Set the minimum CMake version
cmake_minimum_required(VERSION 3.16)

# Project name and C++ standard
project(TFIDFProject LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wunused-private-field -pthread -Wdeprecated-declarations")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/opt/homebrew/include -Iinclude -Iinclude/OleanderStemmingLibrary/src")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/homebrew/lib -larmadillo")

# Source directories
set(SRC_DIR src)
set(COMMON_SOURCES
    ${SRC_DIR}/count_vectorization.cpp
    ${SRC_DIR}/document.cpp
    ${SRC_DIR}/categories.cpp
    ${SRC_DIR}/preprocess.cpp
    ${SRC_DIR}/file_operations.cpp
    ${SRC_DIR}/tfidf.cpp
)

# Define each target and its sources
add_executable(test1 ${SRC_DIR}/main.cpp ${COMMON_SOURCES})
add_executable(test2 ${SRC_DIR}/main_2.cpp ${COMMON_SOURCES})
add_executable(test3 ${SRC_DIR}/main_3.cpp ${COMMON_SOURCES})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/OleanderStemmingLibrary/src)
include_directories(/opt/homebrew/include)

# Link libraries
target_link_libraries(test1 armadillo pthread)
target_link_libraries(test2 armadillo pthread)
target_link_libraries(test3 armadillo pthread)

# Custom clean command
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_BINARY_DIR})
