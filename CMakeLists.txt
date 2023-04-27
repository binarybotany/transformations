cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(transformations LANGUAGES C CXX)

set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_BUILD_DOCS OFF)

set(EXTERN_DIR ${PROJECT_SOURCE_DIR}/extern)

add_subdirectory(${EXTERN_DIR}/glad)
add_subdirectory(${EXTERN_DIR}/glfw)
add_subdirectory(${EXTERN_DIR}/glm)
add_subdirectory(${EXTERN_DIR}/stbi)

add_subdirectory(src bin)