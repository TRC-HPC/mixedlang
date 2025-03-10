# C++ demo to call C++ library.
add_executable(democpp demo.cpp)
target_include_directories(democpp PRIVATE ${CXX_INC_DIRS})
target_link_libraries(democpp PRIVATE mixedlang)
set_target_properties(democpp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# C demo to call C++ library.
add_executable(democ demo.c)
target_include_directories(democ PRIVATE ${C_INC_DIRS})
target_link_libraries(democ PRIVATE mixedlang)
set_target_properties(democ PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Fortran demo to use Fortran module in C++ library.
add_executable(demof demo.F90)
target_include_directories(demof PUBLIC ${Fortran_MODULE_DIRECTORY} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(demof mixedlang)
set_target_properties(demof PROPERTIES
    LINKER_LANGUAGE Fortran
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Copy all Python files to the build directory
add_custom_target(py_scripts ALL
    ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/apps/py_scripts/ ${CMAKE_BINARY_DIR}
    COMMAND echo "apps/py_scripts/* copied to build/"
)