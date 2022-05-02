# Step 2: Adding a Library

Now we will add a library to our project. This library will contain our own
implementation for computing the square root of a number. The executable can
then use this library instead of the standard square root function provided by
the compiler.

For this tutorial we will put the library into a subdirectory
called [`MathFunctions`](./Step3/MathFunctions). This directory already contains a header file,
`MathFunctions.h`, and a source file `mysqrt.cxx`. The source file has one
function called `mysqrt` that provides similar functionality to the
compiler's `sqrt` function.

Add the following one line `CMakeLists.txt` file to the [`Step3/MathFunctions`](./Step3/MathFunctions/)
directory:

```cmake
add_library(MathFunctions mysqrt.cxx)
```

where [`add_libray`](https://cmake.org/cmake/help/latest/command/add_library.html) adds a library target called `<name>` to be built from the source files listed in the command invocation.

To make use of the new library we will add an `add_subdirectory`
call in the top-level [`Step3/CMakeLists.txt`](./Step3/CMakeLists.txt) file so that the library will get
built. We add the new library to the executable, and add `MathFunctions` as
an include directory so that the `MathFunctions.h` header file can be found.
The last few lines of the top-level [`Step3/CMakeLists.txt`](./Step3/CMakeLists.txt) file should now look
like:

```cmake
# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions)

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/MathFunctions"
                          )
```

where [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html) specifies libraries or flags to use when linking a given target and/or its dependents.

Now let us make the `MathFunctions` library optional. While for the tutorial
there really isn't any need to do so, for larger projects this is a common
occurrence. The first step is to add an option to the top-level
[`Step3/CMakeLists.txt`](./Step3/CMakeLists.txt) file.

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

This option will be displayed in the `cmake-gui` and `cmake`
with a default value of `ON` that can be changed by the user. This setting
will be stored in the cache so that the user does not need to set the value
each time they run CMake on a build directory.

The next change is to make building and linking the `MathFunctions` library
conditional. To do this,  we will create an `if` statement which checks the
value of the option.  Inside the `if` block, put the
`add_subdirectory` command from above with some additional list
commands to store information needed to link to the library and add the
subdirectory as an include directory in the `Tutorial` target.
The end of the top-level [`Step3/CMakeLists.txt`](./Step3/CMakeLists.txt) file will now look like the following:

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add the executable
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )

```

Note the use of the variable `EXTRA_LIBS` to collect up any optional
libraries to later be linked into the executable. The variable
`EXTRA_INCLUDES` is used similarly for optional header files. This is a
classic approach when dealing with many optional components, we will cover
the modern approach in the next step.

The corresponding changes to the source code are fairly straightforward.
First, in [`Step3/tutorial.cxx`](./Step3/tutorial.cxx), include the `MathFunctions.h` header if we
need it:

```C++ standard
#ifdef USE_MYMATH
#  include "MathFunctions.h"
#endif
```

Then, in the same file, make `USE_MYMATH` control which square root
function is used:

```C++ standard
#ifdef USE_MYMATH
  const double outputValue = mysqrt(inputValue);
#else
  const double outputValue = sqrt(inputValue);
#endif
```

Since the source code now requires `USE_MYMATH` we can add it to
[`Step3/TutorialConfig.h.in`](./Step3/TutorialConfig.h.in) with the following line:

```cmake
#cmakedefine USE_MYMATH
```

**Exercise**: Why is it important that we configure `TutorialConfig.h.in`
after the option for `USE_MYMATH`? What would happen if we inverted the two?

Run the `cmake` executable or the `cmake-gui` to configure the project and then build it
with your chosen build tool. Then run the built Tutorial executable.

```shell
mkdir Step3_build && cd Step3_build
cmake ../Step3
cmake --build .
```

Now let's update the value of `USE_MYMATH`. The easiest way is to use the
`cmake-gui` or  `cmake` if you're in the terminal. Or, alternatively, if you want to change the option from the
command-line, try:

```shell
cmake ../Step3 -DUSE_MYMATH=OFF
cmake --build .
```

Rebuild and run the tutorial again.

Which function gives better results, `sqrt` or `mysqrt`?
