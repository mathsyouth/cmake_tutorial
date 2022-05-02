# Step 9: Selecting Static or Shared Libraries

In this section we will show how the [`BUILD_SHARED_LIBS`](https://cmake.org/cmake/help/latest/variable/BUILD_SHARED_LIBS.html#variable:BUILD_SHARED_LIBS) variable can
be used to control the default behavior of [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html#command:add_library),
and allow control over how libraries without an explicit type (`STATIC`,
`SHARED`, `MODULE` or `OBJECT`) are built.

To accomplish this we need to add `BUILD_SHARED_LIBS` to the
top-level `CMakeLists.txt`. We use the [`option`](https://cmake.org/cmake/help/latest/command/option.html#command:option) command as it allows
users to optionally select if the value should be `ON` or `OFF`.

Next we are going to refactor `MathFunctions` to become a real library that
encapsulates using `mysqrt` or `sqrt`, instead of requiring the calling
code to do this logic. This will also mean that `USE_MYMATH` will not control
building `MathFunctions`, but instead will control the behavior of this
library.

The first step is to update the starting section of the top-level
[`Step10/CMakeLists.txt`](./Step10/CMakeLists.txt) to look like:

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions)
```

Now that we have made `MathFunctions` always be used, we will need to update
the logic of that library. So, in `MathFunctions/CMakeLists.txt` we need to
create a SqrtLibrary that will conditionally be built and installed when
`USE_MYMATH` is enabled. Now, since this is a tutorial, we are going to
explicitly require that SqrtLibrary is built statically.

The end result is that [`Step10/MathFunctions/CMakeLists.txt`](./Step10/MathFunctions/CMakeLists.txt) should look like:

```cmake
# add the library that runs
add_library(MathFunctions MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)

  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # first we add the executable that generates the table
  add_executable(MakeTable MakeTable.cxx)

  # add the command to generate the source code
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    )

  # library that just does sqrt
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h
              )

  # state that we depend on our binary dir to find Table.h
  target_include_directories(SqrtLibrary PRIVATE
                             ${CMAKE_CURRENT_BINARY_DIR}
                             )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

# define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions(MathFunctions PRIVATE "EXPORTING_MYMATH")

# install rules
set(installable_libs MathFunctions)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

Next, update [`Step10/MathFunctions/mysqrt.cxx`](./Step10/MathFunctions/mysqrt.cxx) to use the `mathfunctions` and
`detail` namespaces:

```C++ standard
#include <iostream>

#include "MathFunctions.h"

// include the generated table
#include "Table.h"

namespace mathfunctions {
namespace detail {
// a hack square root calculation using simple operations
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    std::cout << "Use the table to help find an initial value " << std::endl;
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (int i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
  }

  return result;
}
}
}
```

We also need to make some changes in `tutorial.cxx`, so that it no longer
uses `USE_MYMATH`:

1. Always include `MathFunctions.h`
1. Always use `mathfunctions::sqrt`
1. Don't include `cmath`

Finally, update [`Step10/MathFunctions/MathFunctions.h`](./Step10/MathFunctions/MathFunctions.h) to use dll export defines:

```C++ standard
#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

namespace mathfunctions {
double DECLSPEC sqrt(double x);
}
```

At this point, if you build everything, you may notice that linking fails
as we are combining a static library without position independent code with a
library that has position independent code. The solution to this is to
explicitly set the [`POSITION_INDEPENDENT_CODE`](https://cmake.org/cmake/help/latest/prop_tgt/POSITION_INDEPENDENT_CODE.html#prop_tgt:POSITION_INDEPENDENT_CODE) target property of
SqrtLibrary to be ``True`` no matter the build type.

```cmake
  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties(SqrtLibrary PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
```

**Exercise**: We modified `MathFunctions.h` to use dll export defines.
Using CMake documentation can you find a helper module to simplify this?
