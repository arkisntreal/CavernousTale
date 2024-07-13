cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQURED True)

project(CavernousTale)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/sdl2/")

include_directories(headers)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

file(GLOB SOURCES src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(SDL2 REQUIRED)
target_link_libraries(CavernousTale SDL2)

find_package(SDL2_image REQUIRED)
target_link_libraries(CavernousTale SDL2_image)

