cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add GoogleTest
add_subdirectory(extern/googletest)

# Include paths
include_directories(include)

# Main application
add_executable(MyProjectApp
        main.cpp
        src/my_utils.cpp
        src/library.cpp
        src/file_io.cpp
        src/Book.cpp
)
target_include_directories(MyProjectApp PRIVATE include)

# Test executable
add_executable(MyProjectTests
        test/my_utils_tests.cpp
        src/my_utils.cpp
        src/library.cpp
        src/file_io.cpp
        src/Book.cpp
)
target_include_directories(MyProjectTests PRIVATE include)
target_link_libraries(MyProjectTests gtest_main)

# Enable testing
enable_testing()
include(GoogleTest)
gtest_discover_tests(MyProjectTests)
