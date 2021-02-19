cmake_minimum_required(VERSION 3.10)

project(RealTest)

file(GLOB header "*.h")
file(GLOB src "*.cpp")

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/header/")
INCLUDE_DIRECTORIES("/usr/include/")
INCLUDE_DIRECTORIES("/usr/include/SDL2/")
INCLUDE_DIRECTORIES("/usr/local/USD/include/")

add_executable(${PROJECT_NAME} ${header} ${src})

target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/USD/lib")
target_link_directories(${PROJECT_NAME} PUBLIC "/usr/local/USD/lib64")

target_link_libraries(${PROJECT_NAME} PUBLIC vulkan SDL2)
