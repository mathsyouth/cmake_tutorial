# Step 6: Adding a Custom Command and Generated File

Suppose, for the purpose of this tutorial, we decide that we never want to use
the platform `log` and `exp` functions and instead would like to
generate a table of precomputed values to use in the `mysqrt` function.
In this section, we will create the table as part of the build process,
and then compile that table into our application.

First, let's remove the check for the `log` and `exp` functions in
[`Step7/MathFunctions/CMakeLists.txt`](./Step7/MathFunctions/CMakeLists.txt). Then remove the check for `HAVE_LOG` and
`HAVE_EXP` from `mysqrt.cxx`. At the same time, we can remove
`#include <cmath>`.

In the [`Step7/MathFunctions`](./Step7/MathFunctions/) subdirectory, a new source file named
`MakeTable.cxx` has been provided to generate the table.

After reviewing the file, we can see that the table is produced as valid C++
code and that the output filename is passed in as an argument.

The next step is to add the appropriate commands to the
[`Step7/MathFunctions/CMakeLists.txt`](./Step7/MathFunctions/CMakeLists.txt) file to build the MakeTable executable and
then run it as part of the build process. A few commands are needed to
accomplish this.

First, at the top of [`Step7/MathFunctions/CMakeLists.txt`](./Step7/MathFunctions/CMakeLists.txt), the executable for
`MakeTable` is added as any other executable would be added.

```cmake
add_executable(MakeTable MakeTable.cxx)
```

Then we add a custom command that specifies how to produce `Table.h`
by running MakeTable.

```cmake
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )
```

Next we have to let CMake know that `mysqrt.cxx` depends on the generated
file `Table.h`. This is done by adding the generated `Table.h` to the list
of sources for the library `MathFunctions`.

```cmake
add_library(MathFunctions
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )
```

We also have to add the current binary directory to the list of include
directories so that `Table.h` can be found and included by `mysqrt.cxx`.

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
          )
```

Now let's use the generated table. First, modify `mysqrt.cxx` to include
`Table.h`. Next, we can rewrite the `mysqrt` function to use the table:

```C++ standard
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
```

Run the `cmake` executable or the `cmake-gui` to configure the project and then build it with your chosen build tool.

When this project is built it will first build the `MakeTable` executable.
It will then run `MakeTable` to produce `Table.h`. Finally, it will
compile `mysqrt.cxx` which includes `Table.h` to produce the
`MathFunctions` library.

Run the Tutorial executable and verify that it is using the table.
