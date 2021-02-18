cmake_minimum_required(VERSION 3.10)

project(RealTest)

file(GLOB header "./header/*.h")
file(GLOB src "./src/*.cpp")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/header/")
INCLUDE_DIRECTORIES("/usr/include/")

add_executable(${PROJECT_NAME} ${header} ${src})

target_link_libraries(${PROJECT_NAME} PUBLIC vulkan SDL2)
