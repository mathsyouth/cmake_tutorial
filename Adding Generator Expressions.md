# Step 10: Adding Generator Expressions

[`Generator expressions`](https://cmake.org/cmake/help/latest/manual/cmake-generator-expressions.7.html#manual:cmake-generator-expressions(7)) are evaluated
during build system generation to produce information specific to each build
configuration.

`Generator expressions` are allowed in the context of many target properties, such as [`LINK_LIBRARIES`](https://cmake.org/cmake/help/latest/prop_tgt/LINK_LIBRARIES.html#prop_tgt:LINK_LIBRARIES),
[`INCLUDE_DIRECTORIES`](https://cmake.org/cmake/help/latest/prop_tgt/INCLUDE_DIRECTORIES.html#prop_tgt:INCLUDE_DIRECTORIES), [`COMPILE_DEFINITIONS`](https://cmake.org/cmake/help/latest/prop_tgt/COMPILE_DEFINITIONS.html#prop_tgt:COMPILE_DEFINITIONS) and others.
They may also be used when using commands to populate those properties, such as
[`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries), [`target_include_directories`](https://cmake.org/cmake/help/latest/command/target_include_directories.html#command:target_include_directories),
[`target_compile_definitions`](https://cmake.org/cmake/help/latest/command/target_compile_definitions.html#command:target_compile_definitions) and others.

`Generator expressions`  may be used to enable conditional linking, conditional definitions used when compiling, conditional include directories and more. The conditions may be based on the build configuration, target properties, platform information or any other queryable information.

There are different types of `generator expressions` including Logical, Informational, and Output expressions.

Logical expressions are used to create conditional output. The basic
expressions are the `0` and `1` expressions. A `$<0:...>` results in the
empty string, and `<1:...>` results in the content of `...`.  They can also
be nested.

A common usage of `generator expressions` is to
conditionally add compiler flags, such as those for language levels or
warnings. A nice pattern is to associate this information to an `INTERFACE`
target allowing this information to propagate. Let's start by constructing an
`INTERFACE` target and specifying the required C++ standard level of `11`
instead of using [`CMAKE_CXX_STANDARD`](https://cmake.org/cmake/help/latest/variable/CMAKE_CXX_STANDARD.html#variable:CMAKE_CXX_STANDARD)

So the following code:

```cmake
# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

Would be replaced with:

```cmake
add_library(tutorial_compiler_flags INTERFACE)
target_compile_features(tutorial_compiler_flags INTERFACE cxx_std_11)
```

**Note**:  This upcoming section will require a change to the
[`cmake_minimum_required`](https://cmake.org/cmake/help/latest/command/cmake_minimum_required.html#command:cmake_minimum_required) usage in the code.  The Generator Expression
that is about to be used was introduced in `3.15`.  Update the call to require
that more recent version:

```cmake
cmake_minimum_required(VERSION 3.15)
```

Next we add the desired compiler warning flags that we want for our project. As
warning flags vary based on the compiler we use the `COMPILE_LANG_AND_ID`
generator expression to control which flags to apply given a language and a set
of compiler ids as seen below:

```cmake
set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
target_compile_options(tutorial_compiler_flags INTERFACE
  "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>>"
  "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
```

Looking at this we see that the warning flags are encapsulated inside a
`BUILD_INTERFACE` condition. This is done so that consumers of our installed
project will not inherit our warning flags.

**Exercise**: Modify `MathFunctions/CMakeLists.txt` so that all targets have
a [`target_link_libraries`](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#command:target_link_libraries) call to `tutorial_compiler_flags`.
