# Step 8: Adding Support for a Testing Dashboard

Adding support for submitting our test results to a dashboard is simple. We
already defined a number of tests for our project in
[`Testing Support`](./Installing%20and%20Testing.md#testing-support). Now we just have to run
those tests and submit them to a dashboard. To include support for dashboards
we include the [`CTest`](https://cmake.org/cmake/help/latest/module/CTest.html#module:CTest) module in our top-level [`Step9/CMakeLists.txt`](./Step9/CMakeLists.txt).

Replace:

```cmake
# enable testing
enable_testing()
```

With:

```cmake
# enable dashboard scripting
include(CTest)
```

The `CTest` module will automatically call `enable_testing()`, so we
can remove it from our CMake files.

We will also need to acquire a `CTestConfig.cmake` file to be placed in the
top-level directory where we can specify information to CTest about the
project. It contains:

* The project name
* The project "Nightly" start time
  * The time when a 24 hour "day" starts for this project.
* The URL of the CDash instance where the submission's generated documents will be sent

One has been provided for you in this directory.  It would normally be
downloaded from the `Settings` page of the project on the CDash
instance that will host and display the test results.  Once downloaded from
CDash, the file [`Step9/CTestConfig.cmake`](./Step9/CTestConfig.cmake) should not be modified locally.

```cmake
set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
```

The [`ctest`](https://cmake.org/cmake/help/latest/manual/ctest.1.html#manual:ctest(1)) executable will read in this file when it runs.
To create a simple dashboard you can run the `cmake`
executable or the `cmake-gui` to configure the project,
but do not build it yet. Instead, change directory to the binary tree, and then
run:

```shell
ctest [-VV] -D Experimental
```

Remember, for multi-config generators (e.g. Visual Studio), the configuration
type must be specified:

```shell
ctest [-VV] -C Debug -D Experimental
```

Or, from an IDE, build the `Experimental` target.

The `ctest` executable will build and test the project and
submit the results to Kitware's public dashboard:
https://my.cdash.org/index.php?project=CMakeTutorial.
