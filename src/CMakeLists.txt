cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

set(HEADERS 
    rendering_context.h 
    rendering_program.h 
    rotating_cube.h 
    window_data.h)

set(SOURCES 
    rendering_context.cc 
    rendering_program.cc 
    rotating_cube.cc)

add_executable(transformations main.cc ${HEADERS} ${SOURCES})

target_include_directories(transformations PRIVATE ${EXTERN_DIR}/glfw/include)
target_include_directories(transformations PRIVATE ${EXTERN_DIR}/glad/include)
target_include_directories(transformations PRIVATE ${EXTERN_DIR}/glm/glm)
target_include_directories(transformations PRIVATE ${EXTERN_DIR}/stbi/include)

target_link_libraries(transformations PUBLIC glad glfw glm stbi)