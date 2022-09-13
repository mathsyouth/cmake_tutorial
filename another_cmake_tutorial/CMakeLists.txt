cmake_minimum_required(VERSION 3.22.1)

project(cmake_tutorial VERSION 1.0)

configure_file(cmake_tutorialConfig.h.in cmake_tutorialConfig.h)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} main.cpp)

# Include generated cmake_tutorialConfig.h file
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_BINARY_DIR}
)


add_subdirectory(Adder)

# DOWNLOAD ALL THE SUBMODULES
find_package(Git QUIET)

cmake_path(HAS_PARENT_PATH PROJECT_SOURCE_DIR PROJECT_HAS_PARENT)
if(PROJECT_HAS_PARENT)
    cmake_path(GET PROJECT_SOURCE_DIR PARENT_PATH PROJECT_PARENT_DIR)
endif()
if(GIT_FOUND AND EXISTS "${PROJECT_PARENT_DIR}/.git")
# Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)
    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                        RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

# CHECK ALL THE SUBMODULES
if(NOT EXISTS "${PROJECT_SOURCE_DIR}/external/glfw/CMakeLists.txt")
    message(FATAL_ERROR "The glfw submodules was not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

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
