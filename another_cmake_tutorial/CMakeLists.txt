cmake_minimum_required(VERSION 3.16.3)

project(cmake_tutorial)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(Adder)

add_subdirectory(external/glfw)

# Could use include in a standard way
target_include_directories(${PROJECT_NAME} 
    PRIVATE Adder 
    PUBLIC external/glfw/include
)
# Path for build folder
target_link_directories(${PROJECT_NAME} 
    PRIVATE Adder
    PRIVATE external/glfw/src)
target_link_libraries(${PROJECT_NAME} add glfw)
