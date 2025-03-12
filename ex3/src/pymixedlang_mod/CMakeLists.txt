file(GLOB SRC_MIXEDLANG_MODULE *.cpp)

# Make the Python pymixedlang module available to standalone Python
# Use sys.path.append('lib') to find it.
Python_add_library(pymixedlang SHARED ${SRC_MIXEDLANG_MODULE})
target_include_directories(pymixedlang PRIVATE ${CXX_INC_DIRS})
target_link_libraries(pymixedlang PUBLIC mixedlang)
set_target_properties(pymixedlang PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib PREFIX "")
