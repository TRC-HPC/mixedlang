cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_COMPILER g++)

project(MixedLang VERSION 1.0.0 DESCRIPTION "Mixed Language Programming Example" LANGUAGES CXX C)

set(CMAKE_CONFIGURATION_TYPES Debug Release)
set(CMAKE_CONFIGURATION_TYPES "{CMAKE_CONFIGURATION_TYPES}" CACHE STRING
    "Reset the configurations"
    FORCE)

# Default build type to Release
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release")
endif()

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Map the root path (e.g. /home/username/mixedlang/ex3) to "."
# This prevents this long path prefix being output in log files when logging __FILE__.
list(APPEND CMAKE_CXX_FLAGS
    -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.
)

list(APPEND CMAKE_C_FLAGS
    -fmacro-prefix-map=${CMAKE_SOURCE_DIR}=.
)

enable_language(Fortran)
set(CMAKE_Fortran_MODULE_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find any required packages your code might need
find_package(Python COMPONENTS Development.Embed Development.Module REQUIRED)

# Include the found package include dirs as required
# Note: Not required if packages support newer style package format (see CXX_LINK_DIRS below)
set(CXX_INC_DIRS
    # ${SOME_PACKAGE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

set(C_INC_DIRS
    # ${SOME_PACKAGE_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Append other package library dirs as required
set(CXX_LINK_DIRS
    # Old style
    #  SOME_PACKAGE_LIBRARIES
    # new_style
    #  some_package::some_package
)

add_subdirectory(src)
add_subdirectory(apps)

# Copy the Python modules directory to the build directory
add_custom_target(py_modules ALL
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/modules/ ${CMAKE_BINARY_DIR}/modules/
    COMMAND echo "modules copied to build/"
)
