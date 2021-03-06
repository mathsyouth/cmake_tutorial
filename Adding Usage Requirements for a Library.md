# Step 3: Adding Usage Requirements for a Library

Usage requirements allow for far better control over a library or executable's
link and include line while also giving more control over the transitive
property of targets inside CMake. The primary commands that leverage usage
requirements are:

* [`target_compile_definitions`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html)
* [`target_compile_options`](https://cmake.org/cmake/help/latest/command/target_compile_options.html)
* [`target_include_directories](https://cmake.org/cmake/help/latest/command/target_include_directories.html)
* [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html)

Let's refactor our code from [Step2: Adding a Library](./Adding%20a%20Library.md) to use the
modern CMake approach of usage requirements. We first state that anybody
linking to `MathFunctions` needs to include the current source directory,
while `MathFunctions` itself doesn't. So this can become an `INTERFACE`
usage requirement.

Remember `INTERFACE` means things that consumers require but the producer
doesn't. Add the following lines to the end of
[`Step4/MathFunctions/CMakeLists.txt`](./Step4/MathFunctions/CMakeLists.txt):

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
```

Now that we've specified usage requirements for `MathFunctions` we can safely
remove our uses of the `EXTRA_INCLUDES` variable from the top-level
[`Step4/CMakeLists.txt`](./Step4/CMakeLists.txt), here:

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()
```

And here:

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

Once this is done, run the `cmake` executable or the `cmake-gui` to configure the project and then build it by using:

```shell
mkdir Step4_build && cd Step4_build
cmake ../Step4
cmake --build .
```

from the build directory.
