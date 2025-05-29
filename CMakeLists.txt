cmake_minimum_required(VERSION 3.16)

project(protocol)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

add_subdirectory(deps)

add_subdirectory(tool)

enable_testing()

add_subdirectory(test)