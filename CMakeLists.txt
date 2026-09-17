cmake_minimum_required(VERSION 4.4)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

project(Fusion VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Library
add_library(Fusion STATIC)

target_sources(Fusion
    PUBLIC
        FILE_SET CXX_MODULES
        FILES
            src/renderer.cppm
)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_MODULE_STD ON)

# Example
add_executable(example example/main.cpp)

set_property(TARGET example PROPERTY CXX_MODULE_STD ON)

target_link_libraries(example PRIVATE Fusion)
