file(GLOB SRC_DEMO_APP *.cpp)

add_executable(demoapp ${SRC_DEMO_APP})
target_include_directories(demoapp PRIVATE ${CXX_INC_DIRS} ../pymixedlang_mod)
target_link_libraries(demoapp PRIVATE mixedlang pymixedlang Python::Python)
set_target_properties(demoapp PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
