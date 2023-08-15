add_subdirectory(Basic)
add_subdirectory(Lex)
add_subdirectory(Frontend)
# add_subdirectory(CodeGen)
add_subdirectory(Parse)
# add_subdirectory(Sema)
# add_subdirectory(Support)
set(lcc_src
        ${lccBasic}
        ${lccLex}
        ${lccFrontend}
        ${lccParse}
        )
add_library(LCC STATIC ${lcc_src})