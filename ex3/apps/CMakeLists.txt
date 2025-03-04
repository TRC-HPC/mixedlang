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