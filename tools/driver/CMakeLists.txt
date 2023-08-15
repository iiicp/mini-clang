SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
add_executable(lcc main.cc)

target_link_libraries(lcc
        PRIVATE
        LCC
        LLVM
        # lccCodeGen
        # lccLexer
        # lccParser
        # lccSema
        )