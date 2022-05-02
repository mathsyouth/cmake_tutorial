# Step 1: A Basic Starting Point

The most basic project is an executable built from source code files.
For simple projects, a three line `CMakeLists.txt` file is all that is
required. This will be the starting point for our tutorial. Create a
[`CMakeLists.txt`](./Step1/CMakeLists.txt) file in the [`Step1`](./Step1) directory that looks like:

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name
project(Tutorial)

# add the executable
add_executable(Tutorial tutorial.cxx)
```

Note that this example uses lower case commands in the `CMakeLists.txt` file.
Upper, lower, and mixed case commands are supported by CMake. The source
code for [`tutorial.cxx`](./Step1/tutorial.cxx) is provided in the [`Step1`](./Step1) directory and can be
used to compute the square root of a number.

## Build and Run

That's all that is needed - we can build and run our project now! First, run
the `cmake` executable or the
`cmake-gui` to configure the project and then build it
with your chosen build tool.

For example, from the command line create a build directory:

```shell
mkdir Step1_build
```

Next, navigate to the build directory and run CMake to configure the project
and generate a native build system:

```shell
cd Step1_build
cmake ../Step1
```

Then call that build system to actually compile/link the project:

```shell
cmake --build .
```

Finally, try to use the newly built `Tutorial` with these commands:

```shell
./Tutorial 4294967296
./Tutorial 10
./Tutorial
```

## Adding a Version Number and Configured Header File

The first feature we will add is to provide our executable and project with a
version number. While we could do this exclusively in the source code, using
`CMakeLists.txt``provides more flexibility.

First, modify the [`Step2/CMakeLists.txt`](./Step2/CMakeLists.txt) file to use the `project` command
to set the project name and version number.

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)
```

Then, configure a header file to pass the version number to the source
code:

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

Since the configured file will be written into the binary tree, we
must add that directory to the list of paths to search for include
files. Add the following lines to the end of the [`Step2/CMakeLists.txt`](./Step2/CMakeLists.txt) file:

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

where [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html) specifies include directories to use when compiling a given target. The named `<target>` must have been created by a command such as `add_executable()` or `add_library()`.

Using your favorite editor, create [`Step2/TutorialConfig.h.in`](./Step2/TutorialConfig.h.in) in the source
directory with the following contents:

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

When CMake configures this header file the values for
`@Tutorial_VERSION_MAJOR@` and `@Tutorial_VERSION_MINOR@` will be
replaced.

Next modify [`Step2/tutorial.cxx`](./Step2/tutorial.cxx) to include the configured header file, [`Step2/TutorialConfig.h`](./Step2/TutorialConfig.h)

Finally, let's print out the executable name and version number by updating
[`Step2/tutorial.cxx`](./Step2/tutorial.cxx) as follows:

```C++ standard
  if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
  }
```

## Specify the C++ Standard

Next let's add some C++11 features to our project by replacing `atof` with
`std::stod` in [`Step2/tutorial.cxx`](./Step2/tutorial.cxx).  At the same time, remove `#include <cstdlib>`.

```C++ standard
  const double inputValue = std::stod(argv[1]);
```

We will need to explicitly state in the CMake code that it should use the
correct flags. The easiest way to enable support for a specific C++ standard
in CMake is by using the `CMAKE_CXX_STANDARD` variable. For this
tutorial, set the `CMAKE_CXX_STANDARD` variable in the
`CMakeLists.txt` file to `11` and `CMAKE_CXX_STANDARD_REQUIRED`
to `True`. Make sure to add the `CMAKE_CXX_STANDARD` declarations above the
call to `add_executable`.

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

## Rebuild

Let's build our project again:

```shell
mkdir Step2_build && cd Step2_build
cmake ../Step2
cmake --build .
```

Now we can try to use the newly built `Tutorial` with same commands as before:

```shell
./Tutorial 4294967296
./Tutorial 10
./Tutorial
```

Check that the version number is now reported when running the executable without any arguments.
