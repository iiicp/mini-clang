cmake_minimum_required(VERSION 3.18)

# set compiler version
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_COMPILE_WARNING_AS_ERROR OFF)

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    project(LCC)

    include_directories("${CMAKE_BINARY_DIR}/include"
            "${CMAKE_BINARY_DIR}/llvm/include"
            "include"
            "llvm/include")
    link_directories("${CMAKE_BINARY_DIR}")

    set(LCC_BUILT_STANDALONE 1)
endif ()

if (CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR AND NOT MSVC_IDE)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif ()

set(CLANG_VERSION_STRING "1.0")
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/include/clang/Basic/Version.inc.in
        ${CMAKE_CURRENT_BINARY_DIR}/include/clang/Basic/Version.inc)

include_directories(BEFORE
        ${CMAKE_CURRENT_BINARY_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(llvm)
add_subdirectory(lib)
add_subdirectory(tools)

enable_testing()
add_subdirectory(unittests/clang)
