add_library(parser error.h result.h)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:parser>
    PARENT_SCOPE)
