cmake_minimum_required(VERSION 3.22.1)

project(cmake_tutorial VERSION 1.0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#FIND LIBRARY FOR CMAKE 3.13.4 (NO [REQUIRED] IN CMAKE YET)
find_library(myglewlib glew 
    NAMES glew2 GLEW GLEW2 
    PATHS "/mnt/e/code/glew-2.2.0/build"
    PATH_SUFFIXES "lib"
    DOC "glew library")
if (${myglewlib} STREQUAL myglewlib-NOTFOUND)
	message(FATAL_ERROR "no glew, sad day")
else()
	message(STATUS "glew found! as ${myglewlib}")
endif()

find_library(myglulib glu NAMES GLU DOC "glu library")
if (${myglulib} STREQUAL myglulib-NOTFOUND)
	message(FATAL_ERROR "no glu, sad day")
else()
	message(STATUS "glu found! as ${myglulib}")
endif()

find_library(mygllib gl NAMES GL DOC "gl library")
if (${mygllib} STREQUAL mygllib-NOTFOUND)
	message(FATAL_ERROR "no gl, sad day")
else()
	message(STATUS "gl found! as ${mygllib}")
endif()

# adder use logic
option(USE_ADDER "A simple library for adding 2 floats" ON)
if(USE_ADDER)
   message(STATUS "Use Adder")
   add_subdirectory(Adder)
   list(APPEND EXTRA_INCLUDE_DIRS "Adder")
   list(APPEND EXTRA_LIB_DIRS "Adder") 
   list(APPEND EXTRA_LINKS add)
endif()

configure_file(cmake_tutorialConfig.h.in cmake_tutorialConfig.h)

add_executable(${PROJECT_NAME} main.cpp)

# Include generated cmake_tutorialConfig.h file
target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_BINARY_DIR}
)

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
    PUBLIC external/glfw/include
    ${EXTRA_INCLUDE_DIRS})

# Path for build folder
target_link_directories(${PROJECT_NAME} 
    PRIVATE external/glfw/src
    ${EXTRA_LIB_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${EXTRA_LINKS} glfw ${myglewlib} ${myglulib} ${mygllib})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/cmake_tutorialConfig.h" DESTINATION include)

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
include(CPack)
