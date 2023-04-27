cmake_minimum_required(VERSION 3.4...3.20 FATAL_ERROR)

project(stbi LANGUAGES CXX)

add_library(stbi include/stb_image.h stb_image.cc)
target_include_directories(stbi PUBLIC include)