cmake_minimum_required(VERSION 3.16.3)

project(cmake_tutorial)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(Adder)

add_subdirectory(external/glfw)

# Could use include in a standard way
target_include_directories(${PROJECT_NAME} PRIVATE Adder)
# Path for build folder
target_link_directories(${PROJECT_NAME} PRIVATE Adder)
target_link_libraries(${PROJECT_NAME} add)
