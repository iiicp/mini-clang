SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(test_pp pp_test.cc)
target_link_libraries(test_pp PRIVATE GTest::GTest gtest_main LCC LLVM)

add_test(add_gtests test_pp)