add_subdirectory(statement)

add_library(parser lexer.h lexer.cc parser.h parser.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:parser>
    PARENT_SCOPE)
