cmake_minimum_required(VERSION 3.15)

include(hunter/HunterGate.cmake)
HunterGate(
    URL "https://github.com/cpp-pm/hunter/archive/v0.23.258.tar.gz"
    SHA1 "062a19ab13ce8dffa9a882b6ce3e43bdabdf75d3"
)

project(pathtracer)

string(TOUPPER ${PROJECT_NAME} PROJECT_UPPER_NAME)

file(GLOB ${PROJECT_UPPER_NAME}_SOURCE
    ${CMAKE_SOURCE_DIR}/main.cpp
    ${CMAKE_SOURCE_DIR}/source/*
    ${CMAKE_SOURCE_DIR}/source/ptmath/*
)

file(GLOB ${PROJECT_UPPER_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/include/*
    ${CMAKE_SOURCE_DIR}/include/ptmath/*
)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)
include(hunter/HunterPackages.cmake)


add_executable(${PROJECT_NAME} ${${PROJECT_UPPER_NAME}_SOURCE} ${${PROJECT_UPPER_NAME}_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glew OpenGL::GL Assimp::assimp stb::stb)
