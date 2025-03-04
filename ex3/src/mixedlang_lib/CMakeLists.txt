# Shared C++ library for mixed language programming demo.

file(GLOB SRC_MIXEDLANG_LIB *.cpp)

add_library(mixedlang SHARED ${SRC_MIXEDLANG_LIB})
target_include_directories(mixedlang PRIVATE ${CXX_INC_DIRS} ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(mixedlang PUBLIC ${CXX_LINK_DIRS})
set_target_properties(mixedlang PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
