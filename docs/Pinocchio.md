# Pinocchio
Pinocchio is a library for evaluating robot dynamics.

This was written using version 2.6.9

## Installation
This follows the installation instructions in the package documentation.

### Part I: Setup robotpkg

1. Install dependencies
```
sudo apt install -qqy lsb-release gnupg2 curl
```

2. Add robotpkg as source repository to apt:
```
echo "deb [arch=amd64] http://robotpkg.openrobots.org/packages/debian/pub $(lsb_release -cs) robotpkg" | sudo tee /etc/apt/sources.list.d/robotpkg.list
```

3. Register the authentication certificate of robotpkg:
```
curl http://robotpkg.openrobots.org/packages/debian/robotpkg.key | sudo apt-key add -
```

4. Update
```
sudo apt-get update
```

### Part II: Install Pinocchio

1. Install from robotpkg
```
sudo apt install -qqy robotpkg-py310-pinocchio  # Adapt your desired python version here
```

2. Configure environment variables
```
export PATH=/opt/openrobots/bin:$PATH
export PKG_CONFIG_PATH=/opt/openrobots/lib/pkgconfig:$PKG_CONFIG_PATH
export LD_LIBRARY_PATH=/opt/openrobots/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/opt/openrobots/lib/python3.10/site-packages:$PYTHONPATH # Adapt your desired python version here
```

### Part III: Install CppAD
1. Clone the [cppad](https://github.com/coin-or/CppAD) onto your computer and check the the
   most recent release.
```
git clone https://github.com/coin-or/CppAD
git checkout 20220000.4
```

2. Build with CMake
```
cd CppAD
mkdir build
cd build
cmake .. 
cmake --build . -j4  # or however many cores you want to use
```

3. Check the build (optional)
You can check to make sure it built correctly by running the test suite. 
Note this will take several minutes.
```
cmake --build . --target check -j4
```

4. Install
Installs to `CMAKE_INSTALL_PREFIX`, which by default is `/usr/local`.
```
sudo cmake --build . --target install
```

5. Clone CppADCodeGen
```
cd ../..  # or to wherever you want to clone it (not in the cppad build directory)
git clone https://github.com/joaoleal/CppADCodeGen.git
git checkout v2.4.3
```

6. Build CppADCodeGen
```
cd CppADCodeGen
mkdir build
cd build
cmake -D GOOGLETEST_GIT=ON -D LLVM_VERSION=14 ..  # replace with your version
cmake --build .
sudo cmake --build . --target install
```


## Linking Against the installed library with CMake
The library, by default, gets installed in `/opt/openrobots`. We can bring the library into
ours using the CMake `find_package` command. To do so, we first create a cache variable to 
allow the user to specify the installation location, which is used by `find_package` as
the first root folder to search in. 

```cmake
set(pinocchio_ROOT /opt/openrobots CACHE FILEPATH "Path to Pinocchio installation."
find_package(pinocchio 2.6 REQUIRED)
```

We also need to link against CppAD, which unfortunately doesn't install a CMake package, but
does install a pkgconfig file. We can use the following CMake code to create a target
from the info in the pkgconfig file:

```cmake
find_package(PkgConfig REQUIRED)
pkg_check_modules(CPPAD cppad REQUIRED)
add_library(cppad INTERFACE)
target_link_libraries(cppad INTERFACE ${CPPAD_LINK_LIBRARIES})
target_include_directories(cppad INTERFACE ${CPPAD_INCLUDE_DIRS})
add_library(cppad::cppad ALIAS cppad)
```

This provides the `pinocchio::pinocchio` target we can use in our project, e.g.

```cmake
add_executable(pinocchio_test pinocchio_test.cpp)
target_link_library(pinocchio_test PRIVATE pinocchio::pinocchio)

```





