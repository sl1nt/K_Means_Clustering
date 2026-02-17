cmake_minimum_required(VERSION 4.1)
project(k_means_clustering)

set(CMAKE_CXX_STANDARD 23)

find_package(raylib REQUIRED)
find_package(imgui REQUIRED)

add_executable(k_means_clustering main.cpp
        Application.cpp
        Application.hpp
        external/rlImGui-main/rlImGui.cpp)
target_link_libraries(k_means_clustering raylib imgui)
