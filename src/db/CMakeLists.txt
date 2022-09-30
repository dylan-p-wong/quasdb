
add_library(db quas.h quas.cc)

set(ALL_OBJECT_FILES
    ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:db>
    PARENT_SCOPE)
