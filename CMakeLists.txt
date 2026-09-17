cmake_minimum_required(VERSION 4.4)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")

project(Fusion VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
)

FetchContent_Declare(
    volk
    GIT_REPOSITORY https://github.com/zeux/volk.git
    GIT_TAG dbc6d2fbb0cc07d3e42669f594a991ac35f666d9
)

FetchContent_MakeAvailable(glfw volk)

# Library
add_library(${PROJECT_NAME} STATIC)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw volk::volk)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic -g)
target_include_directories(${PROJECT_NAME} PRIVATE src ${glfw_SOURCE_DIR}/include ${volk_SOURCE_DIR})
target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)

set(FUSION_MODULES
    src/core.cppm
    src/types.cppm
    src/renderer/vulkan/instance.cppm
    src/renderer/vulkan/utils.cppm
    src/renderer/vulkan/surface.cppm
    src/renderer/vulkan/physical_device.cppm
    src/platform/window/window.cppm
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    list(APPEND FUSION_MODULES
        src/renderer/vulkan/debug_messenger.cppm
    )
endif()

set(FUSION_SOURCES src/platform/window/window_glfw.cpp)

target_sources(${PROJECT_NAME}
    PUBLIC
        FILE_SET CXX_MODULES FILES ${FUSION_MODULES}
        FILE_SET SOURCES FILES ${FUSION_SOURCES}
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_MODULE_STD ON)

# Example
add_executable(example example/main.cpp)
set_property(TARGET example PROPERTY CXX_MODULE_STD ON)
target_link_libraries(example PRIVATE Fusion)
target_compile_definitions(example PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    $<$<CONFIG:Release>:NDEBUG>
)
