cmake_minimum_required(VERSION 3.15)
project(Fusion VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(${PROJECT_NAME} STATIC
  src/test.cpp
)

# Example
add_executable(example example/main.cpp)
target_compile_features(example PRIVATE cxx_std_23)
target_include_directories(example PRIVATE ./include)
target_link_libraries(example Fusion)
