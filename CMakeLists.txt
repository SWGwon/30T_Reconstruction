cmake_minimum_required(VERSION 3.12)
project(Reconstruction)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find ROOT (assumes ROOT is installed and environment is set)
find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

# Include directories
include_directories(${ROOT_INCLUDE_DIRS})

set(RATROOT $ENV{RATROOT} CACHE PATH "Path to RATROOT")
include_directories(${RATROOT}/include ${RATROOT}/include/stlplus)
link_directories(${RATROOT}/lib)

include_directories(include)

find_library(READLINE_LIBRARY readline)
include_directories(${READLINE_INCLUDE_DIR})

# Define sources
set(SOURCES
    src/FileReader.cxx
    src/DataProcessor.cxx
    src/CommandLineParser.cxx
    src/HistogramFitter.cxx
    src/main.cxx
    )

# Add executable
add_executable(Reconstruction ${SOURCES})

# Link ROOT libraries
target_link_libraries(Reconstruction ${ROOT_LIBRARIES} RATEvent ${READLINE_LIBRARY})
