cmake_minimum_required(VERSION 3.13)
list(APPEND CMAKE_MESSAGE_CONTEXT mpcbench)
project(MPCBenchmarks VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

cmake_policy(SET CMP0077 NEW)

#############################################
# Options
#############################################

#############################################
# Dependencies
#############################################
include(FetchContent)

# Add libfmt
list(APPEND CMAKE_MESSAGE_CONTEXT fmt)
FetchContent_Declare(fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt
  GIT_TAG a33701196adfad74917046096bf5a2aa0ab0bb50  # version 9.1.0
  )
FetchContent_MakeAvailable(fmt)
list(POP_BACK CMAKE_MESSAGE_CONTEXT)

# Add Eigen
list(APPEND CMAKE_MESSAGE_CONTEXT eigen)
set(EIGEN_BUILD_DOC OFF CACHE BOOL "Enable creation of Eigen documentation." FORCE)
set(BUILD_TESTING OFF CACHE BOOL "Enable creation of Eigen tests." FORCE)
set(EIGEN_BUILD_TESTING OFF CACHE BOOL "Enable creation of Eigen tests." FORCE)
FetchContent_Declare(eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3147391d946bb4b6c68edd901f2add6ac1f31f8c  # version 3.4.0
  )
FetchContent_MakeAvailable(eigen)
list(POP_BACK CMAKE_MESSAGE_CONTEXT)

set(pinocchio_ROOT /opt/openrobots/ CACHE FILEPATH "Path to Pinocchio installation.")
set(PINOCCHIO_SOURCE_DIR $ENV{HOME}/Code/pinocchio CACHE FILEPATH "Path to Pinocchio source code")
set(PINOCCHIO_MODEL_DIR ${PINOCCHIO_SOURCE_DIR}/models CACHE FILEPATH "Path to urdf models")
find_package(pinocchio 2.6 REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CPPAD cppad REQUIRED)
add_library(cppad INTERFACE)
target_link_libraries(cppad INTERFACE ${CPPAD_LINK_LIBRARIES})
target_include_directories(cppad INTERFACE ${CPPAD_INCLUDE_DIRS})
add_library(cppad::cppad ALIAS cppad)

#############################################
# Build
#############################################
add_compile_options(-Wall -Wextra -pedantic -Werror)

include(CMakePrintHelpers)
cmake_print_variables(PINOCCHIO_MODEL_DIR)
add_subdirectory(pinocchio)
